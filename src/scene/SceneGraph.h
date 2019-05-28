#ifndef MILK_ACTORS_H
#define MILK_ACTORS_H

#include <string>
#include <queue>
#include <unordered_map>
#include <vector>

#include "data/int.h"
#include "math/Vector2.h"

namespace milk {
	class SceneGraph {
	public:
		static const int MAX;
		static const int MAX_FREE;
		static const U32 IDX_BITS;
		static const U32 GEN_BITS;
		static const U32 INVALID;

		U32 add(const std::string& name, Vector2 position);
		void remove(U32 id);
		bool alive(U32 id);
		std::string getName(U32 id);
		void setName(U32 id, const std::string& name);
		U32 getByName(const std::string& name);
		Vector2 getPosition(U32 id);
		void setPosition(U32 id, Vector2 position);
		U32 getTags(U32 id);
		void setTags(U32 id, U32 mask);

	private:
		std::queue<U16> freeIndeces_;
		std::vector<U16> generations_;
		std::vector<U32> destroyed_;
		std::vector<std::string> names_;
		std::unordered_map<U32, int> nameidmap_;
		std::unordered_map<int, U32> nameidxmap_;
		std::unordered_map<U32, Vector2> positions_;
		std::unordered_map<U32, U32> tags_;
	};
}

#endif