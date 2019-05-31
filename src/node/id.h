#ifndef MILK_ID_H
#define MILK_ID_H

#include <queue>
#include <vector>

#include "data/int.h"

/* 
	This method is based off of the bitsquid entity component system post.
	A web search for 'bitsquid entity component system' will lead to the post if it is still a valid url.

	Node ids are represented by an unsigned 32 bit integer split up into two parts: index and generation.
	
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
namespace milk {
	namespace node_id {
		U32 make(
			std::vector<U16>& generations,
			std::queue<U16>& freeIndeces,
			const U32 INVALID_ID,
			const U32 GENERATION_BITS,
			const int MAX_FREE_INDECES
		);

		void recycle(
			std::vector<U16>& generations,
			std::queue<U16>& freeIndeces,
			const U32 GENERATION_BITS,
			U32 id
		);

		bool valid(
			std::vector<U16>& generations,
			const U32 GENERATION_BITS,
			const U32 INDEX_BITS,
			U32 id
		);
	}
}

#endif