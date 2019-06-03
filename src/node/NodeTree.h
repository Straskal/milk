#ifndef MILK_NODE_TREE_H
#define MILK_NODE_TREE_H

#include <string>
#include <unordered_map>
#include <vector>

#include "node_id.h"
#include "data/int.h"
#include "math/Vector2.h"

namespace milk {
	class NodeTree {
	public:
		NodeTree();

		u32 root() const;
		u32 global() const;

		u32 add(u32 parentNode, const std::string& name, Vector2 position);
		void remove(u32 node);
		bool isAlive(u32 node) const;
		void setParent(u32 node, u32 parentNode);
		u32 getParent(u32 node) const;
		std::vector<u32> getChildren(u32 node) const;
		std::string getName(u32 node) const;
		void setName(u32 node, const std::string& name);
		Vector2 getPosition(u32 node) const;
		void setPosition(u32 node, Vector2 position);

	private:
		const u32 ID_INDEX_BITS;
		const u32 ID_GENERATION_BITS;
		const u32 MAX_NODES;
		const int MAX_FREE_INDECES;

		u32 m_invalidNode;
		u32 m_rootNode;
		u32 m_globalNode;

		NodeIdData m_ids;
		std::unordered_map<u32, u32> m_parents;
		std::unordered_map<u32, std::vector<u32>> m_children;
		std::unordered_map<u32, std::string> m_names;
		std::unordered_map<u32, Vector2> m_positions;
	};
}

#endif