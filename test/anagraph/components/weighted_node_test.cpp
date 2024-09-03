#include "anagraph/components/weighted_graph.hpp"

#include <gtest/gtest.h>

#include <string>

TEST(WeightedNodeTest, DefaultConstructor) {
    using namespace anagraph;
    graph_structure::WeightedNode::resetNodesCount();
    graph_structure::WeightedNode node1;
    EXPECT_EQ(node1.getId(), 0);
    EXPECT_TRUE(node1.getAdjacents().empty());

    graph_structure::WeightedNode node2;
    EXPECT_EQ(node2.getId(), 1);
    EXPECT_TRUE(node2.getAdjacents().empty());
}

TEST(WeightedNodeTest, ConstructorWithId) {
    using namespace anagraph;
    graph_structure::WeightedNode node2(1);
    EXPECT_EQ(node2.getId(), 1);
    EXPECT_TRUE(node2.getAdjacents().empty());
}

TEST(WeightedNodeTest, DefaultConstructorAndIdConstructor) {
    using namespace anagraph::graph_structure;
    spdlog::set_level(spdlog::level::debug);
    WeightedNode::resetNodesCount();
    WeightedNode node1;
    ASSERT_EQ(node1.getId(), 0);

    WeightedNode node2(1);
    ASSERT_EQ(node2.getId(), 1);

    WeightedNode node3;
    ASSERT_EQ(node3.getId(), 2);

    WeightedNode node4(4);
    ASSERT_EQ(node4.getId(), 4);

    WeightedNode node5;
    ASSERT_EQ(node5.getId(), 5);

    WeightedNode::resetNodesCount();
    WeightedNode node6;
    ASSERT_EQ(node6.getId(), 0);
}

TEST(WeightedNodeTest, CopyConstructor) {
    using namespace anagraph;
    graph_structure::WeightedNode node1;
    node1.setId(std::rand());
    node1.setAdjacent(4, 1.5);
    graph_structure::WeightedNode node2(node1);
    EXPECT_EQ(node2.getId(), node1.getId());
    EXPECT_EQ(node2.getAdjacents(), node1.getAdjacents());
}

TEST(WeightedNodeTest, MoveConstructor) {
    using namespace anagraph;
    graph_structure::WeightedNode node1;
    node1.setId(0);
    node1.setAdjacent(4, 1.5);
    graph_structure::WeightedNode node2(std::move(node1));
    EXPECT_EQ(node2.getId(), 0);
    EXPECT_EQ(node2.getAdjacents().at(4), 1.5);
    EXPECT_FALSE(node1.isUsed());
    EXPECT_TRUE(node1.getAdjacents().empty());
}

TEST(WeightedNodeTest, SetAndGetId) {
    using namespace anagraph;
    graph_structure::WeightedNode node1;
    node1.setId(3);
    EXPECT_EQ(node1.getId(), 3);
    node1.setId(4);
    EXPECT_EQ(node1.getId(), 4);
    node1.setId(-1);
    EXPECT_EQ(node1.getId(), 4);
}

TEST(WeightedNodeTest, IsUsed) {
    using namespace anagraph;
    graph_structure::WeightedNode node1;
    graph_structure::WeightedNode node2(1);
    graph_structure::WeightedNode node3(2);
    EXPECT_TRUE(node1.isUsed());
    EXPECT_TRUE(node2.isUsed());
    EXPECT_TRUE(node3.isUsed());

    node1.clear();
    EXPECT_FALSE(node1.isUsed());
}

TEST(WeightedNodeTest, SetAndGetAdjacents) {
    using namespace anagraph;
    graph_structure::WeightedNode node1;
    node1.setAdjacent(4, 1.5);
    node1.setAdjacent(5, 2.0);
    const auto& adjacents = node1.getAdjacents();
    EXPECT_EQ(static_cast<int>(adjacents.size()), 2);
    EXPECT_EQ(adjacents.at(4), 1.5);
    EXPECT_EQ(adjacents.at(5), 2.0);
}

TEST(WeightedNodeTest, SetAndGetAdjacentNodes) {
    using namespace anagraph;
    graph_structure::WeightedNode node1(1);
    graph_structure::WeightedNode node2(2);
    graph_structure::WeightedNode node3(3);

    node1.setAdjacentNode(node2, 1.5);
    node1.setAdjacentNode(node3, 2.0);
    const auto& adjacents = node1.getAdjacentNodes();
    EXPECT_EQ(static_cast<int>(adjacents.size()), 2);
    EXPECT_TRUE(adjacents.contains(2));
    EXPECT_TRUE(adjacents.contains(3));

    auto& adj = adjacents.at(2).get();
    adj.setAdjacentNode(node3, 1.5);    
    EXPECT_EQ(adj.getId(), 2);
    EXPECT_TRUE(adj.getAdjacents().contains(3));
    EXPECT_TRUE(node2.getAdjacents().contains(3));
}

TEST(WeightedNodeTest, UpdateAdjacent) {
    using namespace anagraph;
    graph_structure::WeightedNode node1;
    node1.setAdjacent(4, 1.5);
    node1.updateAdjacent(4, 0.5);
    node1.updateAdjacent(5, 1.5);
    const auto& adjacents = node1.getAdjacents();
    EXPECT_EQ(adjacents.at(4), 2.0);
    EXPECT_EQ(adjacents.at(5), 1.5);
}

TEST(WeightedNodeTest, RemoveAdjacent) {
    using namespace anagraph;
    graph_structure::WeightedNode node1;
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
    graph_structure::WeightedNode node1;
    node1.setAdjacent(4, 1.5);
    node1.clear();
    EXPECT_FALSE(node1.isUsed());
    EXPECT_EQ(node1.getId(), graph_structure::WeightedNode::UNUSED_ID);
    EXPECT_TRUE(node1.getAdjacents().empty());
}