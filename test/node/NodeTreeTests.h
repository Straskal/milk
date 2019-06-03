#ifndef MILK_ACTORS_TESTS_H
#define MILK_ACTORS_TESTS_H

#include <algorithm>

#include "gtest/gtest.h"
#include "node/NodeTree.h"

namespace milk {
	TEST(NodeTreeTests, Constructor_InitializesRootNode) {
		NodeTree nt;
		u32 root = nt.root();

		ASSERT_TRUE(nt.isAlive(root));
		ASSERT_EQ("root", nt.getName(root));
		ASSERT_EQ(Vector2::zero(), nt.getPosition(root));
	}

	TEST(NodeTreeTests, Constructor_InitializesGlobalNode) {
		NodeTree nt;
		u32 global = nt.global();

		ASSERT_TRUE(nt.isAlive(global));
		ASSERT_EQ("global", nt.getName(global));
		ASSERT_EQ(Vector2::zero(), nt.getPosition(global));
	}

	TEST(NodeTreeTests, Add_AddsNode) {
		NodeTree nt;
		u32 root = nt.root();
		u32 node = nt.add(root, "stv", Vector2(15.f, -100.f));

		ASSERT_TRUE(nt.isAlive(node));
		ASSERT_EQ(root, nt.getParent(node));
		ASSERT_EQ("stv", nt.getName(node));
		ASSERT_EQ(Vector2(15.f, -100.f), nt.getPosition(node));

		std::vector<u32> rootChildren = nt.getChildren(root);
		ASSERT_NE(rootChildren.end(), std::find(rootChildren.begin(), rootChildren.end(), node));
	}

	TEST(NodeTreeTests, Remove_CannotRemoveRootNode) {
		NodeTree nt;
		u32 root = nt.root();
		nt.remove(root);

		ASSERT_TRUE(nt.isAlive(root));
	}

	TEST(NodeTreeTests, Remove_CannotRemoveGlobalNode) {
		NodeTree nt;
		u32 global = nt.global();
		nt.remove(global);

		ASSERT_TRUE(nt.isAlive(global));
	}

	TEST(NodeTreeTests, Remove_RemovesNode) {
		NodeTree nt;
		u32 root = nt.root();
		u32 node = nt.add(root, "stv", Vector2(15.f, -100.f));
		u32 node1 = nt.add(node, "stvchild", Vector2(15.f, -100.f));
		u32 node2 = nt.add(node1, "stvgrandchild", Vector2(15.f, -100.f));
		nt.remove(node);

		ASSERT_FALSE(nt.isAlive(node));
		ASSERT_FALSE(nt.isAlive(node1));
		ASSERT_FALSE(nt.isAlive(node2));

		std::vector<u32> rootChildren = nt.getChildren(root);
		ASSERT_EQ(rootChildren.end(), std::find(rootChildren.begin(), rootChildren.end(), node));
	}

	TEST(NodeTreeTests, GetParent_RootNode_ReturnsInvalid) {
		NodeTree nt;
		u32 root = nt.root();
		u32 parent = nt.getParent(root);

		ASSERT_EQ(0, parent);
	}

	TEST(NodeTreeTests, SetParent_CannotSetRootParent) {
		NodeTree nt;
		u32 root = nt.root();
		nt.setParent(root, 7);

		ASSERT_EQ(0, nt.getParent(root));
	}

	TEST(NodeTreeTests, SetParent_CannotSetGlobalParent) {
		NodeTree nt;
		u32 root = nt.root();
		u32 global = nt.global();
		nt.setParent(global, 7);

		ASSERT_EQ(root, nt.getParent(global));
	}

	TEST(NodeTreeTests, SetParent_SetsParentAndParentsChild) {
		NodeTree nt;
		u32 root = nt.root();
		u32 global = nt.global();
		u32 node = nt.add(root, "stv", Vector2(15.f, -100.f));
		nt.setParent(node, global);

		std::vector<u32> rootChildren = nt.getChildren(root);
		ASSERT_EQ(rootChildren.end(), std::find(rootChildren.begin(), rootChildren.end(), node));
		ASSERT_EQ(global, nt.getParent(node));
		std::vector<u32> globalChildren = nt.getChildren(global);
		ASSERT_NE(globalChildren.end(), std::find(globalChildren.begin(), globalChildren.end(), node));
	}

	TEST(NodeTreeTests, SetName_CannotSetRootName) {
		NodeTree nt;
		u32 root = nt.root();
		nt.setName(root, "butt");

		ASSERT_EQ("root", nt.getName(root));
	}

	TEST(NodeTreeTests, SetName_CannotSetGlobalName) {
		NodeTree nt;
		u32 global = nt.global();
		nt.setName(global, "butt");

		ASSERT_EQ("global", nt.getName(global));
	}

	TEST(NodeTreeTests, SetName_SetsName) {
		NodeTree nt;
		u32 root = nt.root();
		u32 node = nt.add(root, "stv", Vector2(15.f, -100.f));
		nt.setName(node, "butt");

		ASSERT_EQ("butt", nt.getName(node));
	}

	TEST(NodeTreeTests, SetPosition_SetsPosition) {
		NodeTree nt;
		u32 root = nt.root();
		nt.setPosition(root, Vector2(100.f, 100.f));

		ASSERT_EQ(Vector2(100.f, 100.f), nt.getPosition(root));
	}
}

#endif