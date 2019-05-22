#include "Ids.h"

milk::Ids::Ids()
	: IDX_BITS{ 16 }
	, GEN_BITS{ 16 }
	, MAX{ 20000 }
	, MAX_FREE{ 1024 } {}

milk::Ids::Ids(U8 idxbits, U8 genbits, U16 max, U16 maxFree) 
	: IDX_BITS{ idxbits }
	, GEN_BITS{ genbits }
	, MAX{ max }
	, MAX_FREE{ maxFree } {}

milk::U32 milk::Ids::create() {
	U32 index;
	if (freeIndeces_.size() > MAX_FREE) {
		index = freeIndeces_.front();
		freeIndeces_.pop();
	}
	else {
		generations_.push_back(0);
		index = generations_.size() - 1;
		assert(index <= MAX);
	}
	U32 id = index | (generations_[index] << GEN_BITS);
	if (id == INVALID) {
		++generations_[index];
		id = index | (generations_[index] << GEN_BITS);
	}
	return id;
}

void milk::Ids::remove(U32 id) {
	U16 index = id & ~(1 << GEN_BITS);
	assert(index < generations_.size());
	++generations_[index];
	freeIndeces_.push(index);
}

bool milk::Ids::valid(U32 id) const {
	U16 index = id & ~(1 << GEN_BITS);
	assert(index < generations_.size());
	U16 generation = id >> IDX_BITS & ~(1 << GEN_BITS);
	return generations_[index] == generation;
}