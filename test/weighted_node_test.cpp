#include "weighted_graph.hpp"
#include "graph_utils.hpp"

#include <gtest/gtest.h>

#include <string>

TEST(WeightedNodeTest, DefaultConstructor) {
    WeightedNode<int> node1;
    EXPECT_EQ(node1.getId(), WeightedNode<int>::UNUSED_ID);
    EXPECT_TRUE(node1.getAdjacents().empty());
}

TEST(WeightedNodeTest, ConstructorWithId) {
    WeightedNode<int> node2(1);
    EXPECT_EQ(node2.getId(), 1);
    EXPECT_TRUE(node2.getAdjacents().empty());
}

TEST(WeightedNodeTest, ConstructorWithIdAndAttributes) {
    WeightedNode<int> node3(2, 42);
    EXPECT_EQ(node3.getId(), 2);
    EXPECT_EQ(node3.getAttributes(), 42);
    EXPECT_TRUE(node3.getAdjacents().empty());
}

TEST(WeightedNodeTest, SetAndGetId) {
    WeightedNode<int> node1;
    node1.setId(3);
    EXPECT_EQ(node1.getId(), 3);
    node1.setId(4);
    EXPECT_EQ(node1.getId(), 4);
    node1.setId(-1);
    EXPECT_EQ(node1.getId(), 4);
}

TEST(WeightedNodeTest, IsUsed) {
    WeightedNode<int> node1;
    WeightedNode<int> node2(1);
    WeightedNode<int> node3(2, 42);
    EXPECT_FALSE(node1.isUsed());
    EXPECT_TRUE(node2.isUsed());
    EXPECT_TRUE(node3.isUsed());
}

TEST(WeightedNodeTest, SetAndGetAdjacents) {
    WeightedNode<int> node1;
    node1.setAdjacent(4, 1.5);
    node1.setAdjacent(5, 2.0);
    const auto& adjacents = node1.getAdjacents();
    EXPECT_EQ(static_cast<int>(adjacents.size()), 2);
    EXPECT_EQ(adjacents.at(4), 1.5);
    EXPECT_EQ(adjacents.at(5), 2.0);
}

TEST(WeightedNodeTest, UpdateAdjacent) {
    WeightedNode<int> node1;
    node1.setAdjacent(4, 1.5);
    node1.updateAdjacent(4, 0.5);
    node1.updateAdjacent(5, 1.5);
    const auto& adjacents = node1.getAdjacents();
    EXPECT_EQ(adjacents.at(4), 2.0);
    EXPECT_EQ(adjacents.at(5), 1.5);
}

TEST(WeightedNodeTest, RemoveAdjacent) {
    WeightedNode<int> node1;
    node1.setAdjacent(4, 1.5);
    node1.setAdjacent(5, 2.0);
    EXPECT_EQ(node1.getAdjacents().size(), 2);
    node1.removeAdjacent(5);
    const auto& adjacents = node1.getAdjacents();
    EXPECT_EQ(adjacents.size(), 1);
    EXPECT_FALSE(adjacents.contains(5));
}

TEST(WeightedNodeTest, SetAndGetAttributes) {
    WeightedNode<int> node1;
    node1.setAttributes(10);
    EXPECT_EQ(node1.getAttributes(), 10);
    WeightedNode<std::string> node2;
    node2.setAttributes("hoge");
    EXPECT_EQ(node2.getAttributes(), "hoge");
}

TEST(WeightedNodeTest, GetAttributesPtr) {
    WeightedNode<int> node1;
    node1.setAttributes(10);
    auto attributesPtr = node1.getAttributesPtr();
    EXPECT_NE(attributesPtr, nullptr);
    EXPECT_EQ(*attributesPtr, 10);
    *attributesPtr = 42;
    EXPECT_EQ(node1.getAttributes(), 42);
}

TEST(WeightedNodeTest, Clear) {
    WeightedNode<int> node1;
    node1.setAdjacent(4, 1.5);
    node1.setAttributes(10);
    node1.clear();
    EXPECT_FALSE(node1.isUsed());
    EXPECT_EQ(node1.getId(), WeightedNode<int>::UNUSED_ID);
    EXPECT_TRUE(node1.getAdjacents().empty());
    EXPECT_EQ(node1.getAttributesPtr(), nullptr);
}