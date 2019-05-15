#ifndef MILK_IDS_H
#define MILK_IDS_H

#include <array>
#include <assert.h>
#include <string>
#include <queue>

#include "common/int.h"

namespace milk 
{
	template<U8 IDX_BITS = 24, U8 GEN_BITS = 8, U16 SIZE = 20000, U16 MAX_FREE = 1024>
	class Ids
	{
	public:
		const static U32 INVALID = 0;

		U32 create()
		{
			U32 index;
			if (freeIndeces_.size() > MAX_FREE)
			{
				index = freeIndeces_.front();
				freeIndeces_.pop();
			}
			else
			{
				index = nextIndex_++;
				assert(nextIndex_ <= SIZE);
			}
			U32 id = index | (generations_[index] << GEN_BITS);
			if (id == INVALID)
			{
				++generations_[index];
			}
			return index | (generations_[index] << GEN_BITS);
		}

		void remove(U32 id)
		{
			U16 index = id & ~GEN_BITS;
			++generations_[index];
			freeIndeces_.push(index);
		}

		bool valid(U32 id) const
		{
			U16 index = id & ~GEN_BITS;
			U16 generation = id >> IDX_BITS & ~(1 << GEN_BITS);
			return generations_[index] == generation;
		}

	private:
		U16 nextIndex_ = 0;
		std::queue<U16> freeIndeces_{};
		std::array<U16, SIZE> generations_{};
	};
}

#endif