#include "anagraph/components/weighted_supernode.hpp"

#include <gtest/gtest.h>

TEST(WeightedSupernodeTest, DefaultConstructor) {
    using namespace anagraph::graph_structure;
    WeightedSupernode::resetNodesCount();
    WeightedSupernode node1;
    ASSERT_EQ(node1.getId(), 0);
    ASSERT_TRUE(node1.isUsed());
    ASSERT_TRUE(node1.isRoot());
    ASSERT_TRUE(node1.isLeaf());
    ASSERT_EQ(node1.getParent(), WeightedSupernode::ROOT);
    ASSERT_TRUE(node1.getChildren().empty());
    ASSERT_TRUE(node1.getAdjacents().empty());

    WeightedSupernode node2;
    ASSERT_EQ(node2.getId(), 1);
    ASSERT_TRUE(node2.isUsed());
    ASSERT_TRUE(node2.isRoot());
    ASSERT_TRUE(node2.isLeaf());
    ASSERT_EQ(node2.getParent(), WeightedSupernode::ROOT);
    ASSERT_TRUE(node2.getChildren().empty());
    ASSERT_TRUE(node2.getAdjacents().empty());
}

TEST(WeightedSupernodeTest, ConstructorWithId) {
    using namespace anagraph::graph_structure;
    WeightedSupernode node2(1);
    ASSERT_EQ(node2.getId(), 1);
    ASSERT_TRUE(node2.isUsed());
    ASSERT_TRUE(node2.isRoot());
    ASSERT_TRUE(node2.isLeaf());
    ASSERT_EQ(node2.getParent(), WeightedSupernode::ROOT);
    ASSERT_TRUE(node2.getChildren().empty());
    ASSERT_TRUE(node2.getAdjacents().empty());
}

TEST(WeightedSupernodeTest, DefaultConstructorAndIdConstructor) {
    using namespace anagraph::graph_structure;
    spdlog::set_level(spdlog::level::debug);
    WeightedSupernode::resetNodesCount();
    WeightedSupernode node1;
    ASSERT_EQ(node1.getId(), 0);

    WeightedSupernode node2(1);
    ASSERT_EQ(node2.getId(), 1);

    WeightedSupernode node3;
    ASSERT_EQ(node3.getId(), 2);

    WeightedSupernode node4(4);
    ASSERT_EQ(node4.getId(), 4);

    WeightedSupernode node5;
    ASSERT_EQ(node5.getId(), 5);

    WeightedSupernode::resetNodesCount();
    WeightedSupernode node6;
    ASSERT_EQ(node6.getId(), 0);
}

TEST(WeightedSupernodeTest, SetAndGetId) {
    using namespace anagraph::graph_structure;
    WeightedSupernode node1;
    node1.setId(2);
    ASSERT_EQ(node1.getId(), 2);
}

TEST(WeightedSupernodeTest, SetAndGetParent) {
    using namespace anagraph::graph_structure;
    WeightedSupernode node1;
    node1.setParent(1);
    ASSERT_EQ(node1.getParent(), 1);
}

TEST(WeightedSupernodeTest, AddAndGetChildren) {
    using namespace anagraph::graph_structure;
    WeightedSupernode node1;
    node1.addChild(3);
    node1.addChild(4);
    ASSERT_EQ(node1.getChildren().size(), static_cast<size_t>(2));
    ASSERT_EQ(node1.getChildren().count(3), static_cast<size_t>(1));
    ASSERT_EQ(node1.getChildren().count(4), static_cast<size_t>(1));
}

TEST(WeightedSupernodeTest, SetAndGetAdjacent) {
    using namespace anagraph::graph_structure;
    WeightedSupernode node1;
    node1.setAdjacent(5, 1.5);
    ASSERT_EQ(node1.getAdjacents().size(), static_cast<size_t>(1));
    ASSERT_EQ(node1.getAdjacents().count(5), static_cast<size_t>(1));
    ASSERT_EQ(node1.getAdjacents().at(5), 1.5);
}

TEST(WeightedNodeTest, SetAndGetAdjacentNodes) {
    using namespace anagraph;
    graph_structure::WeightedSupernode node1(1);
    graph_structure::WeightedSupernode node2(2);
    graph_structure::WeightedSupernode node3(3);

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

TEST(WeightedSupernodeTest, UpdateAdjacent) {
    using namespace anagraph::graph_structure;
    WeightedSupernode node1;
    node1.setAdjacent(5, 1.5);
    ASSERT_EQ(node1.getAdjacents().at(5), 1.5);
    node1.updateAdjacent(5, 2.5);
    ASSERT_EQ(node1.getAdjacents().at(5), 4.0);
    node1.setAdjacent(5, 2.5);
    ASSERT_EQ(node1.getAdjacents().at(5), 2.5);
}

TEST(WeightedSupernodeTest, RemoveAdjacent) {
    using namespace anagraph::graph_structure;
    WeightedSupernode node1;
    node1.setAdjacent(5, 1.5);
    ASSERT_EQ(node1.getAdjacents().at(5), 1.5);
    node1.removeAdjacent(5);
    ASSERT_TRUE(node1.getAdjacents().empty());
}

TEST(WeightedSupernodeTest, GetWeight) {
    using namespace anagraph::graph_structure;
    WeightedSupernode node1;
    node1.setAdjacent(5, 1.5);
    ASSERT_EQ(node1.getWeight(5), 1.5);
    ASSERT_EQ(node1.getWeight(6), 0.0);
}

TEST(WeightedSupernodeTest, Clear) {
    using namespace anagraph::graph_structure;
    WeightedSupernode node1;
    node1.setId(2);
    node1.setParent(1);
    node1.addChild(3);
    node1.setAdjacent(5, 1.5);
    node1.clear();
    ASSERT_EQ(node1.getId(), WeightedSupernode::UNUSED_ID);
    ASSERT_FALSE(node1.isUsed());
    ASSERT_TRUE(node1.isRoot());
    ASSERT_TRUE(node1.isLeaf());
    ASSERT_EQ(node1.getParent(), WeightedSupernode::ROOT);
    ASSERT_TRUE(node1.getChildren().empty());
    ASSERT_TRUE(node1.getAdjacents().empty());
}
