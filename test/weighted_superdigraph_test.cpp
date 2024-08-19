#include "supergraph.hpp"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

TEST(WeightedSuperDigraphTest, SetNode) {
    spdlog::set_level(spdlog::level::debug);
    WeightedSuperDigraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(4);

    EXPECT_EQ(graph.size(), static_cast<size_t>(3));
}

TEST(WeightedSuperDigraphTest, GetNode) {
    spdlog::set_level(spdlog::level::debug);
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
    spdlog::set_level(spdlog::level::debug);
    WeightedSuperDigraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    graph.removeNode(2);

    EXPECT_EQ(graph.size(), static_cast<size_t>(2));

    WeightedSupernode node1 = graph.getNode(1);
    EXPECT_EQ(node1.getId(), 1);
}

TEST(WeightedSuperDigraphTest, GetIds) {
    spdlog::set_level(spdlog::level::debug);
    WeightedSuperDigraph graph;
    graph.setNode(1);
    graph.setNode(3);
    graph.setNode(4);

    std::unordered_set<int> ids = graph.getIds();

    EXPECT_EQ(ids.size(), static_cast<size_t>(3));
    EXPECT_TRUE(ids.contains(1));
    EXPECT_TRUE(ids.contains(3));
    EXPECT_TRUE(ids.contains(4));

    graph.removeNode(3);
    ids = graph.getIds();

    EXPECT_EQ(ids.size(), static_cast<size_t>(2));
    EXPECT_TRUE(ids.contains(1));
    EXPECT_TRUE(ids.contains(4));
}

TEST(WeightedSuperDigraphTest, AddEdge) {
    spdlog::set_level(spdlog::level::debug);
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
    spdlog::set_level(spdlog::level::debug);
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
    spdlog::set_level(spdlog::level::debug);
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
    spdlog::set_level(spdlog::level::debug);
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
    spdlog::set_level(spdlog::level::debug);
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
    spdlog::set_level(spdlog::level::debug);
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
    spdlog::set_level(spdlog::level::debug);
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
    spdlog::set_level(spdlog::level::debug);
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
    spdlog::set_level(spdlog::level::debug);
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

    EXPECT_EQ(children1.size(), static_cast<size_t>(1));
    EXPECT_EQ(children2.size(), static_cast<size_t>(2));
    EXPECT_TRUE(children1.contains(2));
    EXPECT_TRUE(children2.contains(3));
    EXPECT_TRUE(children2.contains(4));
}

TEST(WeightedSuperDigraphTest, GetAdjacents) {
    spdlog::set_level(spdlog::level::debug);
    WeightedSuperDigraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);
    graph.setNode(4);
    graph.setNode(5);
    graph.setNode(6);

    graph.addEdge(1, 2, 5.0);

    graph.setParent(3, 4);
    graph.addEdge(3, 2, 1.0);
    graph.addEdge(3, 5, -3.0);
    graph.addEdge(4, 5, 3.0);
    graph.addEdge(4, 6, 2.0);

    const std::unordered_map<int, double>& adjacents1 = graph.getAdjacents(1);
    EXPECT_EQ(adjacents1.size(), static_cast<size_t>(1));
    EXPECT_DOUBLE_EQ(adjacents1.at(2), 5.0);

    const std::unordered_map<int, double>& adjacents3 = graph.getAdjacents(3);
    EXPECT_EQ(adjacents3.size(), static_cast<size_t>(2));
    EXPECT_DOUBLE_EQ(adjacents3.at(2), 1.0);
    EXPECT_DOUBLE_EQ(adjacents3.at(6), 2.0);
    EXPECT_DOUBLE_EQ(graph.getWeight(3, 5), -3.0);
}

TEST(WeightedSuperDigraphTest, Size) {
    spdlog::set_level(spdlog::level::debug);
    WeightedSuperDigraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    EXPECT_EQ(graph.size(), static_cast<size_t>(3));
}

TEST(WeightedSuperDigraphTest, ReadGraph) {
    spdlog::set_level(spdlog::level::debug);
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
    spdlog::set_level(spdlog::level::debug);
    WeightedSuperDigraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);
    graph.setNode(4);
    graph.setNode(5);
    graph.setNode(6);

    graph.setParent(1, 5);
    graph.setParent(2, 5);
    graph.setParent(3, 6);
    graph.setParent(4, 6);

    graph.addEdge(1, 2, 1.0);
    graph.addEdge(3, 4, 3.0);
    graph.addEdge(5, 6, 2.0);

    graph.writeGraph("../../dataset/output", FileExtension::TXT);
}