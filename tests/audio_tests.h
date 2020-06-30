#ifndef __AUDIO_TESTS_H__
#define __AUDIO_TESTS_H__

#include "milkassert.h"

static void mockLock()
{
}

static void mockUnlock()
{
}

TEST_CASE(initAudio_InitializesSounds)
{
	Audio audio;

	ACT(initializeAudio(&audio));

	for (int i = 0; i < MAX_LOADED_SOUNDS; i++)
	{
		ASSERT_NULL(audio.sounds[i].samples);
		ASSERT_EQ(0, audio.sounds[i].length);
		ASSERT_EQ(0, audio.sounds[i].channelCount);
	}

	END_ASSERTS();
}

TEST_CASE(initAudio_InitializesSoundSlots)
{
	Audio audio;

	ACT(initializeAudio(&audio));

	for (int i = 0; i < MAX_SOUND_SLOTS; i++)
	{
		ASSERT_NULL(audio.soundSlots[i].soundData);
		ASSERT_EQ(STOPPED, audio.soundSlots[i].state);
		ASSERT_EQ(0, audio.soundSlots[i].remainingLength);
		ASSERT_NULL(audio.soundSlots[i].position);
		ASSERT_EQ(0, audio.soundSlots[i].volume);
	}

	END_ASSERTS();
}

TEST_CASE(initAudio_InitializesStreams)
{
	Audio audio;

	ACT(initializeAudio(&audio));

	for (int i = 0; i < MAX_OPEN_STREAMS; i++)
	{
		ASSERT_EQ(0, audio.streams[i].position);
		ASSERT_EQ(0, audio.streams[i].start);
		ASSERT_EQ(0, audio.streams[i].end);
		ASSERT_NULL(audio.streams[i].file);
		ASSERT_NULL(audio.streams[i].chunk);
		ASSERT_EQ(0, audio.streams[i].channelCount);
	}

	END_ASSERTS();
}

TEST_CASE(initAudio_InitializesStreamSlot)
{
	Audio audio;

	ACT(initializeAudio(&audio));

	for (int i = 0; i < MAX_OPEN_STREAMS; i++)
	{
		ASSERT_NULL(audio.streamSlot.stream);
		ASSERT_EQ(STOPPED, audio.streamSlot.state);
		ASSERT_EQ(0, audio.streamSlot.volume);
	}

	END_ASSERTS();
}

TEST_CASE(initAudio_InitializesMasterVolume)
{
	Audio audio;

	ACT(initializeAudio(&audio));

	ASSERT_EQ(MAX_VOLUME, audio.masterVolume);
	END_ASSERTS();
}

TEST_CASE(playSound_WhenIndexOutOfBounds_DoesNothing)
{
	Audio audio;
	initializeAudio(&audio);

	ACT(playSound(&audio, -1, 0, 0));
	ACT(playSound(&audio, MAX_LOADED_SOUNDS + 1, 0, 0));
}

TEST_CASE(playSound_WhenSampleAtIndexNotLoaded_DoesNothing)
{
	Audio audio;
	initializeAudio(&audio);

	ACT(playSound(&audio, 0, -1, 0));
	ACT(playSound(&audio, MAX_SOUND_SLOTS + 1, 0, 0));
}

TEST_CASE(playSound_WhenSampleLengthIsZero_DoesNothing)
{
	Audio audio;
	initializeAudio(&audio);

	audio.lock = mockLock;
	audio.unlock = mockUnlock;
	audio.sounds[0].length = 0;

	ACT(playSound(&audio, 0, 0, 0));

	ASSERT_NEQ(&audio.sounds[0], audio.soundSlots[0].soundData);
	END_ASSERTS();
}

TEST_CASE(playSound_SetsSlot)
{
	Audio audio;
	initializeAudio(&audio);

	audio.lock = mockLock;
	audio.unlock = mockUnlock;
	uint8_t buffer[1];
	audio.sounds[0].samples = buffer;
	audio.sounds[0].length = 1;

	ACT(playSound(&audio, 0, 0, 50));

	ASSERT_EQ(&audio.sounds[0], audio.soundSlots[0].soundData);
	ASSERT_EQ(buffer, audio.soundSlots[0].position);
	ASSERT_EQ(1, audio.soundSlots[0].remainingLength);
	ASSERT_EQ(PLAYING, audio.soundSlots[0].state);
	ASSERT_EQ(50, audio.soundSlots[0].volume);
	END_ASSERTS();
}

