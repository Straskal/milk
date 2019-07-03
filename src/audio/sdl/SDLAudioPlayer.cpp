#include "SDLAudioPlayer.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <queue>
#include <unordered_map>

#include "SDL.h"
#include "SDL_mixer.h"

#include "audio/Music.h"
#include "audio/SampleState.h"
#include "audio/Sound.h"

static const int NUM_CHANNELS = 32;
static const int MIX_CHUNK_SIZE = 2048;
static const int STEREO_CHANNELS = 2;

static const int INVALID_CHANNEL = -1;

static const int LOOP = -1;
static const int NO_LOOP = 0;

static std::array<milk::Sound*, NUM_CHANNELS> channels;
static std::array<float, NUM_CHANNELS> channel_volume;
static std::queue<int> free_channels;

static milk::Music* current_music = nullptr;

static void on_channel_finished(int channelnum)
{
	milk::Sound* sound = channels[channelnum];
	sound->channel = INVALID_CHANNEL;
	sound->state = milk::SampleState::STOPPED;
	channels[channelnum] = nullptr;
	free_channels.push(channelnum);
}

static void on_music_finished()
{
	current_music->state = milk::SampleState::STOPPED;
	current_music = nullptr;
}

milk::SDLAudioPlayer::SDLAudioPlayer()
	: m_masterVolume{1.f}
{
}

bool milk::SDLAudioPlayer::init()
{
	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
		std::cout << "SDL_Init: Failed to initialize audio: " << SDL_GetError() << std::endl;
		return false;
	}

	int flags = MIX_INIT_OGG | MIX_INIT_MP3;
	if (Mix_Init(flags) != flags) {
		std::cout << "SDL_Mixer: Failed to initialize support for Ogg Vorbis and MP3: " << Mix_GetError() << std::endl;
		return false;
	}

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, STEREO_CHANNELS, MIX_CHUNK_SIZE) == -1) {
		std::cout << "SDL_Mixer: Failed to open audio: " << Mix_GetError() << std::endl;
		return false;
	}

	Mix_AllocateChannels(NUM_CHANNELS);
	for (int i = 0; i < NUM_CHANNELS; ++i) {
		channels[i] = nullptr;
		channel_volume[i] = 1.f;
		free_channels.push(i);
	}

	Mix_ChannelFinished(on_channel_finished);
	Mix_HookMusicFinished(on_music_finished);
	return true;
}

