#include "heterogeneous_graph.hpp"
#include "graph_utils.hpp"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <any>
#include <string>
#include <unordered_set>

TEST(WeightedHeteroDigraphTest, GetNode) {
    WeightedHeteroDigraph<int> graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    const WeightedHeteroNode<int>& node1 = graph.getNode(0);
    EXPECT_EQ(node1.getId(), 0);
    const WeightedHeteroNode<int>& node2 = graph.getNode(1);
    EXPECT_EQ(node2.getId(), 1);
    const WeightedHeteroNode<int>& node3 = graph.getNode(2);
    EXPECT_EQ(node3.getId(), 2);
}

TEST(WeightedHeteroDigraphTest, SetNode) {
    WeightedHeteroDigraph<int> graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    EXPECT_EQ(graph.size(), static_cast<size_t>(3));
}

TEST(WeightedHeteroDigraphTest, RemoveNode) {
    WeightedHeteroDigraph<int> graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    graph.removeNode(1);
    EXPECT_EQ(graph.size(), static_cast<size_t>(2));
}

TEST(WeightedHeteroDigraphTest, GetIDs) {
    WeightedHeteroDigraph<int> graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(5);

    std::unordered_set<int> ids = graph.getIds();
    EXPECT_EQ(ids.size(), static_cast<size_t>(4));
    EXPECT_TRUE(ids.contains(0));
    EXPECT_TRUE(ids.contains(1));
    EXPECT_TRUE(ids.contains(2));
    EXPECT_TRUE(ids.contains(5));

    EXPECT_FALSE(ids.contains(-1));
    EXPECT_FALSE(ids.contains(3));
    EXPECT_FALSE(ids.contains(7));
}

TEST(WeightedHeteroDigraphTest, AddEdge) {
    spdlog::set_level(spdlog::level::debug);

    WeightedHeteroDigraph<int> graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    graph.addEdge(0, 1, 5.0);
    graph.addEdge(1, 2, 3.5);

    EXPECT_DOUBLE_EQ(graph.getWeight(0, 1), 5.0);
    EXPECT_DOUBLE_EQ(graph.getWeight(1, 2), 3.5);

    EXPECT_DOUBLE_EQ(graph.getWeight(1, 0), 0.0);
    EXPECT_DOUBLE_EQ(graph.getWeight(2, 1), 0.0);
}

TEST(WeightedHeteroDigraphTest, RemoveEdge) {
    WeightedHeteroDigraph<int> graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    graph.addEdge(0, 1, 5.0);
    graph.addEdge(1, 2, 3.5);

    graph.removeEdge(0, 1);
    EXPECT_DOUBLE_EQ(graph.getWeight(0, 1), 0.0);
}

TEST(WeightedHeteroDigraphTest, GetAdjacents) {
    spdlog::set_level(spdlog::level::debug);
    WeightedHeteroDigraph<int> graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    graph.addEdge(0, 1, 5.0);
    graph.addEdge(0, 2, 2.5);

    const std::unordered_map<int, double>& adjacents = graph.getAdjacents(0);
    for (const auto& [id, weight] : adjacents) {
        spdlog::debug("id: {}, weight: {}", id, weight);
    }
    EXPECT_EQ(adjacents.size(), static_cast<size_t>(2));
    EXPECT_DOUBLE_EQ(adjacents.at(1), 5.0);
    EXPECT_DOUBLE_EQ(adjacents.at(2), 2.5);
}

