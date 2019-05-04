#include "Actors.h"

#include <assert.h>

namespace milk
{
	void Actors::Create(std::vector<std::string>& names, std::vector<U32>& ids)
	{
		assert(names.size() == ids.size());
		for (int i = 0; i < names.size(); ++i)
		{
			U32 index;
			if (freeIndeces_.size() > MAX_FREE_INDECES)
			{
				index = freeIndeces_.front();
				freeIndeces_.pop();
			}
			else
			{
				index = nextIndex_++;
				assert(nextIndex_ <= MAX_ACTORS);
			}
			ids[i] = index | (generations_[index] << GENERATION_BITS);
			names_[i] = names[i];
		}
	}

	void Actors::Destroy(std::vector<U32>& ids)
	{
		for (int i = 0; i < ids.size(); ++i)
		{
			U16 index = ids[i] & ~GENERATION_BITS;
			names_[index] = "";
			++generations_[index];
			freeIndeces_.push(index);
		}
	}

	bool Actors::Alive(std::vector<U32>& ids)
	{
		for (int i = 0; i < ids.size(); ++i)
		{
			U16 index = ids[i] & ~GENERATION_BITS;
			U16 generation = ids[i] >> INDEX_BITS & ~(1 << GENERATION_BITS);
			if (generations_[index] != generation)
			{
				return false;
			}
		}
		return true;
	}

	namespace ActorUtils
	{
		U32 Create(Actors& actors, const std::string& name)
		{
			std::vector<std::string> names{ name };
			std::vector<U32> ids(1);
			actors.Create(names, ids);
			return ids[0];
		}

		void Destroy(Actors& actors, U32 id)
		{
			std::vector<U32> ids{ id };
			actors.Destroy(ids);
		}

		bool Alive(Actors& actors, U32 id)
		{
			std::vector<U32> ids{ id };
			return actors.Alive(ids);
		}
	}
}
