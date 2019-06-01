//#ifndef MILK_ACTORS_TESTS_H
//#define MILK_ACTORS_TESTS_H
//
//#include "gtest/gtest.h"
//#include "scene/RootNode.h"
//
//namespace milk {
//	TEST(SceneGraphTests, Creation) {
//		RootNode scene;
//
//		U32 actor0 = scene.add("hail satan", Vector2{ 0.f, 12.f });
//		U32 actor1 = scene.add("stev", Vector2{ 15.5f, 98.f });
//		U32 actor2 = scene.add("danny devito", Vector2{ 0.f, -1.f });
//
//		EXPECT_NE(RootNode::INVALID_NODE_ID, actor0);
//		EXPECT_NE(RootNode::INVALID_NODE_ID, actor1);
//		EXPECT_NE(RootNode::INVALID_NODE_ID, actor2);
//
//		EXPECT_NE(actor0, actor1);
//		EXPECT_NE(actor0, actor2);
//		EXPECT_NE(actor1, actor2);
//
//		EXPECT_EQ("hail satan", scene.getName(actor0));
//		EXPECT_EQ("stev", scene.getName(actor1));
//		EXPECT_EQ("danny devito", scene.getName(actor2));
//
//		EXPECT_EQ(Vector2(0.f, 12.f), scene.getPosition(actor0));
//		EXPECT_EQ(Vector2(15.5f, 98.f), scene.getPosition(actor1));
//		EXPECT_EQ(Vector2(0.f, -1.f), scene.getPosition(actor2));
//	}
//
//	TEST(SceneGraphTests, Destruction) {
//		RootNode scene;
//
//		U32 actor0 = scene.add("hail satan", Vector2{ 0.f, 12.f });
//		U32 actor1 = scene.add("stev", Vector2{ 15.5f, 98.f });
//		U32 actor2 = scene.add("danny devito", Vector2{ 0.f, -1.f });
//
//		EXPECT_TRUE(scene.alive(actor0));
//		EXPECT_TRUE(scene.alive(actor1));
//		EXPECT_TRUE(scene.alive(actor2));
//
//		scene.remove(actor0);
//		scene.remove(actor1);
//		scene.remove(actor2);
//
//		EXPECT_FALSE(scene.alive(actor0));
//		EXPECT_FALSE(scene.alive(actor1));
//		EXPECT_FALSE(scene.alive(actor2));
//	}
//
//	TEST(SceneGraphTests, Position) {
//		RootNode scene;
//
//		U32 actor0 = scene.add("hail satan", Vector2{ 0.f, 12.f });
//		U32 actor1 = scene.add("stev", Vector2{ 15.5f, 98.f });
//		U32 actor2 = scene.add("danny devito", Vector2{ 0.f, -1.f });
//
//		scene.setPosition(actor0, Vector2::zero());
//		scene.setPosition(actor1, Vector2{ 1.f, 1.f });
//		scene.setPosition(actor2, Vector2{ -10.f, 20.f });
//
//		EXPECT_EQ(Vector2::zero(), scene.getPosition(actor0));
//		EXPECT_EQ(Vector2(1.f, 1.f), scene.getPosition(actor1));
//		EXPECT_EQ(Vector2(-10.f, 20.f), scene.getPosition(actor2));
//	}
//
//	TEST(SceneGraphTests, Name) {
//		RootNode scene;
//
//		U32 actor0 = scene.add("hail satan", Vector2{ 0.f, 12.f });
//		U32 actor1 = scene.add("stev", Vector2{ 15.5f, 98.f });
//		U32 actor2 = scene.add("danny devito", Vector2{ 0.f, -1.f });
//
//		scene.setName(actor0, "bobody");
//		scene.setName(actor1, "burt macklin");
//		scene.setName(actor2, "day man");
//
//		EXPECT_EQ("bobody", scene.getName(actor0));
//		EXPECT_EQ("burt macklin", scene.getName(actor1));
//		EXPECT_EQ("day man", scene.getName(actor2));
//
//		U32 foundActor0 = scene.getByName("bobody");
//		U32 foundActor1 = scene.getByName("burt macklin");
//		U32 foundActor2 = scene.getByName("day man");
//
//		EXPECT_EQ(actor0, foundActor0);
//		EXPECT_EQ(actor1, foundActor1);
//		EXPECT_EQ(actor2, foundActor2);
//	}
//
//	TEST(SceneGraphTests, Tags) {
//		RootNode scene;
//
//		U32 actor0 = scene.add("hail satan", Vector2{ 0.f, 12.f });
//		U32 actor1 = scene.add("stev", Vector2{ 15.5f, 98.f });
//		U32 actor2 = scene.add("danny devito", Vector2{ 0.f, -1.f });
//
//		U32 playerTag = 1 << 0;
//		U32 enemyTag = 1 << 1;
//		U32 interactabletag = 1 << 2;
//
//		scene.setTags(actor0, playerTag);
//		scene.setTags(actor1, enemyTag | interactabletag);
//
//		EXPECT_EQ(playerTag, scene.getTags(actor0));
//		EXPECT_EQ(enemyTag | interactabletag, scene.getTags(actor1));
//		EXPECT_EQ(0, scene.getTags(actor2));
//	}
//}
//
//#endif#ifndef MILK_ACTORS_TESTS_H
//#define MILK_ACTORS_TESTS_H
//
//#include "gtest/gtest.h"
//#include "scene/RootNode.h"
//
//namespace milk {
//	TEST(SceneGraphTests, Creation) {
//		RootNode scene;
//
//		U32 actor0 = scene.add("hail satan", Vector2{ 0.f, 12.f });
//		U32 actor1 = scene.add("stev", Vector2{ 15.5f, 98.f });
//		U32 actor2 = scene.add("danny devito", Vector2{ 0.f, -1.f });
//
//		EXPECT_NE(RootNode::INVALID_NODE_ID, actor0);
//		EXPECT_NE(RootNode::INVALID_NODE_ID, actor1);
//		EXPECT_NE(RootNode::INVALID_NODE_ID, actor2);
//
//		EXPECT_NE(actor0, actor1);
//		EXPECT_NE(actor0, actor2);
//		EXPECT_NE(actor1, actor2);
//
//		EXPECT_EQ("hail satan", scene.getName(actor0));
//		EXPECT_EQ("stev", scene.getName(actor1));
//		EXPECT_EQ("danny devito", scene.getName(actor2));
//
//		EXPECT_EQ(Vector2(0.f, 12.f), scene.getPosition(actor0));
//		EXPECT_EQ(Vector2(15.5f, 98.f), scene.getPosition(actor1));
//		EXPECT_EQ(Vector2(0.f, -1.f), scene.getPosition(actor2));
//	}
//
//	TEST(SceneGraphTests, Destruction) {
//		RootNode scene;
//
//		U32 actor0 = scene.add("hail satan", Vector2{ 0.f, 12.f });
//		U32 actor1 = scene.add("stev", Vector2{ 15.5f, 98.f });
//		U32 actor2 = scene.add("danny devito", Vector2{ 0.f, -1.f });
//
//		EXPECT_TRUE(scene.alive(actor0));
//		EXPECT_TRUE(scene.alive(actor1));
//		EXPECT_TRUE(scene.alive(actor2));
//
//		scene.remove(actor0);
//		scene.remove(actor1);
//		scene.remove(actor2);
//
//		EXPECT_FALSE(scene.alive(actor0));
//		EXPECT_FALSE(scene.alive(actor1));
//		EXPECT_FALSE(scene.alive(actor2));
//	}
//
//	TEST(SceneGraphTests, Position) {
//		RootNode scene;
//
//		U32 actor0 = scene.add("hail satan", Vector2{ 0.f, 12.f });
//		U32 actor1 = scene.add("stev", Vector2{ 15.5f, 98.f });
//		U32 actor2 = scene.add("danny devito", Vector2{ 0.f, -1.f });
//
//		scene.setPosition(actor0, Vector2::zero());
//		scene.setPosition(actor1, Vector2{ 1.f, 1.f });
//		scene.setPosition(actor2, Vector2{ -10.f, 20.f });
//
//		EXPECT_EQ(Vector2::zero(), scene.getPosition(actor0));
//		EXPECT_EQ(Vector2(1.f, 1.f), scene.getPosition(actor1));
//		EXPECT_EQ(Vector2(-10.f, 20.f), scene.getPosition(actor2));
//	}
//
//	TEST(SceneGraphTests, Name) {
//		RootNode scene;
//
//		U32 actor0 = scene.add("hail satan", Vector2{ 0.f, 12.f });
//		U32 actor1 = scene.add("stev", Vector2{ 15.5f, 98.f });
//		U32 actor2 = scene.add("danny devito", Vector2{ 0.f, -1.f });
//
//		scene.setName(actor0, "bobody");
//		scene.setName(actor1, "burt macklin");
//		scene.setName(actor2, "day man");
//
//		EXPECT_EQ("bobody", scene.getName(actor0));
//		EXPECT_EQ("burt macklin", scene.getName(actor1));
//		EXPECT_EQ("day man", scene.getName(actor2));
//
//		U32 foundActor0 = scene.getByName("bobody");
//		U32 foundActor1 = scene.getByName("burt macklin");
//		U32 foundActor2 = scene.getByName("day man");
//
//		EXPECT_EQ(actor0, foundActor0);
//		EXPECT_EQ(actor1, foundActor1);
//		EXPECT_EQ(actor2, foundActor2);
//	}
//
//	TEST(SceneGraphTests, Tags) {
//		RootNode scene;
//
//		U32 actor0 = scene.add("hail satan", Vector2{ 0.f, 12.f });
//		U32 actor1 = scene.add("stev", Vector2{ 15.5f, 98.f });
//		U32 actor2 = scene.add("danny devito", Vector2{ 0.f, -1.f });
//
//		U32 playerTag = 1 << 0;
//		U32 enemyTag = 1 << 1;
//		U32 interactabletag = 1 << 2;
//
//		scene.setTags(actor0, playerTag);
//		scene.setTags(actor1, enemyTag | interactabletag);
//
//		EXPECT_EQ(playerTag, scene.getTags(actor0));
//		EXPECT_EQ(enemyTag | interactabletag, scene.getTags(actor1));
//		EXPECT_EQ(0, scene.getTags(actor2));
//	}
//}
//
//#endif