#ifndef _SAMPLE_H_
#define _SAMPLE_H_

#include <string>

#include "int.h"

namespace milk
{
	enum class sample_state
	{
		PLAYING,
		PAUSED,
		STOPPED
	};

	struct sound_data
	{
		std::string path;
		void* handle;
		int ref_count;
	};

	struct sound
	{
		u32 uid;
		sample_state state;
		int channel;
		float volume;
	};

	struct music_data
	{
		std::string path;
		void* handle;
		int ref_count;
	};

	struct music
	{
		u32 uid;
		sample_state state;
		float volume;
	};
}

#endif