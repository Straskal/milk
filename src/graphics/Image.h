#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <string>

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
		ImageData* data;
	};
}

#endif
