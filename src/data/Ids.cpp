#include "Ids.h"

#include <assert.h>

namespace milk {
	namespace {
		const U32 MAX = 20000;
		const U32 MAX_FREE = 1024;
		const U32 IDX_BITS = 16;
		const U32 GEN_BITS = 16;
	}
}

milk::U32 milk::id::create(IdData& ids) {
	U32 index;
	if (ids.freeIndeces.size() > MAX_FREE) {
		index = ids.freeIndeces.front();
		ids.freeIndeces.pop();
	}
	else {
		ids.generations.push_back(0);
		index = ids.generations.size() - 1;
		assert(index <= MAX);
	}
	U32 id = index | (ids.generations[index] << GEN_BITS);
	if (id == INVALID) {
		++ids.generations[index];
		id = index | (ids.generations[index] << GEN_BITS);
	}
	return id;
}

void milk::id::remove(IdData& ids, U32 id) {
	U16 index = id & ~(1 << GEN_BITS);
	assert(index < ids.generations.size());
	++ids.generations[index];
	ids.freeIndeces.push(index);
}

bool milk::id::valid(IdData& ids, U32 id) {
	U16 index = id & ~(1 << GEN_BITS);
	assert(index < ids.generations.size());
	U16 generation = id >> IDX_BITS & ~(1 << GEN_BITS);
	return ids.generations[index] == generation;
}
