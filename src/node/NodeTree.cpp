#include "NodeTree.h"

#include <algorithm>
#include "node_id.h"

namespace milk {
	milk::NodeTree::NodeTree()
		: ID_INDEX_BITS{ 16 }
		, ID_GENERATION_BITS{ 16 }
		, MAX_NODES{ 2 ^ ID_INDEX_BITS }
		, MAX_FREE_INDECES{ 1024 } {

		m_invalidNode = node_id::make(m_ids, ID_GENERATION_BITS, MAX_FREE_INDECES);
		m_rootNode = node_id::make(m_ids, ID_GENERATION_BITS, MAX_FREE_INDECES);
		m_globalNode = node_id::make(m_ids, ID_GENERATION_BITS, MAX_FREE_INDECES);
		m_children.insert(std::make_pair(m_rootNode, std::vector<U32>{}));
		m_children.insert(std::make_pair(m_globalNode, std::vector<U32>{}));
		m_names.insert(std::make_pair(m_rootNode, "root"));
		m_names.insert(std::make_pair(m_globalNode, "global"));
		m_positions.insert(std::make_pair(m_rootNode, Vector2::zero()));
		m_positions.insert(std::make_pair(m_globalNode, Vector2::zero()));

		m_parents.insert(std::make_pair(m_globalNode, m_rootNode));
		m_children.at(m_rootNode).push_back(m_globalNode);
	}

	milk::U32 milk::NodeTree::root() const {
		return m_rootNode;
	}

	milk::U32 milk::NodeTree::global() const {
		return m_globalNode;
	}

	milk::U32 milk::NodeTree::add(U32 parentNode, const std::string& name, Vector2 position) {
		U32 node = node_id::make(m_ids, ID_GENERATION_BITS, MAX_FREE_INDECES);
		m_parents.insert(std::make_pair(node, parentNode));
		m_children.at(parentNode).push_back(node);
		m_children.insert(std::make_pair(node, std::vector<U32>{}));
		m_names.insert(std::make_pair(node, name));
		m_positions.insert(std::make_pair(node, position));
		return node;
	}

	void milk::NodeTree::remove(U32 node) {
		std::vector<U32> children = m_children.at(node);
		for (int i = 0; i < children.size(); ++i)
			remove(children.at(i));

		U32 parent = m_parents.at(node);
		std::vector<U32>& parentChildren = m_children.at(parent);
		parentChildren.erase(std::find(parentChildren.begin(), parentChildren.end(), node));

		node_id::recycle(m_ids, ID_GENERATION_BITS, node);
		m_parents.erase(node);
		m_children.erase(node);
		m_names.erase(node);
		m_positions.erase(node);
	}

	bool milk::NodeTree::isAlive(U32 node) {
		return node != m_invalidNode && node_id::valid(m_ids, ID_GENERATION_BITS, ID_INDEX_BITS, node);
	}

	void milk::NodeTree::setParent(U32 node, U32 parentNode) {
		if (node == m_invalidNode || node == m_rootNode || node == m_globalNode)
			return;

		U32 parent = m_parents.at(node);
		std::vector<U32>& parentChildren = m_children.at(parent);
		parentChildren.erase(std::find(parentChildren.begin(), parentChildren.end(), node));

		m_parents.at(node) = parentNode;
		m_children.at(parentNode).push_back(node);
	}

	milk::U32 milk::NodeTree::getParent(U32 node) {
		return node == m_rootNode ? m_invalidNode : m_parents.at(node);
	}

	std::vector<milk::U32> milk::NodeTree::getChildren(U32 node) {
		return m_children.at(node);
	}

	std::string milk::NodeTree::getName(U32 node) {
		return m_names.at(node);
	}

	void milk::NodeTree::setName(U32 node, const std::string& name) {
		if (node == m_invalidNode || node == m_rootNode || node == m_globalNode)
			return;

		m_names.at(node) = name;
	}

	milk::Vector2 milk::NodeTree::getPosition(U32 node) {
		return m_positions.at(node);
	}

	void milk::NodeTree::setPosition(U32 node, Vector2 position) {
		if (node == m_invalidNode)
			return;

		m_positions.at(node) = position;
	}
}
