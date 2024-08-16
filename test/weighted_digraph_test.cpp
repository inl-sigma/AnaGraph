#include "weighted_graph.hpp"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <any>
#include <string>

TEST(WeightedDigraphTest, GetNode) {
    WeightedDigraph graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    const WeightedNode& node1 = graph.getNode(0);
    EXPECT_EQ(node1.getId(), 0);
    const WeightedNode& node2 = graph.getNode(1);
    EXPECT_EQ(node2.getId(), 1);
    const WeightedNode& node3 = graph.getNode(2);
    EXPECT_EQ(node3.getId(), 2);
}

TEST(WeightedDigraphTest, SetNode) {
    WeightedDigraph graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    EXPECT_EQ(graph.size(), 3);
}

TEST(WeightedDigraphTest, RemoveNode) {
    WeightedDigraph graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    graph.removeNode(1);
    EXPECT_EQ(graph.size(), 2);
}

TEST(WeightedDigraphTest, AddEdge) {
    spdlog::set_level(spdlog::level::debug);

    WeightedDigraph graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    graph.addEdge(0, 1, 5.0);
    graph.addEdge(1, 2, 3.5);

    EXPECT_DOUBLE_EQ(graph.getWeight(0, 1), 5.0);
    EXPECT_DOUBLE_EQ(graph.getWeight(1, 2), 3.5);
    
    EXPECT_DOUBLE_EQ(graph.getWeight(1, 0), 0.0);    
    EXPECT_DOUBLE_EQ(graph.getWeight(0, 2), 0.0);
}

TEST(WeightedDigraphTest, RemoveEdge) {
    WeightedDigraph graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    graph.addEdge(0, 1, 5.0);
    graph.addEdge(1, 2, 3.5);

    graph.removeEdge(0, 1);
    EXPECT_DOUBLE_EQ(graph.getWeight(0, 1), 0.0);

    EXPECT_DOUBLE_EQ(graph.getWeight(1, 2), 3.5);
}

TEST(WeightedDigraphTest, GetAdjacents) {
    spdlog::set_level(spdlog::level::debug);
    WeightedDigraph graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    graph.addEdge(0, 1, 5.0);
    graph.addEdge(0, 2, 2.5);

    const std::unordered_map<int, double>& adjacents = graph.getAdjacents(0);
    for (const auto& [id, weight] : adjacents) {
        spdlog::debug("id: {}, weight: {}", id, weight);
    }
    EXPECT_EQ(adjacents.size(), 2);
    EXPECT_DOUBLE_EQ(adjacents.at(1), 5.0);
    EXPECT_DOUBLE_EQ(adjacents.at(2), 2.5);
}

TEST(WeightedDigraphTest, GetSubgraph) {
    spdlog::set_level(spdlog::level::debug);
    WeightedDigraph graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    graph.addEdge(0, 1, 5.0);
    graph.addEdge(0, 2, 2.5);
    graph.addEdge(1, 3, 3.0);

    std::unordered_set<int> indices = {0, 1, 3};
    spdlog::debug("create subgraph");
    WeightedDigraph subgraph = graph.getSubgraph(indices);

    spdlog::debug("calculate subgraph size");
    EXPECT_EQ(subgraph.size(), 3);
    EXPECT_DOUBLE_EQ(subgraph.getWeight(0, 1), 5.0);
    EXPECT_DOUBLE_EQ(subgraph.getWeight(1, 3), 3.0);

    EXPECT_THROW(subgraph.getWeight(0, 2), std::out_of_range);
}

TEST(WeightedDigraphTest, Organize) {
    spdlog::set_level(spdlog::level::debug);
    WeightedDigraph graph;
    graph.setNode(0);
    graph.setNode(2);
    graph.setNode(4);

    graph.addEdge(0, 2, 5.0);
    graph.addEdge(2, 4, 3.5);

    graph.organize();
    EXPECT_EQ(graph.size(), 3);
    EXPECT_DOUBLE_EQ(graph.getWeight(0, 1), 5.0);
    EXPECT_DOUBLE_EQ(graph.getWeight(1, 2), 3.5);

    EXPECT_DOUBLE_EQ(graph.getWeight(0, 2), 0.0);
    EXPECT_THROW(graph.getWeight(2, 4), std::out_of_range);
}

TEST(WeightedDigraphTest, ReadGraph) {
    WeightedDigraph graph;
    graph.readGraph("../../dataset/graph.txt", FileExtension::TXT);

    EXPECT_EQ(graph.size(), 6);
    EXPECT_EQ(graph.getWeight(0, 1), 1.0);
    EXPECT_EQ(graph.getWeight(0, 2), 1.0);
    EXPECT_EQ(graph.getWeight(1, 2), 1.0);
    EXPECT_EQ(graph.getWeight(2, 3), 1.0);
    EXPECT_EQ(graph.getWeight(2, 4), 1.0);
    EXPECT_EQ(graph.getWeight(4, 5), 1.0);

    EXPECT_DOUBLE_EQ(graph.getWeight(1, 0), 0.0);
    EXPECT_THROW(graph.getWeight(0, 6), std::out_of_range);
}

TEST(WeightedDigraphTest, WriteGraph) {
    WeightedDigraph graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);
    graph.setNode(4);
    graph.setNode(5);

    graph.addEdge(0, 1, 1.0);
    graph.addEdge(0, 2, 2.0);
    graph.addEdge(1, 2, 3.0);
    graph.addEdge(2, 3, 1.5);
    graph.addEdge(2, 4, 2.5);
    graph.addEdge(4, 5, 0.5);

    graph.writeGraph("../../dataset/graph_output.txt", FileExtension::TXT);
}