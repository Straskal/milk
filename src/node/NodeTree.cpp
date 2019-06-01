#include "NodeTree.h"

#include <algorithm>

#include "node_id.h"

namespace milk {
	namespace {
		void insertParent(
			std::unordered_map<U32, U32>& parentidmap,
			U32 id,
			U32 parentid
		) {
			parentidmap.insert(std::make_pair(id, parentid));
		}

		void updateParent(
			std::unordered_map<U32, U32>& parentidmap,
			U32 id,
			U32 parentid
		) {
			parentidmap.at(id) = parentid;
		}

		void deleteParent(
			std::unordered_map<U32, U32>& parentidmap,
			U32 id
		) {
			parentidmap.erase(id);
		}

		U32 queryParentById(
			std::unordered_map<U32, U32>& parents,
			U32 id
		) {
			return parents.at(id);
		}

		void insertChildren(
			std::unordered_map<U32, std::vector<U32>>& childrenidmap,
			U32 id
		) {
			childrenidmap.insert(std::make_pair(id, std::vector<U32>{}));
		}

		void deleteChildren(
			std::unordered_map<U32, std::vector<U32>>& childrenidmap,
			U32 id
		) {
			childrenidmap.erase(id);
		}

		void insertChild(
			std::unordered_map<U32, std::vector<U32>>& childrenidmap,
			U32 id,
			U32 childid
		) {
			childrenidmap.at(id).push_back(childid);
		}

		void deleteChild(
			std::unordered_map<U32, std::vector<U32>>& childrenidmap,
			U32 id,
			U32 childid
		) {
			auto& children = childrenidmap.at(id);
			auto childitr = std::find(children.begin(), children.end(), childid);
			children.erase(childitr);
		}

		std::vector<U32> queryChildrenById(
			std::unordered_map<U32, std::vector<U32>>& childrenidmap,
			U32 id
		) {
			return childrenidmap.at(id);
		}

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
			const U32 INVALID,
			const std::string& name
		) {
			for (int i = 0; i < names.size(); ++i) {
				if (name == names[i]) {
					return nameidxmap.at(i);
				}
			}
			return INVALID;
		}

		void insertPosition(
			std::unordered_map<U32, Vector2>& positionidmap,
			U32 id,
			Vector2 position
		) {
			positionidmap.insert(std::make_pair(id, position));
		}

		void updatePosition(
			std::unordered_map<U32, Vector2>& positionidmap,
			U32 id,
			Vector2 position
		) {
			positionidmap.at(id) = position;
		}

		void deletePosition(
			std::unordered_map<U32, Vector2>& positionidmap,
			U32 id
		) {
			positionidmap.erase(id);
		}

		Vector2 queryPositionsById(
			std::unordered_map<U32, Vector2>& positions,
			U32 id
		) {
			auto foundpos = positions.find(id);
			return foundpos != positions.end() ? foundpos->second : Vector2::zero();
		}

		void insertUpdateTag(
			std::unordered_map<U32, U32>& tags,
			U32 id,
			U32 mask
		) {
			auto foundtag = tags.find(id);
			if (foundtag != tags.end()) {
				foundtag->second = mask;
				return;
			}
			tags.insert(std::make_pair(id, mask));
		}

		U32 queryTagsById(
			std::unordered_map<U32, U32>& tags,
			U32 id
		) {
			auto foundtag = tags.find(id);
			return foundtag == tags.end() ? 0 : foundtag->second;
		}

		void deleteTags(
			std::unordered_map<U32, U32>& tags,
			U32 id
		) {
			auto foundtag = tags.find(id);
			if (foundtag != tags.end()) {
				tags.erase(id);
			}
		}
	}
}

milk::NodeTree::NodeTree()
	: ID_INDEX_BITS{ 16 }
	, ID_GENERATION_BITS{ 16 }
	, INVALID_NODE_ID{ node_id::make(generations_, freeIndeces_, ID_GENERATION_BITS, MAX_FREE_INDECES) }
	, ROOT_NODE_ID{ node_id::make(generations_, freeIndeces_, ID_GENERATION_BITS, MAX_FREE_INDECES) }
	, GLOBAL_NODE_ID{ node_id::make(generations_, freeIndeces_, ID_GENERATION_BITS, MAX_FREE_INDECES) }
	, MAX_NODES{ 2 ^ ID_INDEX_BITS }
	, MAX_FREE_INDECES{ 1024 } {
	
	insertName(names_, nameidmap_, nameidxmap_, ROOT_NODE_ID, "root");
	insertName(names_, nameidmap_, nameidxmap_, GLOBAL_NODE_ID, "global");
	insertPosition(positionidmap_, ROOT_NODE_ID, Vector2::zero());
	insertPosition(positionidmap_, GLOBAL_NODE_ID, Vector2::zero());
	insertChildren(childrenidmap_, ROOT_NODE_ID);
	insertChildren(childrenidmap_, GLOBAL_NODE_ID);
	insertParent(parentidmap_, GLOBAL_NODE_ID, ROOT_NODE_ID);
	insertChild(childrenidmap_, ROOT_NODE_ID, GLOBAL_NODE_ID);
}

milk::U32 milk::NodeTree::add(U32 parentid, const std::string& name) {
	U32 id = node_id::make(generations_, freeIndeces_, ID_GENERATION_BITS, MAX_FREE_INDECES);
	insertName(names_, nameidmap_, nameidxmap_, id, name);
	insertPosition(positionidmap_, id, Vector2::zero());
	insertParent(parentidmap_, id, parentid);
	insertChildren(childrenidmap_, id);
	insertChild(childrenidmap_, parentid, id);
	return id;
}

void milk::NodeTree::remove(U32 id) {
	node_id::recycle(generations_, freeIndeces_, ID_GENERATION_BITS, id);
	deleteName(names_, nameidmap_, nameidxmap_, id);
	deletePosition(positionidmap_, id);
}

bool milk::NodeTree::alive(U32 id) {
	return node_id::valid(generations_, ID_GENERATION_BITS, ID_INDEX_BITS, id);
}

void milk::NodeTree::setParent(U32 id, U32 parentid) {
	U32 currentparent = getParent(id);
	if (currentparent == parentid) {
		return;
	}
	updateParent(parentidmap_, id, parentid);
	insertChild(childrenidmap_, parentid, id);
	deleteChild(childrenidmap_, currentparent, id);
}

milk::U32 milk::NodeTree::getParent(U32 id) {
	return queryParentById(parentidmap_, id);
}

std::vector<milk::U32> milk::NodeTree::getChildren(U32 id) {
	return queryChildrenById(childrenidmap_, id);
}

std::string milk::NodeTree::getName(U32 id) {
	return queryNamesById(names_, nameidmap_, id);
}

void milk::NodeTree::setName(U32 id, const std::string& name) {
	updateName(names_, nameidmap_, id, name);
}

milk::U32 milk::NodeTree::getByName(const std::string& name) {
	return queryIdsByName(names_, nameidxmap_, INVALID_NODE_ID, name);
}

milk::Vector2 milk::NodeTree::getPosition(U32 id) {
	return queryPositionsById(positionidmap_, id);
}

void milk::NodeTree::setPosition(U32 id, Vector2 position) {
	updatePosition(positionidmap_, id, position);
}

milk::U32 milk::NodeTree::getTags(U32 id) {
	return queryTagsById(tagidmap_, id);
}

void milk::NodeTree::setTags(U32 id, U32 mask) {
	insertUpdateTag(tagidmap_, id, mask);
}
