#include "anagraph/components/weighted_hetero_node.hpp"

#include <gtest/gtest.h>

#include <string>

TEST(WeightedHeteroNodeTest, DefaultConstructor) {
    using namespace anagraph;
    graph_structure::WeightedHeteroNode<int> node1;
    EXPECT_EQ(node1.getId(), 0);
    EXPECT_TRUE(node1.getAdjacents().empty());
}

TEST(WeightedHeteroNodeTest, ConstructorWithId) {
    using namespace anagraph;
    graph_structure::WeightedHeteroNode<int> node2(1);
    EXPECT_EQ(node2.getId(), 1);
    EXPECT_TRUE(node2.getAdjacents().empty());
}

TEST(WeightedHeteroNodeTest, ConstructorWithIdAndAttributes) {
    using namespace anagraph;
    graph_structure::WeightedHeteroNode<int> node3(2, 42);
    EXPECT_EQ(node3.getId(), 2);
    EXPECT_EQ(node3.getAttributes(), 42);
    EXPECT_TRUE(node3.getAdjacents().empty());
}

TEST(WeightedHeteroNodeTest, DefaultConstructorAndIdConstructor) {
    using namespace anagraph::graph_structure;
    spdlog::set_level(spdlog::level::debug);
    WeightedHeteroNode<int> node1;
    ASSERT_EQ(node1.getId(), 0);

    WeightedHeteroNode<int> node2(1);
    ASSERT_EQ(node2.getId(), 1);

    WeightedHeteroNode<int> node3;
    ASSERT_EQ(node3.getId(), 2);

    WeightedHeteroNode<int> node4(4);
    ASSERT_EQ(node4.getId(), 4);

    WeightedHeteroNode<int> node5;
    ASSERT_EQ(node5.getId(), 5);
}

TEST(WeightedHeteroNodeTest, CopyConstructor) {
    using namespace anagraph;
    graph_structure::WeightedHeteroNode<int> node1;
    node1.setId(std::rand());
    node1.setAdjacent(4, 1.5);
    node1.setAttributes(10);
    graph_structure::WeightedHeteroNode<int> node2(node1);
    EXPECT_EQ(node2.getId(), node1.getId());
    EXPECT_EQ(node2.getAttributes(), node1.getAttributes());
    EXPECT_EQ(node2.getAdjacents(), node1.getAdjacents());
}

TEST(WeightedHeteroNodeTest, MoveConstructor) {
    using namespace anagraph;
    graph_structure::WeightedHeteroNode<int> node1;
    node1.setId(0);
    node1.setAdjacent(4, 1.5);
    node1.setAttributes(10);
    graph_structure::WeightedHeteroNode<int> node2(std::move(node1));
    EXPECT_EQ(node2.getId(), 0);
    EXPECT_EQ(node2.getAttributes(), 10);
    EXPECT_EQ(node2.getAdjacents().at(4), 1.5);
    EXPECT_FALSE(node1.isUsed());
    EXPECT_TRUE(node1.getId() < 0);
    EXPECT_TRUE(node1.getAdjacents().empty());
}

TEST(WeightedHeteroNodeTest, SetAndGetId) {
    using namespace anagraph;
    graph_structure::WeightedHeteroNode<int> node1;
    node1.setId(3);
    EXPECT_EQ(node1.getId(), 3);
    node1.setId(4);
    EXPECT_EQ(node1.getId(), 4);
    node1.setId(-1);
    EXPECT_EQ(node1.getId(), 4);
}

TEST(WeightedHeteroNodeTest, IsUsed) {
    using namespace anagraph;
    graph_structure::WeightedHeteroNode<int> node1;
    graph_structure::WeightedHeteroNode<int> node2(1);
    graph_structure::WeightedHeteroNode<int> node3(2, 42);
    EXPECT_TRUE(node1.isUsed());
    EXPECT_TRUE(node2.isUsed());
    EXPECT_TRUE(node3.isUsed());

    node1.clear();
    EXPECT_FALSE(node1.isUsed());
}

TEST(WeightedHeteroNodeTest, SetAndGetAdjacents) {
    using namespace anagraph;
    graph_structure::WeightedHeteroNode<int> node1;
    node1.setAdjacent(4, 1.5);
    node1.setAdjacent(5, 2.0);
    const auto& adjacents = node1.getAdjacents();
    EXPECT_EQ(static_cast<int>(adjacents.size()), 2);
    EXPECT_EQ(adjacents.at(4), 1.5);
    EXPECT_EQ(adjacents.at(5), 2.0);
}

TEST(WeightedHeteroNodeTest, SetAndGetAdjacentNodes) {
    using namespace anagraph;
    graph_structure::WeightedHeteroNode<int> node1(1);
    graph_structure::WeightedHeteroNode<int> node2(2);
    graph_structure::WeightedHeteroNode<int> node3(3);

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

TEST(WeightedHeteroNodeTest, UpdateAdjacent) {
    using namespace anagraph;
    graph_structure::WeightedHeteroNode<int> node1;
    node1.setAdjacent(4, 1.5);
    node1.updateAdjacent(4, 0.5);
    node1.updateAdjacent(5, 1.5);
    const auto& adjacents = node1.getAdjacents();
    EXPECT_EQ(adjacents.at(4), 2.0);
    EXPECT_EQ(adjacents.at(5), 1.5);
}

TEST(WeightedHeteroNodeTest, RemoveAdjacent) {
    using namespace anagraph;
    graph_structure::WeightedHeteroNode<int> node1;
    node1.setAdjacent(4, 1.5);
    node1.setAdjacent(5, 2.0);
    EXPECT_EQ(node1.getAdjacents().size(), static_cast<size_t>(2));
    node1.removeAdjacent(5);
    const auto& adjacents = node1.getAdjacents();
    EXPECT_EQ(adjacents.size(), static_cast<size_t>(1));
    EXPECT_FALSE(adjacents.contains(5));
}

TEST(WeightedHeteroNodeTest, SetAndGetAttributes) {
    using namespace anagraph;
    graph_structure::WeightedHeteroNode<int> node1;
    node1.setAttributes(10);
    EXPECT_EQ(node1.getAttributes(), 10);
    graph_structure::WeightedHeteroNode<std::string> node2;
    node2.setAttributes("hoge");
    EXPECT_EQ(node2.getAttributes(), "hoge");

    graph_structure::WeightedHeteroNode<int> node3;
    EXPECT_THROW(node3.getAttributes(), std::runtime_error);
}

TEST(WeightedHeteroNodeTest, Clear) {
    using namespace anagraph;
    graph_structure::WeightedHeteroNode<int> node1;
    node1.setAdjacent(4, 1.5);
    node1.setAttributes(10);
    node1.clear();
    EXPECT_FALSE(node1.isUsed());
    EXPECT_EQ(node1.getId(), node1.UNUSED_ID);
    EXPECT_TRUE(node1.getAdjacents().empty());
    EXPECT_THROW(node1.getAttributes(), std::runtime_error);
}