#include "anagraph/components/unweighted_graph.hpp"

#include <gtest/gtest.h>

#include <string>

TEST(UnweightedNodeTest, DefaultConstructor) {
    using namespace anagraph::graph_structure;
    Node::resetNodesCount();
    Node node1;
    EXPECT_EQ(node1.getId(), 0);
    EXPECT_TRUE(node1.getAdjacents().empty());
}

TEST(UnweightedNodeTest, ConstructorWithId) {
    using namespace anagraph::graph_structure;
    Node node2(1);
    EXPECT_EQ(node2.getId(), 1);
    EXPECT_TRUE(node2.getAdjacents().empty());
}

TEST(UnweightedNodeTest, DefaultConstructorAndIdConstructor) {
    using namespace anagraph::graph_structure;
    Node::resetNodesCount();
    Node node1;
    ASSERT_EQ(node1.getId(), 0);

    Node node2(1);
    ASSERT_EQ(node2.getId(), 1);

    Node node3;
    ASSERT_EQ(node3.getId(), 2);

    Node node4(4);
    ASSERT_EQ(node4.getId(), 4);

    Node node5;
    ASSERT_EQ(node5.getId(), 5);

    Node::resetNodesCount();
    Node node6;
    ASSERT_EQ(node6.getId(), 0);
}

TEST(UnweightedNodeTest, CopyConstructor) {
    using namespace anagraph::graph_structure;
    Node node1;
    node1.setId(std::rand());
    node1.setAdjacent(4);
    Node node2(node1);
    EXPECT_EQ(node2.getId(), node1.getId());
    EXPECT_EQ(node2.getAdjacents(), node1.getAdjacents());
}

TEST(UnweightedNodeTest, MoveConstructor) {
    using namespace anagraph::graph_structure;
    Node node1;
    node1.setId(0);
    node1.setAdjacent(4);
    Node node2(std::move(node1));
    EXPECT_EQ(node2.getId(), 0);
    EXPECT_TRUE(node2.getAdjacents().contains(4));
    EXPECT_FALSE(node1.isUsed());
    EXPECT_TRUE(node1.getAdjacents().empty());
}

TEST(UnweightedNodeTest, SetAndGetId) {
    using namespace anagraph::graph_structure;
    Node node1;
    node1.setId(3);
    EXPECT_EQ(node1.getId(), 3);
    node1.setId(4);
    EXPECT_EQ(node1.getId(), 4);
    node1.setId(-1);
    EXPECT_EQ(node1.getId(), 4);
}

TEST(UnweightedNodeTest, IsUsed) {
    using namespace anagraph::graph_structure;
    Node node1;
    Node node2(1);
    Node node3(2);
    EXPECT_TRUE(node1.isUsed());
    EXPECT_TRUE(node2.isUsed());
    EXPECT_TRUE(node3.isUsed());

    node1.clear();
    EXPECT_FALSE(node1.isUsed());
}

TEST(UnweightedNodeTest, SetAndGetAdjacents) {
    using namespace anagraph::graph_structure;
    Node node1;
    node1.setAdjacent(4);
    node1.setAdjacent(5);
    const auto& adjacents = node1.getAdjacents();
    EXPECT_EQ(static_cast<int>(adjacents.size()), 2);
    EXPECT_TRUE(adjacents.contains(4));
    EXPECT_TRUE(adjacents.contains(5));
}

TEST(UnweightedNodeTest, SetAndGetAdjacentNodes) {
    using namespace anagraph::graph_structure;
    Node node1(1);
    Node node2(2);
    Node node3(3);
    node1.setAdjacentNode(node2);
    node1.setAdjacentNode(node3);
    const auto& adjacents = node1.getAdjacentNodes();
    EXPECT_EQ(static_cast<int>(adjacents.size()), 2);
    EXPECT_TRUE(adjacents.contains(2));
    EXPECT_TRUE(adjacents.contains(3));

    auto &adj = adjacents.at(2).get();
    node2.setAdjacentNode(node3); // update after getting the adjacent node
    EXPECT_EQ(adj.getId(), 2);
    EXPECT_TRUE(node2.getAdjacents().contains(3));
    EXPECT_TRUE(adj.getAdjacents().contains(3));
}

TEST(UnweightedNodeTest, RemoveAdjacent) {
    using namespace anagraph::graph_structure;
    Node node1;
    node1.setAdjacent(4);
    node1.setAdjacent(5);
    EXPECT_EQ(static_cast<int>(node1.getAdjacents().size()), 2);
    node1.removeAdjacent(5);
    const auto& adjacents = node1.getAdjacents();
    EXPECT_EQ(adjacents.size(), static_cast<size_t>(1));
    EXPECT_FALSE(adjacents.contains(5));
}

TEST(UnweightedNodeTest, Clear) {
    using namespace anagraph::graph_structure;
    Node node1;
    node1.setAdjacent(4);
    node1.clear();
    EXPECT_FALSE(node1.isUsed());
    EXPECT_EQ(node1.getId(), Node::UNUSED_ID);
    EXPECT_TRUE(node1.getAdjacents().empty());
}