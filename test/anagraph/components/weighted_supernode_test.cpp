#include "anagraph/components/supergraph.hpp"

#include <gtest/gtest.h>

TEST(WeightedSupernodeTest, DefaultConstructor) {
    using namespace anagraph;
    WeightedSupernode node1;
    ASSERT_EQ(node1.getId(), WeightedSupernode::UNUSED_ID);
    ASSERT_FALSE(node1.isUsed());
    ASSERT_TRUE(node1.isRoot());
    ASSERT_TRUE(node1.isLeaf());
    ASSERT_EQ(node1.getParent(), WeightedSupernode::ROOT);
    ASSERT_TRUE(node1.getChildren().empty());
    ASSERT_TRUE(node1.getAdjacents().empty());
}

TEST(WeightedSupernodeTest, ConstructorWithId) {
    using namespace anagraph;
    WeightedSupernode node2(1);
    ASSERT_EQ(node2.getId(), 1);
    ASSERT_TRUE(node2.isUsed());
    ASSERT_TRUE(node2.isRoot());
    ASSERT_TRUE(node2.isLeaf());
    ASSERT_EQ(node2.getParent(), WeightedSupernode::ROOT);
    ASSERT_TRUE(node2.getChildren().empty());
    ASSERT_TRUE(node2.getAdjacents().empty());
}

TEST(WeightedSupernodeTest, SetAndGetId) {
    using namespace anagraph;
    WeightedSupernode node1;
    node1.setId(2);
    ASSERT_EQ(node1.getId(), 2);
}

TEST(WeightedSupernodeTest, SetAndGetParent) {
    using namespace anagraph;
    WeightedSupernode node1;
    node1.setParent(1);
    ASSERT_EQ(node1.getParent(), 1);
}

TEST(WeightedSupernodeTest, AddAndGetChildren) {
    using namespace anagraph;
    WeightedSupernode node1;
    node1.addChild(3);
    node1.addChild(4);
    ASSERT_EQ(node1.getChildren().size(), static_cast<size_t>(2));
    ASSERT_EQ(node1.getChildren().count(3), static_cast<size_t>(1));
    ASSERT_EQ(node1.getChildren().count(4), static_cast<size_t>(1));
}

TEST(WeightedSupernodeTest, SetAndGetAdjacent) {
    using namespace anagraph;
    WeightedSupernode node1;
    node1.setAdjacent(5, 1.5);
    ASSERT_EQ(node1.getAdjacents().size(), static_cast<size_t>(1));
    ASSERT_EQ(node1.getAdjacents().count(5), static_cast<size_t>(1));
    ASSERT_EQ(node1.getAdjacents().at(5), 1.5);
}

TEST(WeightedSupernodeTest, UpdateAdjacent) {
    using namespace anagraph;
    WeightedSupernode node1;
    node1.setAdjacent(5, 1.5);
    ASSERT_EQ(node1.getAdjacents().at(5), 1.5);
    node1.updateAdjacent(5, 2.5);
    ASSERT_EQ(node1.getAdjacents().at(5), 4.0);
    node1.setAdjacent(5, 2.5);
    ASSERT_EQ(node1.getAdjacents().at(5), 2.5);
}

TEST(WeightedSupernodeTest, RemoveAdjacent) {
    using namespace anagraph;
    WeightedSupernode node1;
    node1.setAdjacent(5, 1.5);
    ASSERT_EQ(node1.getAdjacents().at(5), 1.5);
    node1.removeAdjacent(5);
    ASSERT_TRUE(node1.getAdjacents().empty());
}

TEST(WeightedSupernodeTest, Clear) {
    using namespace anagraph;
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
