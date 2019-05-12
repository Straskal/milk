#ifndef MILK_FASTMAP_H
#define MILK_FASTMAP_H

#include <assert.h>

#include "Array.h"
#include "HashMap.h"
#include "int.h"

namespace milk
{
	template<class T>
	class FastMap 
	{
	public:
		void push(U32 id, const T& t) 
		{
			array_.push_back(t);
			idxMap_.insert(std::make_pair(id, array_.size() - 1));
		}

		void remove(U32 id) 
		{
			U32 idx = idxMap_.at(id);
			if (idx < array_.size() - 1) 
			{
				array_[idx] = array_[array_.size() - 1];
			}
			array_.pop_back();
			idxMap_.erase(id);
		}

		T& lookup(U32 id)
		{
			U32 idx = idxMap_.at(id);
			return array_[idx];
		}

		T& operator[] (U32 idx) 
		{
			assert(idx < array_.size());
			return array_[idx];
		}

		U32 size() 
		{
			return array_.size();
		}

	private:
		Array<T> array_{};
		HashMap<U32, U32> idxMap_{};
	};
}

#endif