#include "id.h"

milk::u32 milk::id::make(milk::IdData& data, const milk::u32 GENERATION_BITS, const int MAX_FREE_INDECES) {
	u16 index;
	if (data.freeIndeces.size() > MAX_FREE_INDECES) {
		index = data.freeIndeces.front();
		data.freeIndeces.pop();
	}
	else {
		data.generations.push_back(0);
		index = data.generations.size() - 1;
	}
	return index | (data.generations[index] << GENERATION_BITS);
}

void milk::id::recycle(milk::IdData& data, const milk::u32 GENERATION_BITS, milk::u32 id) {
	u16 index = id & ~(1 << GENERATION_BITS);
	++data.generations[index];
	data.freeIndeces.push(index);
}

bool milk::id::valid(const milk::IdData& data, const u32 GENERATION_BITS, const u32 INDEX_BITS, milk::u32 id) {
	u16 index = id & ~(1 << GENERATION_BITS);
	u16 generation = id >> INDEX_BITS & ~(1 << GENERATION_BITS);
	return data.generations[index] == generation;
}