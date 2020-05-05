#include "uid.h"

static const milk::u32 UID_GENERATION_BITS = 22;
static const milk::u32 UID_INDEX_BITS = 8;
static const milk::u32 MAX_FREE_INDECES = 1024;

milk::uid_data::uid_data()
{
	// Push generation 0, which will represent an invalid id.
	generations.push_back(0);
}

milk::u32 milk::uid_new(uid_data* data)
{
	u16 index;
	if (data->free_indeces.size() > MAX_FREE_INDECES) {
		index = data->free_indeces.front();
		data->free_indeces.pop();
	}
	else {
		data->generations.push_back(0);
		index = data->generations.size() - 1;
	}
	return index | (data->generations[index] << UID_GENERATION_BITS);
}

void milk::uid_free(uid_data* data, u32 id)
{
	u16 index = id & ~(1 << UID_GENERATION_BITS);
	++data->generations[index];
	data->free_indeces.push(index);
}

bool milk::uid_alive(const uid_data* data, u32 id)
{
	u16 index = id & ~(1 << UID_GENERATION_BITS);
	u16 generation = id >> UID_INDEX_BITS & ~(1 << UID_GENERATION_BITS);
	return data->generations[index] == generation;
}
