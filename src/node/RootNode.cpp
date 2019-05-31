#include "RootNode.h"

#include "id.h"

namespace milk {
	const U32 RootNode::ID_INDEX_BITS = 16;
	const U32 RootNode::ID_GENERATION_BITS = 16;
	const U32 RootNode::ROOT_NODE_ID = 1;
	const U32 RootNode::INVALID_NODE_ID = 0;
	const int RootNode::MAX_NODES = 2 ^ ID_INDEX_BITS;
	const int RootNode::MAX_FREE_INDECES = 1024;

	namespace {
		void insertName(
			std::vector<std::string>& names,
			std::unordered_map<U32, int>& nameidmap,
			std::unordered_map<int, U32>& nameidxmap,
			U32 id,
			const std::string& name
		) {
			names.push_back(name);
			int nameidx = names.size() - 1;
			nameidmap.insert(std::make_pair(id, nameidx));
			nameidxmap.insert(std::make_pair(nameidx, id));
		}

		void deleteName(
			std::vector<std::string>& names,
			std::unordered_map<U32, int>& nameidmap,
			std::unordered_map<int, U32>& nameidxmap,
			U32 id
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
			std::vector<std::string>& names,
			std::unordered_map<U32, int>& nameidmap,
			U32 id,
			const std::string& name
		) {
			int nameidx = nameidmap.at(id);
			names[nameidx] = name;
		}

		std::string queryNamesById(
			std::vector<std::string>& names,
			std::unordered_map<U32, int>& nameidmap,
			U32 id
		) {
			int nameidx = nameidmap.at(id);
			return names[nameidx];
		}

		U32 queryIdsByName(
			std::vector<std::string>& names,
			std::unordered_map<int, U32>& nameidxmap,
			const std::string& name
		) {
			for (int i = 0; i < names.size(); ++i) {
				if (name == names[i]) {
					return nameidxmap.at(i);
				}
			}
			return RootNode::INVALID_NODE_ID;
		}

		void insertPosition(
			std::unordered_map<U32, Vector2>& positions,
			U32 id,
			Vector2 position
		) {
			positions.insert(std::make_pair(id, position));
		}

		void updatePosition(
			std::unordered_map<U32, Vector2>& positions,
			U32 id,
			Vector2 position
		) {
			positions.at(id) = position;
		}

		void deletePosition(
			std::unordered_map<U32, Vector2>& positions,
			U32 id
		) {
			positions.erase(id);
		}

		Vector2 queryPositionsById(
			std::unordered_map<U32, Vector2>& positions,
			U32 id
		) {
			return positions.at(id);
		}

		void insertOrUpdateTag(
			std::unordered_map<U32, U32>& tags,
			U32 id,
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
			std::unordered_map<U32, U32>& tags,
			U32 id
		) {
			std::unordered_map<U32, U32>::iterator tag = tags.find(id);
			return tag == tags.end() ? 0 : tag->second;
		}
	}
}

milk::U32 milk::RootNode::add(const std::string& name, Vector2 position) {
	U32 id = node_id::make(generations_, freeIndeces_, INVALID_NODE_ID, ID_GENERATION_BITS, MAX_FREE_INDECES);
	insertName(names_, nameidmap_, nameidxmap_, id, name);
	insertPosition(positions_, id, position);
	return id;
}

void milk::RootNode::remove(U32 id) {
	node_id::recycle(generations_, freeIndeces_, ID_GENERATION_BITS, id);
	deleteName(names_, nameidmap_, nameidxmap_, id);
	deletePosition(positions_, id);
}

bool milk::RootNode::alive(U32 id) {
	return node_id::valid(generations_, ID_GENERATION_BITS, ID_INDEX_BITS, id);
}

std::string milk::RootNode::getName(U32 id) {
	return queryNamesById(names_, nameidmap_, id);
}

void milk::RootNode::setName(U32 id, const std::string& name) {
	updateName(names_, nameidmap_, id, name);
}

milk::U32 milk::RootNode::getByName(const std::string& name) {
	return queryIdsByName(names_, nameidxmap_, name);
}

milk::Vector2 milk::RootNode::getPosition(U32 id) {
	return queryPositionsById(positions_, id);
}

void milk::RootNode::setPosition(U32 id, Vector2 position) {
	updatePosition(positions_, id, position);
}

milk::U32 milk::RootNode::getTags(U32 id) {
	return queryTagsById(tags_, id);
}

void milk::RootNode::setTags(U32 id, U32 mask) {
	insertOrUpdateTag(tags_, id, mask);
}
