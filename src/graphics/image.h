#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <string>

#include "int.h"

namespace milk
{
	struct image_data
	{
		std::string path;
		void* handle;
		int ref_count;
		int width;
		int height;
	};

	struct image
	{
		u32 uid;
	};
}

#endif
