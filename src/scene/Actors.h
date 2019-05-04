#ifndef MILK_ACTORS_H
#define MILK_ACTORS_H

#include <array>
#include <string>
#include <queue>
#include <vector>

#include "common/int.h"
#include "math/Vector2.h"

namespace milk
{
	class Actors final
	{
	public:
		static const U8 INDEX_BITS = 24;
		static const U8 GENERATION_BITS = 8;
		static const U16 MAX_FREE_INDECES = 1024;
		static const U16 MAX_ACTORS = 20000;
		
		void Create(std::vector<std::string>& names, std::vector<U32>& ids);
		void Destroy(std::vector<U32>& ids);
		bool Alive(std::vector<U32>& ids);

	private:
		U16 nextIndex_ = 0;
		std::queue<U16> freeIndeces_{};
		std::array<std::string, MAX_ACTORS> names_{};
		std::array<U16, MAX_ACTORS> generations_{};
	};

	namespace ActorUtils 
	{
		U32 Create(Actors& actors, const std::string& name);
		void Destroy(Actors& actors, U32 id);
		bool Alive(Actors& actors, U32 id);
	}
}

#endif