TEST_CASE(playSound_ClampsVolumeToMin)
{
	Audio audio;
	initializeAudio(&audio);

	uint8_t buffer[1];
	audio.lock = mockLock;
	audio.unlock = mockUnlock;
	audio.sounds[0].samples = buffer;

	ACT(playSound(&audio, 0, 0, -10));

	ASSERT_EQ(0, audio.soundSlots[0].volume);
	END_ASSERTS();
}

TEST_CASE(playSound_ClampsVolumeToMax)
{
	Audio audio;
	initializeAudio(&audio);

	uint8_t buffer[1];
	audio.lock = mockLock;
	audio.unlock = mockUnlock;
	audio.sounds[0].samples = buffer;

	ACT(playSound(&audio, 0, 0, MAX_VOLUME + 10));

	ASSERT_EQ(MAX_VOLUME, audio.soundSlots[0].volume);
	END_ASSERTS();
}

TEST_CASE(setMasterVolume_ClampsToMinValue)
{
	Audio audio;
	initializeAudio(&audio);

	ACT(setMasterVolume(&audio, -10));
	ASSERT_EQ(0, audio.masterVolume);
	END_ASSERTS();
}

TEST_CASE(setMasterVolume_ClampsToMaxValue)
{
	Audio audio;
	initializeAudio(&audio);

	ACT(setMasterVolume(&audio, MAX_VOLUME + 10));
	ASSERT_EQ(MAX_VOLUME, audio.masterVolume);
	END_ASSERTS();
}

TEST_CASE(stopSound_WhenIndexOutOfBounds_DoesNothing)
{
	Audio audio;
	initializeAudio(&audio);

	audio.lock = mockLock;
	audio.unlock = mockUnlock;
	ACT(stopSound(&audio, -10));
	ACT(stopSound(&audio, MAX_SOUND_SLOTS + 10));
}

TEST_CASE(stopSound_StopsSound)
{
	Audio audio;
	initializeAudio(&audio);

	audio.lock = mockLock;
	audio.unlock = mockUnlock;
	audio.lock = mockLock;
	audio.unlock = mockUnlock;
	audio.soundSlots[0].soundData = &audio.sounds[0];
	audio.soundSlots[0].state = PLAYING;

	ACT(stopSound(&audio, 0));

	ASSERT_NULL(audio.soundSlots[0].soundData);
	ASSERT_EQ(STOPPED, audio.soundSlots[0].state);

	END_ASSERTS();
}

TEST_CASE(pauseSound_WhenIndexOutOfBounds_DoesNothing)
{
	Audio audio;
	initializeAudio(&audio);

	audio.lock = mockLock;
	audio.unlock = mockUnlock;
	ACT(pauseSound(&audio, -10));
	ACT(pauseSound(&audio, MAX_SOUND_SLOTS + 10));
}

TEST_CASE(pauseSound_PausesSound)
{
	Audio audio;
	initializeAudio(&audio);

	audio.lock = mockLock;
	audio.unlock = mockUnlock;
	audio.soundSlots[0].soundData = &audio.sounds[0];
	audio.soundSlots[0].state = PLAYING;

	ACT(pauseSound(&audio, 0));

	ASSERT_EQ(PAUSED, audio.soundSlots[0].state);
	END_ASSERTS();
}

TEST_CASE(resumeSound_WhenIndexOutOfBounds_DoesNothing)
{
	Audio audio;
	initializeAudio(&audio);

	audio.lock = mockLock;
	audio.unlock = mockUnlock;
	ACT(resumeSound(&audio, -10));
	ACT(resumeSound(&audio, MAX_SOUND_SLOTS + 10));
}

TEST_CASE(resumeSound_resumesSound)
{
	Audio audio;
	initializeAudio(&audio);

	audio.lock = mockLock;
	audio.unlock = mockUnlock;
	audio.soundSlots[0].soundData = &audio.sounds[0];
	audio.soundSlots[0].state = PAUSED;

	ACT(resumeSound(&audio, 0));

	ASSERT_EQ(PLAYING, audio.soundSlots[0].state);
	END_ASSERTS();
}

#endif
