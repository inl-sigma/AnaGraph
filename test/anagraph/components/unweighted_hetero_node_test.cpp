#include "anagraph/components/unweighted_hetero_node.hpp"

#include <gtest/gtest.h>

#include <string>

TEST(HeteroNodeTest, DefaultConstructor) {
    using namespace anagraph;
    graph_structure::HeteroNode<int>::resetNodesCount();
    graph_structure::HeteroNode<int> node1;
    EXPECT_EQ(node1.getId(), 0);
    EXPECT_TRUE(node1.getAdjacents().empty());
}

TEST(HeteroNodeTest, ConstructorWithId) {
    using namespace anagraph;
    graph_structure::HeteroNode<int> node2(1);
    EXPECT_EQ(node2.getId(), 1);
    EXPECT_TRUE(node2.getAdjacents().empty());
}

TEST(UnweightedNodeTest, DefaultConstructorAndIdConstructor) {
    using namespace anagraph::graph_structure;
    HeteroNode<int>::resetNodesCount();
    HeteroNode<int> node1;
    ASSERT_EQ(node1.getId(), 0);

    HeteroNode<int> node2(1);
    ASSERT_EQ(node2.getId(), 1);

    HeteroNode<std::any> node3;
    ASSERT_EQ(node3.getId(), 2);

    HeteroNode<int> node4(4);
    ASSERT_EQ(node4.getId(), 4);

    HeteroNode<int> node5;
    ASSERT_EQ(node5.getId(), 5);

    HeteroNode<int>::resetNodesCount();
    HeteroNode<int> node6;
    ASSERT_EQ(node6.getId(), 0);
}

TEST(HeteroNodeTest, ConstructorWithIdAndAttributes) {
    using namespace anagraph;
    graph_structure::HeteroNode<int> node3(2, 42);
    EXPECT_EQ(node3.getId(), 2);
    EXPECT_EQ(node3.getAttributes(), 42);
    EXPECT_TRUE(node3.getAdjacents().empty());
}

TEST(HeteroNodeTest, CopyConstructor) {
    using namespace anagraph;
    graph_structure::HeteroNode<int> node1;
    node1.setId(std::rand());
    node1.setAdjacent(4);
    node1.setAttributes(10);
    graph_structure::HeteroNode<int> node2(node1);
    EXPECT_EQ(node2.getId(), node1.getId());
    EXPECT_EQ(node2.getAttributes(), node1.getAttributes());
    EXPECT_EQ(node2.getAdjacents(), node1.getAdjacents());
}

TEST(HeteroNodeTest, MoveConstructor) {
    using namespace anagraph;
    graph_structure::HeteroNode<int> node1;
    node1.setId(0);
    node1.setAdjacent(4);
    node1.setAttributes(10);
    graph_structure::HeteroNode<int> node2(std::move(node1));
    EXPECT_EQ(node2.getId(), 0);
    EXPECT_EQ(node2.getAttributes(), 10);
    EXPECT_TRUE(node2.getAdjacents().contains(4));
    EXPECT_FALSE(node1.isUsed());
    EXPECT_TRUE(node1.getId() < 0);
    EXPECT_TRUE(node1.getAdjacents().empty());
}

TEST(HeteroNodeTest, SetAndGetId) {
    using namespace anagraph;
    graph_structure::HeteroNode<int> node1;
    node1.setId(3);
    EXPECT_EQ(node1.getId(), 3);
    node1.setId(4);
    EXPECT_EQ(node1.getId(), 4);
    node1.setId(-1);
    EXPECT_EQ(node1.getId(), 4);
}

TEST(HeteroNodeTest, IsUsed) {
    using namespace anagraph;
    graph_structure::HeteroNode<int> node1;
    graph_structure::HeteroNode<int> node2(1);
    graph_structure::HeteroNode<int> node3(2, 42);
    EXPECT_TRUE(node1.isUsed());
    EXPECT_TRUE(node2.isUsed());
    EXPECT_TRUE(node3.isUsed());
    
    node1.clear();
    EXPECT_FALSE(node1.isUsed());
}

TEST(HeteroNodeTest, SetAndGetAdjacents) {
    using namespace anagraph;
    graph_structure::HeteroNode<int> node1;
    node1.setAdjacent(4);
    node1.setAdjacent(5);
    const auto& adjacents = node1.getAdjacents();
    EXPECT_EQ(static_cast<int>(adjacents.size()), 2);
    EXPECT_TRUE(adjacents.contains(4));
    EXPECT_TRUE(adjacents.contains(5));
}

TEST(HeteroNodeTest, RemoveAdjacent) {
    using namespace anagraph;
    graph_structure::HeteroNode<int> node1;
    node1.setAdjacent(4);
    node1.setAdjacent(5);
    EXPECT_EQ(node1.getAdjacents().size(), static_cast<size_t>(2));
    node1.removeAdjacent(5);
    const auto& adjacents = node1.getAdjacents();
    EXPECT_EQ(adjacents.size(), static_cast<size_t>(1));
    EXPECT_FALSE(adjacents.contains(5));
}

TEST(HeteroNodeTest, SetAndGetAttributes) {
    using namespace anagraph;
    graph_structure::HeteroNode<int> node1;
    node1.setAttributes(10);
    EXPECT_EQ(node1.getAttributes(), 10);
    graph_structure::HeteroNode<std::string> node2;
    node2.setAttributes("hoge");
    EXPECT_EQ(node2.getAttributes(), "hoge");

    graph_structure::HeteroNode<int> node3;
    EXPECT_THROW(node3.getAttributes(), std::runtime_error);
}

TEST(HeteroNodeTest, SetAndGetAdjacentNodes) {
    using namespace anagraph;
    graph_structure::HeteroNode<int> node1(0);
    graph_structure::HeteroNode<int> node2(1);
    graph_structure::HeteroNode<int> node3(2);
    node1.setAdjacentNode(node2);
    node1.setAdjacentNode(node3);

    const auto& adjacents = node1.getAdjacentNodes();
    EXPECT_EQ(static_cast<int>(adjacents.size()), 2);
    EXPECT_TRUE(adjacents.contains(1));
    EXPECT_TRUE(adjacents.contains(2));

    const auto& adj = adjacents.at(1).get();
    node2.setAdjacentNode(node3);
    EXPECT_EQ(static_cast<int>(adj.getAdjacents().size()), 1);
    EXPECT_TRUE(adj.getAdjacents().contains(2));
    EXPECT_TRUE(node2.getAdjacents().contains(2));
}  

TEST(HeteroNodeTest, Clear) {
    using namespace anagraph;
    graph_structure::HeteroNode<int> node1;
    node1.setAdjacent(4);
    node1.setAttributes(10);
    node1.clear();
    EXPECT_FALSE(node1.isUsed());
    EXPECT_EQ(node1.getId(), node1.UNUSED_ID);
    EXPECT_TRUE(node1.getAdjacents().empty());
    EXPECT_THROW(node1.getAttributes(), std::runtime_error);
}