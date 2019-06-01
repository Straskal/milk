#include "node_id.h"

milk::U32 milk::node_id::make(
	std::vector<U16>& generations,
	std::queue<U16>& freeIndeces,
	const milk::U32 GENERATION_BITS,
	const int MAX_FREE_INDECES
) {
	U16 index;
	if (freeIndeces.size() > MAX_FREE_INDECES) {
		index = freeIndeces.front();
		freeIndeces.pop();
	}
	else {
		generations.push_back(0);
		index = generations.size() - 1;
	}
	return index | (generations[index] << GENERATION_BITS);
}

void milk::node_id::recycle(
	std::vector<milk::U16>& generations,
	std::queue<milk::U16>& freeIndeces,
	const milk::U32 GENERATION_BITS,
	milk::U32 id
) {
	U16 index = id & ~(1 << GENERATION_BITS);
	++generations[index];
	freeIndeces.push(index);
}

bool milk::node_id::valid(
	std::vector<U16>& generations,
	const U32 GENERATION_BITS,
	const U32 INDEX_BITS,
	milk::U32 id
) {
	U16 index = id & ~(1 << GENERATION_BITS);
	U16 generation = id >> INDEX_BITS & ~(1 << GENERATION_BITS);
	return generations[index] == generation;
}