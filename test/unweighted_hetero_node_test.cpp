#include "anagraph/components/heterogeneous_graph.hpp"

#include <gtest/gtest.h>

#include <string>

TEST(HeteroNodeTest, DefaultConstructor) {
    HeteroNode<int> node1;
    EXPECT_TRUE(node1.getId() < 0);
    EXPECT_TRUE(node1.getAdjacents().empty());
}

TEST(HeteroNodeTest, ConstructorWithId) {
    HeteroNode<int> node2(1);
    EXPECT_EQ(node2.getId(), 1);
    EXPECT_TRUE(node2.getAdjacents().empty());
}

TEST(HeteroNodeTest, ConstructorWithIdAndAttributes) {
    HeteroNode<int> node3(2, 42);
    EXPECT_EQ(node3.getId(), 2);
    EXPECT_EQ(node3.getAttributes(), 42);
    EXPECT_TRUE(node3.getAdjacents().empty());
}

TEST(HeteroNodeTest, CopyConstructor) {
    HeteroNode<int> node1;
    node1.setId(std::rand());
    node1.setAdjacent(4);
    node1.setAttributes(10);
    HeteroNode<int> node2(node1);
    EXPECT_EQ(node2.getId(), node1.getId());
    EXPECT_EQ(node2.getAttributes(), node1.getAttributes());
    EXPECT_EQ(node2.getAdjacents(), node1.getAdjacents());
}

TEST(HeteroNodeTest, MoveConstructor) {
    HeteroNode<int> node1;
    node1.setId(0);
    node1.setAdjacent(4);
    node1.setAttributes(10);
    HeteroNode<int> node2(std::move(node1));
    EXPECT_EQ(node2.getId(), 0);
    EXPECT_EQ(node2.getAttributes(), 10);
    EXPECT_TRUE(node2.getAdjacents().contains(4));
    EXPECT_FALSE(node1.isUsed());
    EXPECT_TRUE(node1.getId() < 0);
    EXPECT_TRUE(node1.getAdjacents().empty());
}

TEST(HeteroNodeTest, SetAndGetId) {
    HeteroNode<int> node1;
    node1.setId(3);
    EXPECT_EQ(node1.getId(), 3);
    node1.setId(4);
    EXPECT_EQ(node1.getId(), 4);
    node1.setId(-1);
    EXPECT_EQ(node1.getId(), 4);
}

TEST(HeteroNodeTest, IsUsed) {
    HeteroNode<int> node1;
    HeteroNode<int> node2(1);
    HeteroNode<int> node3(2, 42);
    EXPECT_FALSE(node1.isUsed());
    EXPECT_TRUE(node2.isUsed());
    EXPECT_TRUE(node3.isUsed());
}

TEST(HeteroNodeTest, SetAndGetAdjacents) {
    HeteroNode<int> node1;
    node1.setAdjacent(4);
    node1.setAdjacent(5);
    const auto& adjacents = node1.getAdjacents();
    EXPECT_EQ(static_cast<int>(adjacents.size()), 2);
    EXPECT_TRUE(adjacents.contains(4));
    EXPECT_TRUE(adjacents.contains(5));
}

TEST(HeteroNodeTest, RemoveAdjacent) {
    HeteroNode<int> node1;
    node1.setAdjacent(4);
    node1.setAdjacent(5);
    EXPECT_EQ(node1.getAdjacents().size(), static_cast<size_t>(2));
    node1.removeAdjacent(5);
    const auto& adjacents = node1.getAdjacents();
    EXPECT_EQ(adjacents.size(), static_cast<size_t>(1));
    EXPECT_FALSE(adjacents.contains(5));
}

TEST(HeteroNodeTest, SetAndGetAttributes) {
    HeteroNode<int> node1;
    node1.setAttributes(10);
    EXPECT_EQ(node1.getAttributes(), 10);
    HeteroNode<std::string> node2;
    node2.setAttributes("hoge");
    EXPECT_EQ(node2.getAttributes(), "hoge");

    HeteroNode<int> node3;
    EXPECT_THROW(node3.getAttributes(), std::runtime_error);
}

TEST(HeteroNodeTest, Clear) {
    HeteroNode<int> node1;
    node1.setAdjacent(4);
    node1.setAttributes(10);
    node1.clear();
    EXPECT_FALSE(node1.isUsed());
    EXPECT_TRUE(node1.getId() < 0);
    EXPECT_TRUE(node1.getAdjacents().empty());
    EXPECT_THROW(node1.getAttributes(), std::runtime_error);
}