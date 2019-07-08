#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <string>

#include "int.h"

namespace milk
{
	struct ImageData
	{
		std::string path;
		void* handle;
		int refCount;
		int width;
		int height;
	};

	struct Image
	{
		u32 uid;
	};
}

#endif
