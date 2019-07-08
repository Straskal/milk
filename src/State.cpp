#include "State.h"

#include "audio/audio.h"

bool milk::State::initialized = false;

milk::Time* milk::State::time = nullptr;
milk::Mouse* milk::State::mouse = nullptr;
milk::Keyboard* milk::State::keyboard = nullptr;
milk::SoundCache* milk::State::sounds = nullptr;
milk::AudioPlayer* milk::State::audioPlayer = nullptr;
milk::MusicCache* milk::State::music = nullptr;