TEST(WeightedHeteroDigraphTest, GetSubgraph) {
    spdlog::set_level(spdlog::level::debug);
    WeightedHeteroDigraph<int> graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    graph.addEdge(0, 1, 5.0);
    graph.addEdge(0, 2, 2.5);
    graph.addEdge(1, 3, 3.0);

    std::unordered_set<int> indices = {0, 1, 3};
    spdlog::debug("create subgraph");
    WeightedHeteroDigraph<int> subgraph = graph.getSubgraph(indices);

    spdlog::debug("calculate subgraph size");
    EXPECT_EQ(subgraph.size(), static_cast<size_t>(3));
    EXPECT_DOUBLE_EQ(subgraph.getWeight(0, 1), 5.0);
    EXPECT_DOUBLE_EQ(subgraph.getWeight(1, 3), 3.0);

    EXPECT_DOUBLE_EQ(subgraph.getWeight(0, 2), 0.0);
}

TEST(WeightedHeteroDigraphTest, Reorganize) {
    spdlog::set_level(spdlog::level::debug);
    WeightedHeteroDigraph<int> graph;
    graph.setNode(0);
    graph.setNode(2);
    graph.setNode(4);

    graph.addEdge(0, 2, 5.0);
    graph.addEdge(2, 4, 3.5);

    graph.reorganize();
    EXPECT_EQ(graph.size(), static_cast<size_t>(3));
    EXPECT_DOUBLE_EQ(graph.getWeight(0, 1), 5.0);
    EXPECT_DOUBLE_EQ(graph.getWeight(1, 2), 3.5);

    EXPECT_DOUBLE_EQ(graph.getWeight(0, 2), 0.0);
    EXPECT_THROW(graph.getWeight(2, 4), std::out_of_range);
}

TEST(WeightedHeteroDigraphTest, GetAttributes) {
    WeightedHeteroDigraph<std::string> graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    graph.setAttributes(0, "Node 0");
    graph.setAttributes(1, "Node 1");
    graph.setAttributes(2, "Node 2");

    std::string attributes = graph.getAttributes(1);
    EXPECT_EQ(attributes, "Node 1");
}

TEST(WeightedHeteroDigraphTest, SetAttributes) {
    WeightedHeteroDigraph<std::string> graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    graph.setAttributes(0, "Node 0");
    graph.setAttributes(1, "Node 1");
    graph.setAttributes(2, "Node 2");

    graph.setAttributes(1, "Updated Node 1");
    std::string attributes = graph.getAttributes(1);
    EXPECT_EQ(attributes, "Updated Node 1");
}

TEST(WeightedHeteroDigraphTest, AnyAttributes) {
    spdlog::set_level(spdlog::level::debug);
    WeightedHeteroDigraph<std::any> graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    graph.setAttributes(0, 1);
    graph.setAttributes(1, 2.0);
    graph.setAttributes(2, std::string("Node 2")); // to avoid casting to char[ ]

    spdlog::debug("get int attributes");
    int intAttributes = std::any_cast<int>(graph.getAttributes(0));
    EXPECT_EQ(intAttributes, 1);

    spdlog::debug("get double attributes");
    double doubleAttributes = std::any_cast<double>(graph.getAttributes(1));
    EXPECT_DOUBLE_EQ(doubleAttributes, 2.0);

    spdlog::debug("get string attributes");
    std::string stringAttributes = std::any_cast<std::string>(graph.getAttributes(2));
    EXPECT_EQ(stringAttributes, "Node 2");
}

TEST(WeightedHeteroDigraphTest, ReadGraph) {
    WeightedHeteroDigraph<int> graph;
    graph.readGraph("../../dataset/graph.txt", FileExtension::TXT);

    EXPECT_EQ(graph.size(), static_cast<size_t>(6));
    EXPECT_EQ(graph.getWeight(0, 1), 1.0);
    EXPECT_EQ(graph.getWeight(0, 2), 1.0);
    EXPECT_EQ(graph.getWeight(1, 2), 1.0);
    EXPECT_EQ(graph.getWeight(2, 3), 1.0);
    EXPECT_EQ(graph.getWeight(2, 4), 1.0);
    EXPECT_EQ(graph.getWeight(4, 5), 1.0);
}

TEST(WeightedHeteroDigraphTest, WriteGraph) {
    WeightedHeteroDigraph<int> graph;
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