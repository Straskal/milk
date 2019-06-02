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

		U32 root() const;
		U32 global() const;

		U32 add(U32 parentNode, const std::string& name, Vector2 position);
		void remove(U32 node);
		bool isAlive(U32 node) const;
		void setParent(U32 node, U32 parentNode);
		U32 getParent(U32 node) const;
		std::vector<U32> getChildren(U32 node) const;
		std::string getName(U32 node) const;
		void setName(U32 node, const std::string& name);
		Vector2 getPosition(U32 node) const;
		void setPosition(U32 node, Vector2 position);

	private:
		const U32 ID_INDEX_BITS;
		const U32 ID_GENERATION_BITS;
		const U32 MAX_NODES;
		const int MAX_FREE_INDECES;

		U32 m_invalidNode;
		U32 m_rootNode;
		U32 m_globalNode;

		NodeIdData m_ids;
		std::unordered_map<U32, U32> m_parents;
		std::unordered_map<U32, std::vector<U32>> m_children;
		std::unordered_map<U32, std::string> m_names;
		std::unordered_map<U32, Vector2> m_positions;
	};
}

#endif