#include "supergraph.hpp"

#include <gtest/gtest.h>

TEST(WeightedSuperDigraphTest, SetNode) {
    WeightedSuperDigraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(4);

    EXPECT_EQ(graph.size(), 3);
}

TEST(WeightedSuperDigraphTest, GetNode) {
    WeightedSuperDigraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    WeightedSupernode node1 = graph.getNode(1);
    WeightedSupernode node2 = graph.getNode(2);
    WeightedSupernode node3 = graph.getNode(3);

    EXPECT_EQ(node1.getId(), 1);
    EXPECT_EQ(node2.getId(), 2);
    EXPECT_EQ(node3.getId(), 3);
}

TEST(WeightedSuperDigraphTest, RemoveNode) {
    WeightedSuperDigraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    graph.removeNode(2);

    EXPECT_EQ(graph.size(), 2);
    EXPECT_THROW(graph.getNode(2), std::out_of_range);

    WeightedSupernode node1 = graph.getNode(1);
    EXPECT_EQ(node1.getId(), 1);
}

TEST(WeightedSuperDigraphTest, AddEdge) {
    WeightedSuperDigraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    graph.addEdge(1, 2, 5.0);
    graph.addEdge(2, 3, 3.0);

    EXPECT_EQ(graph.getWeight(1, 2), 5.0);
    EXPECT_EQ(graph.getWeight(2, 3), 3.0);
}

TEST(WeightedSuperDigraphTest, RemoveEdge) {
    WeightedSuperDigraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    graph.addEdge(1, 2, 5.0);
    graph.addEdge(2, 3, 3.0);

    graph.removeEdge(1, 2);

    EXPECT_DOUBLE_EQ(graph.getWeight(1, 2), 0.0);
    EXPECT_DOUBLE_EQ(graph.getWeight(2, 3), 3.0);
}

TEST(WeightedSuperDigraphTest, GetWeight) {
    WeightedSuperDigraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    graph.addEdge(1, 2, 5.0);
    graph.addEdge(2, 3, 3.0);

    EXPECT_DOUBLE_EQ(graph.getWeight(1, 2), 5.0);
    EXPECT_DOUBLE_EQ(graph.getWeight(2, 3), 3.0);
}

TEST(WeightedSuperDigraphTest, SetWeight) {
    WeightedSuperDigraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    graph.addEdge(1, 2, 5.0);
    graph.addEdge(2, 3, 3.0);

    graph.setWeight(1, 2, 10.0);
    graph.setWeight(2, 3, 7.0);

    EXPECT_EQ(graph.getWeight(1, 2), 10.0);
    EXPECT_EQ(graph.getWeight(2, 3), 7.0);
}

TEST(WeightedSuperDigraphTest, AddWeight) {
    WeightedSuperDigraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    graph.addEdge(1, 2, 5.0);
    graph.addEdge(2, 3, 3.0);

    graph.addWeight(1, 2, 2.0);
    graph.addWeight(2, 3, -1.0);

    EXPECT_EQ(graph.getWeight(1, 2), 7.0);
    EXPECT_EQ(graph.getWeight(2, 3), 2.0);
}

TEST(WeightedSuperDigraphTest, GetAndSetParent) {
    WeightedSuperDigraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    graph.setParent(2, 1);
    graph.setParent(3, 2);

    EXPECT_EQ(graph.getParent(2), 1);
    EXPECT_EQ(graph.getParent(3), 2);

    EXPECT_EQ(graph.getParent(1), WeightedSupernode::ROOT);
}

TEST(WeightedSuperDigraphTest, UpdateParent) {
    WeightedSuperDigraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    graph.setParent(2, 1);
    graph.setParent(3, 2);
    EXPECT_EQ(graph.getParent(3), 2);

    graph.updateParent(3, 1);
    EXPECT_EQ(graph.getParent(3), 1);
}

TEST(WeightedSuperDigraphTest, RemoveParent) {
    WeightedSuperDigraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    graph.setParent(2, 1);
    graph.setParent(3, 2);

    graph.removeParent(3);

    EXPECT_EQ(graph.getParent(3), WeightedSupernode::ROOT);
}

TEST(WeightedSuperDigraphTest, GetChildren) {
    WeightedSuperDigraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);
    graph.setNode(4);

    graph.setParent(2, 1);
    graph.setParent(3, 2);
    graph.setParent(4, 2);

    std::unordered_set<int> children1 = graph.getChildren(1);
    std::unordered_set<int> children2 = graph.getChildren(2);

    EXPECT_EQ(children1.size(), 1);
    EXPECT_EQ(children2.size(), 2);
    EXPECT_TRUE(children1.contains(2));
    EXPECT_TRUE(children2.contains(3));
    EXPECT_TRUE(children2.contains(4));
}

TEST(WeightedSuperDigraphTest, GetAdjacents) {
    WeightedSuperDigraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);
    graph.setNode(4);
    graph.setNode(5);
    graph.setNode(6);

    graph.addEdge(1, 2, 5.0);

    graph.setParent(3, 4);
    graph.addEdge(4, 5, 3.0);
    graph.addEdge(4, 6, 2.0);
    graph.addEdge(3, 4, -3.0);

    const std::unordered_map<int, double>& adjacents1 = graph.getAdjacents(1);
    EXPECT_EQ(adjacents1.size(), 1);
    EXPECT_EQ(adjacents1.at(2), 5.0);

    const std::unordered_map<int, double>& adjacents3 = graph.getAdjacents(3);
    EXPECT_EQ(adjacents3.size(), 1);
    EXPECT_FALSE(adjacents3.contains(4));
    EXPECT_EQ(graph.getWeight(3, 4), -3.0);
    EXPECT_EQ(adjacents3.at(6), 2.0);
}

TEST(WeightedSuperDigraphTest, Size) {
    WeightedSuperDigraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    EXPECT_EQ(graph.size(), 3);
}

TEST(WeightedSuperDigraphTest, ReadGraph) {
    WeightedSuperDigraph graph;
    graph.readGraph("../../dataset/input", FileExtension::TXT);

    EXPECT_EQ(graph.size(), static_cast<size_t>(6));
    EXPECT_EQ(graph.getWeight(0, 1), 1.0);
    EXPECT_EQ(graph.getWeight(0, 2), 1.0);
    EXPECT_EQ(graph.getWeight(1, 2), 1.0);
    EXPECT_EQ(graph.getWeight(2, 3), 1.0);
    EXPECT_EQ(graph.getWeight(2, 4), 1.0);
    EXPECT_EQ(graph.getWeight(5, 4), 1.0);

    EXPECT_EQ(graph.getParent(0), WeightedSupernode::ROOT);
    EXPECT_EQ(graph.getParent(1), 5);
    EXPECT_EQ(graph.getParent(2), WeightedSupernode::ROOT);
    EXPECT_EQ(graph.getParent(3), WeightedSupernode::ROOT);
    EXPECT_EQ(graph.getParent(4), WeightedSupernode::ROOT);
    EXPECT_EQ(graph.getParent(5), WeightedSupernode::ROOT);
}

TEST(WeightedSuperDigraphTest, WriteGraph) {
    WeightedSuperDigraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);
    graph.addEdge(1, 2, 5.0);
    graph.addEdge(2, 3, 3.0);

    graph.writeGraph("../../dataset/output", FileExtension::TXT);
}