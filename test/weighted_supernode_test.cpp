#include "supergraph.hpp"

#include <gtest/gtest.h>

#include <cassert>

TEST(WeightedSupernodeTest, DefaultConstructor) {
    WeightedSuperNode node1;
    ASSERT_EQ(node1.getId(), WeightedSuperNode::UNUSED_ID);
    ASSERT_FALSE(node1.isUsed());
    ASSERT_TRUE(node1.isRoot());
    ASSERT_TRUE(node1.isLeaf());
    ASSERT_EQ(node1.getParent(), WeightedSuperNode::ROOT);
    ASSERT_TRUE(node1.getChildren().empty());
    ASSERT_TRUE(node1.getAdjacents().empty());
}

TEST(WeightedSupernodeTest, ConstructorWithId) {
    WeightedSuperNode node2(1);
    ASSERT_EQ(node2.getId(), 1);
    ASSERT_TRUE(node2.isUsed());
    ASSERT_TRUE(node2.isRoot());
    ASSERT_TRUE(node2.isLeaf());
    ASSERT_EQ(node2.getParent(), WeightedSuperNode::ROOT);
    ASSERT_TRUE(node2.getChildren().empty());
    ASSERT_TRUE(node2.getAdjacents().empty());
}

TEST(WeightedSupernodeTest, SetAndGetId) {
    WeightedSuperNode node1;
    node1.setId(2);
    ASSERT_EQ(node1.getId(), 2);
}

TEST(WeightedSupernodeTest, SetAndGetParent) {
    WeightedSuperNode node1;
    node1.setParent(1);
    ASSERT_EQ(node1.getParent(), 1);
}

TEST(WeightedSupernodeTest, AddAndGetChildren) {
    WeightedSuperNode node1;
    node1.addChild(3);
    node1.addChild(4);
    ASSERT_EQ(node1.getChildren().size(), 2);
    ASSERT_EQ(node1.getChildren().count(3), 1);
    ASSERT_EQ(node1.getChildren().count(4), 1);
}

TEST(WeightedSupernodeTest, SetAndGetAdjacent) {
    WeightedSuperNode node1;
    node1.setAdjacent(5, 1.5);
    ASSERT_EQ(node1.getAdjacents().size(), 1);
    ASSERT_EQ(node1.getAdjacents().count(5), 1);
    ASSERT_EQ(node1.getAdjacents().at(5), 1.5);
}

TEST(WeightedSupernodeTest, UpdateAdjacent) {
    WeightedSuperNode node1;
    node1.setAdjacent(5, 1.5);
    ASSERT_EQ(node1.getAdjacents().at(5), 1.5);
    node1.updateAdjacent(5, 2.5);
    ASSERT_EQ(node1.getAdjacents().at(5), 2.5);
}

TEST(WeightedSupernodeTest, RemoveAdjacent) {
    WeightedSuperNode node1;
    node1.setAdjacent(5, 1.5);
    ASSERT_EQ(node1.getAdjacents().at(5), 1.5);
    node1.removeAdjacent(5);
    ASSERT_TRUE(node1.getAdjacents().empty());
}

TEST(WeightedSupernodeTest, Clear) {
    WeightedSuperNode node1;
    node1.setId(2);
    node1.setParent(1);
    node1.addChild(3);
    node1.setAdjacent(5, 1.5);
    node1.clear();
    ASSERT_EQ(node1.getId(), WeightedSuperNode::UNUSED_ID);
    ASSERT_FALSE(node1.isUsed());
    ASSERT_FALSE(node1.isRoot());
    ASSERT_TRUE(node1.isLeaf());
    ASSERT_EQ(node1.getParent(), WeightedSuperNode::ROOT);
    ASSERT_TRUE(node1.getChildren().empty());
    ASSERT_TRUE(node1.getAdjacents().empty());
}
