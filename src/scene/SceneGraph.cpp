#include "SceneGraph.h"

#include <assert.h>

namespace milk {
	const int milk::SceneGraph::MAX = 20000;
	const int milk::SceneGraph::MAX_FREE = 1024;
	const milk::U32 milk::SceneGraph::IDX_BITS = 16;
	const milk::U32 milk::SceneGraph::GEN_BITS = 16;
	const milk::U32 milk::SceneGraph::INVALID = 0;

	namespace {
		U32 makeId(
			std::vector<U16>& generations,
			std::queue<U16>& freeIndeces
		) {
			U16 index;
			if (freeIndeces.size() > SceneGraph::MAX_FREE) {
				index = freeIndeces.front();
				freeIndeces.pop();
			}
			else {
				generations.push_back(0);
				index = generations.size() - 1;
				assert(index <= SceneGraph::MAX);
			}
			U32 id = index | (generations[index] << SceneGraph::GEN_BITS);
			if (id == SceneGraph::INVALID) {
				++generations[index];
				id = index | (generations[index] << SceneGraph::GEN_BITS);
			}
			return id;
		}

		void deleteId(
			U32 id,
			std::vector<U16>& generations,
			std::queue<U16>& freeIndeces,
			std::vector<U32> destroyed
		) {
			U16 index = id & ~(1 << SceneGraph::GEN_BITS);
			assert(index < generations.size());
			++generations[index];
			freeIndeces.push(index);
			destroyed.push_back(id);
		}

		bool validId(
			U32 id,
			std::vector<U16>& generations
		) {
			U16 index = id & ~(1 << SceneGraph::GEN_BITS);
			assert(index < generations.size());
			U16 generation = id >> SceneGraph::IDX_BITS & ~(1 << SceneGraph::GEN_BITS);
			return generations[index] == generation;
		}

		void insertName(
			U32 id,
			std::vector<std::string>& names,
			std::unordered_map<U32, int>& nameidmap,
			std::unordered_map<int, U32>& nameidxmap,
			const std::string& name
		) {
			names.push_back(name);
			int nameidx = names.size() - 1;
			nameidmap.insert(std::make_pair(id, nameidx));
			nameidxmap.insert(std::make_pair(nameidx, id));
		}

		void deleteName(
			U32 id,
			std::vector<std::string>& names,
			std::unordered_map<U32, int>& nameidmap,
			std::unordered_map<int, U32>& nameidxmap
		) {
			size_t lastidx = names.size() - 1;
			if (names.size() > 1) {
				// Swap the deleted element with last, then remove last element.
				int nameidx = nameidmap.at(id);
				U32 lastid = nameidxmap.at(lastidx);
				names[nameidx] = names[lastidx];
				nameidmap.at(lastid) = nameidx;
				nameidxmap.at(nameidx) = lastid;
			}
			names.pop_back();
			nameidmap.erase(id);
			nameidxmap.erase(lastidx);
		}

		void updateName(
			U32 id,
			std::vector<std::string>& names,
			std::unordered_map<U32, int>& nameidmap,
			const std::string& name
		) {
			int nameidx = nameidmap.at(id);
			names[nameidx] = name;
		}

		std::string queryNamesById(
			U32 id,
			std::vector<std::string>& names,
			std::unordered_map<U32, int>& nameidmap
		) {
			int nameidx = nameidmap.at(id);
			return names[nameidx];
		}

		U32 queryIdsByName(
			const std::string& name,
			std::vector<std::string>& names,
			std::unordered_map<int, U32>& nameidxmap
		) {
			for (int i = 0; i < names.size(); ++i) {
				if (name == names[i]) {
					return nameidxmap.at(i);
				}
			}
			return SceneGraph::INVALID;
		}

		void insertPosition(
			U32 id,
			std::unordered_map<U32, Vector2>& positions,
			Vector2 position
		) {
			positions.insert(std::make_pair(id, position));
		}

		void updatePosition(
			U32 id,
			std::unordered_map<U32, Vector2>& positions,
			Vector2 position
		) {
			positions.at(id) = position;
		}

		void deletePosition(
			U32 id,
			std::unordered_map<U32, Vector2>& positions
		) {
			positions.erase(id);
		}

		Vector2 queryPositionsById(
			U32 id,
			std::unordered_map<U32, Vector2>& positions
		) {
			return positions.at(id);
		}

		void insertOrUpdateTag(
			U32 id,
			std::unordered_map<U32, U32>& tags,
			U32 mask
		) {
			std::unordered_map<U32, U32>::iterator tag = tags.find(id);
			if (tag != tags.end()) {
				tag->second = mask;
			}
			else {
				tags.insert(std::make_pair(id, mask));
			}
		}

		U32 queryTagsById(
			U32 id,
			std::unordered_map<U32, U32>& tags
		) {
			std::unordered_map<U32, U32>::iterator tag = tags.find(id);
			return tag == tags.end() ? 0 : tag->second;
		}
	}
}

milk::U32 milk::SceneGraph::add(const std::string& name, Vector2 position) {
	U32 id = makeId(generations_, freeIndeces_);
	insertName(id, names_, nameidmap_, nameidxmap_, name);
	insertPosition(id, positions_, position);
	return id;
}

void milk::SceneGraph::remove(U32 id) {
	deleteId(id, generations_, freeIndeces_, destroyed_);
	deleteName(id, names_, nameidmap_, nameidxmap_);
	deletePosition(id, positions_);
}

bool milk::SceneGraph::alive(U32 id) {
	return validId(id, generations_);
}

std::string milk::SceneGraph::getName(U32 id) {
	return queryNamesById(id, names_, nameidmap_);
}

void milk::SceneGraph::setName(U32 id, const std::string& name) {
	updateName(id, names_, nameidmap_, name);
}

milk::U32 milk::SceneGraph::getByName(const std::string& name) {
	return queryIdsByName(name, names_, nameidxmap_);
}

milk::Vector2 milk::SceneGraph::getPosition(U32 id) {
	return queryPositionsById(id, positions_);
}

void milk::SceneGraph::setPosition(U32 id, Vector2 position) {
	updatePosition(id, positions_, position);
}

milk::U32 milk::SceneGraph::getTags(U32 id) {
	return queryTagsById(id, tags_);
}

void milk::SceneGraph::setTags(U32 id, U32 mask) {
	insertOrUpdateTag(id, tags_, mask);
}
