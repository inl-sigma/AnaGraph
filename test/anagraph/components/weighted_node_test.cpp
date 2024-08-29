#include "anagraph/components/weighted_graph.hpp"

#include <gtest/gtest.h>

#include <string>

TEST(WeightedNodeTest, DefaultConstructor) {
    using namespace anagraph;
    graph::WeightedNode node1;
    EXPECT_EQ(node1.getId(), graph::WeightedNode::UNUSED_ID);
    EXPECT_TRUE(node1.getAdjacents().empty());
}

TEST(WeightedNodeTest, ConstructorWithId) {
    using namespace anagraph;
    graph::WeightedNode node2(1);
    EXPECT_EQ(node2.getId(), 1);
    EXPECT_TRUE(node2.getAdjacents().empty());
}

TEST(WeightedNodeTest, CopyConstructor) {
    using namespace anagraph;
    graph::WeightedNode node1;
    node1.setId(std::rand());
    node1.setAdjacent(4, 1.5);
    graph::WeightedNode node2(node1);
    EXPECT_EQ(node2.getId(), node1.getId());
    EXPECT_EQ(node2.getAdjacents(), node1.getAdjacents());
}

TEST(WeightedNodeTest, MoveConstructor) {
    using namespace anagraph;
    graph::WeightedNode node1;
    node1.setId(0);
    node1.setAdjacent(4, 1.5);
    graph::WeightedNode node2(std::move(node1));
    EXPECT_EQ(node2.getId(), 0);
    EXPECT_EQ(node2.getAdjacents().at(4), 1.5);
    EXPECT_FALSE(node1.isUsed());
    EXPECT_TRUE(node1.getAdjacents().empty());
}

TEST(WeightedNodeTest, SetAndGetId) {
    using namespace anagraph;
    graph::WeightedNode node1;
    node1.setId(3);
    EXPECT_EQ(node1.getId(), 3);
    node1.setId(4);
    EXPECT_EQ(node1.getId(), 4);
    node1.setId(-1);
    EXPECT_EQ(node1.getId(), 4);
}

TEST(WeightedNodeTest, IsUsed) {
    using namespace anagraph;
    graph::WeightedNode node1;
    graph::WeightedNode node2(1);
    graph::WeightedNode node3(2);
    EXPECT_FALSE(node1.isUsed());
    EXPECT_TRUE(node2.isUsed());
    EXPECT_TRUE(node3.isUsed());
}

TEST(WeightedNodeTest, SetAndGetAdjacents) {
    using namespace anagraph;
    graph::WeightedNode node1;
    node1.setAdjacent(4, 1.5);
    node1.setAdjacent(5, 2.0);
    const auto& adjacents = node1.getAdjacents();
    EXPECT_EQ(static_cast<int>(adjacents.size()), 2);
    EXPECT_EQ(adjacents.at(4), 1.5);
    EXPECT_EQ(adjacents.at(5), 2.0);
}

TEST(WeightedNodeTest, UpdateAdjacent) {
    using namespace anagraph;
    graph::WeightedNode node1;
    node1.setAdjacent(4, 1.5);
    node1.updateAdjacent(4, 0.5);
    node1.updateAdjacent(5, 1.5);
    const auto& adjacents = node1.getAdjacents();
    EXPECT_EQ(adjacents.at(4), 2.0);
    EXPECT_EQ(adjacents.at(5), 1.5);
}

TEST(WeightedNodeTest, RemoveAdjacent) {
    using namespace anagraph;
    graph::WeightedNode node1;
    node1.setAdjacent(4, 1.5);
    node1.setAdjacent(5, 2.0);
    EXPECT_EQ(static_cast<int>(node1.getAdjacents().size()), 2);
    node1.removeAdjacent(5);
    const auto& adjacents = node1.getAdjacents();
    EXPECT_EQ(adjacents.size(), static_cast<size_t>(1));
    EXPECT_FALSE(adjacents.contains(5));
}

TEST(WeightedNodeTest, Clear) {
    using namespace anagraph;
    graph::WeightedNode node1;
    node1.setAdjacent(4, 1.5);
    node1.clear();
    EXPECT_FALSE(node1.isUsed());
    EXPECT_EQ(node1.getId(), graph::WeightedNode::UNUSED_ID);
    EXPECT_TRUE(node1.getAdjacents().empty());
}