#include "anagraph/components/weighted_graph.hpp"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <any>
#include <string>

namespace {
    const std::string datasetDirectory = PROJECT_SOURCE_DIR + std::string("/dataset");
}

TEST(WeightedGraphTest, GetNode) {
    using namespace anagraph;
    graph_structure::WeightedGraph graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    const graph_structure::WeightedNode& node1 = graph.getNode(0);
    EXPECT_EQ(node1.getId(), 0);
    const graph_structure::WeightedNode& node2 = graph.getNode(1);
    EXPECT_EQ(node2.getId(), 1);
    const graph_structure::WeightedNode& node3 = graph.getNode(2);
    EXPECT_EQ(node3.getId(), 2);
}

TEST(WeightedGraphTest, SetNode) {
    using namespace anagraph;
    graph_structure::WeightedGraph graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    EXPECT_EQ(graph.size(), static_cast<size_t>(3));
}

TEST(WeightedGraphTest, RemoveNode) {
    using namespace anagraph;
    graph_structure::WeightedGraph graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    graph.removeNode(1);
    EXPECT_EQ(graph.size(), static_cast<size_t>(2));
}

TEST(WeightedGraphTest, GetIDs) {
    using namespace anagraph;
    graph_structure::WeightedGraph graph;
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

TEST(WeightedGraphTest, SetEdge) {
    using namespace anagraph;
    spdlog::set_level(spdlog::level::debug);

    graph_structure::WeightedGraph graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    graph.setEdge(0, 1, 5.0);
    graph.setEdge(1, 2, 3.5);

    EXPECT_DOUBLE_EQ(graph.getWeight(0, 1), 5.0);
    EXPECT_DOUBLE_EQ(graph.getWeight(1, 0), 5.0);
    EXPECT_DOUBLE_EQ(graph.getWeight(1, 2), 3.5);
    EXPECT_DOUBLE_EQ(graph.getWeight(2, 1), 3.5);
}

TEST(WeightedGraphTest, RemoveEdge) {
    using namespace anagraph;
    graph_structure::WeightedGraph graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    graph.setEdge(0, 1, 5.0);
    graph.setEdge(1, 2, 3.5);

    graph.removeEdge(0, 1);
    EXPECT_DOUBLE_EQ(graph.getWeight(0, 1), 0.0);
    EXPECT_DOUBLE_EQ(graph.getWeight(1, 0), 0.0);
}

TEST(WeightedGraphTest, GetAdjacents) {
    using namespace anagraph;
    spdlog::set_level(spdlog::level::debug);
    graph_structure::WeightedGraph graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    graph.setEdge(0, 1, 5.0);
    graph.setEdge(0, 2, 2.5);

    const std::unordered_map<int, double>& adjacents = graph.getAdjacents(0);
    for (const auto& [id, weight] : adjacents) {
        spdlog::debug("id: {}, weight: {}", id, weight);
    }
    EXPECT_EQ(adjacents.size(), static_cast<size_t>(2));
    EXPECT_DOUBLE_EQ(adjacents.at(1), 5.0);
    EXPECT_DOUBLE_EQ(adjacents.at(2), 2.5);
}

TEST(WeightedGraphTest, GetSubgraph) {
    using namespace anagraph;
    spdlog::set_level(spdlog::level::debug);
    graph_structure::WeightedGraph graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    graph.setEdge(0, 1, 5.0);
    graph.setEdge(0, 2, 2.5);
    graph.setEdge(1, 3, 3.0);

    std::unordered_set<int> indices = {0, 1, 3};
    spdlog::debug("create subgraph");
    graph_structure::WeightedGraph subgraph = graph.getSubgraph(indices);

    spdlog::debug("calculate subgraph size");
    EXPECT_EQ(subgraph.size(), static_cast<size_t>(3));
    EXPECT_DOUBLE_EQ(subgraph.getWeight(0, 1), 5.0);
    EXPECT_DOUBLE_EQ(subgraph.getWeight(1, 0), 5.0);
    EXPECT_DOUBLE_EQ(subgraph.getWeight(1, 3), 3.0);
    EXPECT_DOUBLE_EQ(subgraph.getWeight(3, 1), 3.0);

    EXPECT_THROW(subgraph.getWeight(0, 2), std::out_of_range);
}

TEST(WeightedGraphTest, Reorganize) {
    using namespace anagraph;
    spdlog::set_level(spdlog::level::debug);
    graph_structure::WeightedGraph graph;
    graph.setNode(0);
    graph.setNode(2);
    graph.setNode(4);

    graph.setEdge(0, 2, 5.0);
    graph.setEdge(2, 4, 3.5);

    graph.reorganize();
    EXPECT_EQ(graph.size(), static_cast<size_t>(3));
    EXPECT_DOUBLE_EQ(graph.getWeight(0, 1), 5.0);
    EXPECT_DOUBLE_EQ(graph.getWeight(1, 0), 5.0);
    EXPECT_DOUBLE_EQ(graph.getWeight(1, 2), 3.5);
    EXPECT_DOUBLE_EQ(graph.getWeight(2, 1), 3.5);

    EXPECT_DOUBLE_EQ(graph.getWeight(0, 2), 0.0);
    EXPECT_THROW(graph.getWeight(2, 4), std::out_of_range);
}

TEST(WeightedGraphTest, toDigraph) {
    using namespace anagraph;
    graph_structure::WeightedGraph graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    graph.setEdge(0, 1, 5.0);
    graph.setEdge(0, 2, 2.5);
    graph.setEdge(1, 3, 3.0);

    graph_structure::WeightedDigraph digraph = graph.toDigraph();
    EXPECT_EQ(digraph.size(), static_cast<size_t>(4));
    EXPECT_DOUBLE_EQ(digraph.getWeight(0, 1), 5.0);
    EXPECT_DOUBLE_EQ(digraph.getWeight(1, 0), 5.0);
    EXPECT_DOUBLE_EQ(digraph.getWeight(0, 2), 2.5);
    EXPECT_DOUBLE_EQ(digraph.getWeight(2, 0), 2.5);
    EXPECT_DOUBLE_EQ(digraph.getWeight(1, 3), 3.0);
    EXPECT_DOUBLE_EQ(digraph.getWeight(3, 1), 3.0);
}

TEST(WeightedGraphTest, ReadGraph) {
    using namespace anagraph;
    spdlog::set_level(spdlog::level::debug);
    graph_structure::WeightedGraph graph;
    const std::string inputPath = datasetDirectory + "/graph.txt";
    graph.readGraph(inputPath, FileExtension::TXT);

    EXPECT_EQ(graph.size(), static_cast<size_t>(6));
    EXPECT_EQ(graph.getWeight(0, 1), 1.0);
    EXPECT_EQ(graph.getWeight(0, 2), 1.0);
    EXPECT_EQ(graph.getWeight(1, 2), 1.0);
    EXPECT_EQ(graph.getWeight(2, 3), 1.0);
    EXPECT_EQ(graph.getWeight(2, 4), 1.0);
    EXPECT_EQ(graph.getWeight(4, 5), 1.0);

    EXPECT_EQ(graph.getWeight(1, 0), 1.0);
    EXPECT_EQ(graph.getWeight(2, 0), 1.0);
    EXPECT_EQ(graph.getWeight(2, 1), 1.0);
    EXPECT_EQ(graph.getWeight(3, 2), 1.0);
    EXPECT_EQ(graph.getWeight(4, 2), 1.0);
    EXPECT_EQ(graph.getWeight(5, 4), 1.0);

    EXPECT_THROW(graph.getWeight(0, 6), std::out_of_range);
    EXPECT_THROW(graph.getNode(6), std::out_of_range);
}

TEST(WeightedGraphTest, WriteGraph) {
    using namespace anagraph;
    graph_structure::WeightedGraph graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);
    graph.setNode(4);
    graph.setNode(5);

    graph.setEdge(0, 1, 1.0);
    graph.setEdge(0, 2, 2.0);
    graph.setEdge(1, 2, 3.0);
    graph.setEdge(2, 3, 1.5);
    graph.setEdge(2, 4, 2.5);
    graph.setEdge(4, 5, 0.5);

    const std::string outputPath = datasetDirectory + "/output/weighted_graph_output.txt";
    graph.writeGraph(outputPath, FileExtension::TXT);
}