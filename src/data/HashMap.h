#ifndef MILK_HASHMAP_H
#define MILK_HASHMAP_H

#include <unordered_map>

namespace milk {
	template<class T, class U>
	using HashMap = std::unordered_map<T, U>;
}

#endif