void milk::SDLAudioPlayer::free()
{
	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void milk::SDLAudioPlayer::setMasterVolume(const float volume)
{
	m_masterVolume = std::min(std::max(volume, 0.f), 1.f);
	int v = (int)(m_masterVolume * MIX_MAX_VOLUME);

	for (int i = 0; i < NUM_CHANNELS; ++i) {
		Mix_Volume(i, (int)(channel_volume[i] * v));
	}

	if (current_music != nullptr) {
		Mix_VolumeMusic((int)(current_music->volume * v));
	}
}

float milk::SDLAudioPlayer::getMasterVolume() const
{
	return m_masterVolume;
}

void milk::SDLAudioPlayer::playSound(Sound* sound, const float fadeTime)
{
	stopSound(sound, 0.f);

	int channelnum = free_channels.front();
	Mix_Volume(channelnum, sound->volume * (m_masterVolume * MIX_MAX_VOLUME));
	if (Mix_FadeInChannel(channelnum, (Mix_Chunk*)sound->data->handle, NO_LOOP, (int)(fadeTime * 1000)) == -1) {
		std::cout << "Mix_PlayChannel: " << Mix_GetError() << std::endl;
		return;
	}

	free_channels.pop();
	sound->channel = channelnum;
	sound->state = SampleState::PLAYING;
	channels[channelnum] = sound;
	channel_volume[channelnum] = sound->volume;
}

void milk::SDLAudioPlayer::loopSound(Sound* sound, const float fadeTime)
{
	stopSound(sound, 0.f);

	int channelnum = free_channels.front();
	Mix_Volume(channelnum, sound->volume * (m_masterVolume * MIX_MAX_VOLUME));
	if (Mix_FadeInChannel(channelnum, (Mix_Chunk*)sound->data->handle, LOOP, (int)(fadeTime * 1000)) == -1) {
		std::cout << "Mix_PlayChannel: " << Mix_GetError() << std::endl;
		return;
	}

	free_channels.pop();
	sound->channel = channelnum;
	sound->state = SampleState::PLAYING;
	channels[channelnum] = sound;
	channel_volume[channelnum] = sound->volume;
}

void milk::SDLAudioPlayer::pauseSound(Sound* sound)
{
	if (sound->state == SampleState::PLAYING) {
		sound->state = SampleState::PAUSED;
		Mix_Pause(sound->channel);
	}
}

void milk::SDLAudioPlayer::resumeSound(Sound* sound)
{
	if (sound->state == SampleState::PAUSED) {
		sound->state = SampleState::PLAYING;
		Mix_Resume(sound->channel);
	}
}

void milk::SDLAudioPlayer::stopSound(Sound* sound, const float fadeTime)
{
	if (sound->state != SampleState::STOPPED) {
		sound->state = SampleState::STOPPED;
		Mix_FadeOutChannel(sound->channel, (int)(fadeTime * 1000));
	}
}

void milk::SDLAudioPlayer::setSoundVolume(Sound* sound, const float volume)
{
	sound->volume = std::min(std::max(volume, 0.f), 1.f);

	if (sound->channel != INVALID_CHANNEL) {
		int v = (int)(sound->volume * (m_masterVolume * MIX_MAX_VOLUME));
		channel_volume[sound->channel] = v;
		Mix_Volume(sound->channel, v);
	}
}

void milk::SDLAudioPlayer::playMusic(Music* music, const float fadeTime)
{
	Mix_HaltMusic();

	Mix_VolumeMusic(music->volume * (m_masterVolume * MIX_MAX_VOLUME));
	if (Mix_FadeInMusic((Mix_Music*)music->data->handle, NO_LOOP, (int)(fadeTime * 1000)) == -1) {
		std::cout << "Mix_PlayMusic: " << Mix_GetError() << std::endl;
		return;
	}

	music->state = SampleState::PLAYING;
	current_music = music;
}

void milk::SDLAudioPlayer::loopMusic(Music* music, const float fadeTime)
{
	Mix_HaltMusic();

	Mix_VolumeMusic(music->volume * (m_masterVolume * MIX_MAX_VOLUME));
	if (Mix_FadeInMusic((Mix_Music*)music->data->handle, LOOP, (int)(fadeTime * 1000)) == -1) {
		std::cout << "Mix_PlayMusic: " << Mix_GetError() << std::endl;
		return;
	}

	music->state = SampleState::PLAYING;
	current_music = music;
}

void milk::SDLAudioPlayer::pauseMusic(Music* music)
{
	if (music->state == SampleState::PLAYING) {
		music->state = SampleState::PAUSED;
		Mix_PauseMusic();
	}
}

void milk::SDLAudioPlayer::resumeMusic(Music* music)
{
	if (music->state == SampleState::PAUSED) {
		music->state = SampleState::PLAYING;
		Mix_ResumeMusic();
	}
}

void milk::SDLAudioPlayer::stopMusic(Music* music, const float fadeTime)
{
	if (music->state != SampleState::STOPPED) {
		music->state = SampleState::STOPPED;
		Mix_FadeOutMusic((int)(fadeTime * 1000));
	}
}

void milk::SDLAudioPlayer::setMusicVolume(Music* music, const float volume)
{
	music->volume = std::min(std::max(volume, 0.f), 1.f);

	if (music->state == SampleState::PLAYING) {
		Mix_VolumeMusic((int)(music->volume * (m_masterVolume * MIX_MAX_VOLUME)));
	}
}
