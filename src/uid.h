#ifndef _UID_H_
#define _UID_H_

#include <queue>
#include <vector>

#include "int.h"

/*
	This method is based off of the bitsquid entity component system post.
	A web search for 'bitsquid entity component system' will lead to the post if it is still a valid url.

	Ids are represented by an unsigned 32 bit integer split up into two parts: index and generation.

	Each time an index is "recycled", its generation is incremented by 1 and it is pushed into the freeIndeces queue.
	A recycled index is only popped from freeIndeces when freeIndeces.size > MAX_FREE_INDECES.

	Max unique ids at one time is (2^INDEX_BITS)
	An id will be reused only once it is destroyed (2^GENERATION_BITS * MAX_FREE_INDECES) times.

	Example:
	INDEX_BITS = 16
	GENERATION_BITS = 16
	MAX_FREE_INDECES = 1024

	Max unique ids at one time is 65,536.
	An id will be duplicated only once it is recycled 67,108,864‬ times.
*/
namespace milk
{
	struct UID
	{
		UID();

		std::queue<u16> freeIndeces;
		std::vector<u16> generations;
	};

	namespace id
	{
		static u32 INVALID_ID = 0;

		u32 make(UID* data, u32 GENERATION_BITS, u32 MAX_FREE_INDECES);
		void recycle(UID* data, u32 GENERATION_BITS, u32 id);
		bool valid(const UID* data, u32 GENERATION_BITS, u32 INDEX_BITS, u32 id);
	}
}

#endif
