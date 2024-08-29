#include "anagraph/components/unweighted_graph.hpp"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <any>
#include <filesystem>
#include <string>

namespace {
    const std::string datasetDirectory = PROJECT_SOURCE_DIR + std::string("/dataset");
}

TEST(DigraphTest, GetNode) {
    using namespace anagraph::graph;
    Digraph graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    const Node& node1 = graph.getNode(0);
    EXPECT_EQ(node1.getId(), 0);
    const Node& node2 = graph.getNode(1);
    EXPECT_EQ(node2.getId(), 1);
    const Node& node3 = graph.getNode(2);
    EXPECT_EQ(node3.getId(), 2);
}

TEST(DigraphTest, SetNode) {
    using namespace anagraph::graph;
    Digraph graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    EXPECT_EQ(graph.size(), static_cast<size_t>(3));
}

TEST(DigraphTest, RemoveNode) {
    using namespace anagraph::graph;
    Digraph graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    graph.removeNode(1);
    EXPECT_EQ(graph.size(), static_cast<size_t>(2));
}

TEST(DigraphTest, GetIDs) {
    using namespace anagraph::graph;
    Digraph graph;
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

TEST(DigraphTest, AddEdge) {
    using namespace anagraph::graph;
    spdlog::set_level(spdlog::level::debug);

    Digraph graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    graph.addEdge(0, 1);
    graph.addEdge(1, 2);

    EXPECT_TRUE(graph.getAdjacents(0).contains(1));
    EXPECT_TRUE(graph.getAdjacents(1).contains(2));

    EXPECT_FALSE(graph.getAdjacents(0).contains(2));
    EXPECT_FALSE(graph.getAdjacents(1).contains(0));
}

TEST(DigraphTest, RemoveEdge) {
    using namespace anagraph::graph;
    Digraph graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    graph.addEdge(0, 1);
    graph.addEdge(1, 2);

    graph.removeEdge(0, 1);
    EXPECT_FALSE(graph.getAdjacents(0).contains(1));
    EXPECT_FALSE(graph.getAdjacents(1).contains(0));
}

TEST(DigraphTest, GetSubgraph) {
    using namespace anagraph::graph;
    spdlog::set_level(spdlog::level::debug);
    Digraph graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 3);

    std::unordered_set<int> indices = {0, 1, 3};
    spdlog::debug("create subgraph");
    Digraph subgraph = graph.getSubgraph(indices);

    spdlog::debug("calculate subgraph size");
    EXPECT_EQ(subgraph.size(), static_cast<size_t>(3));
    EXPECT_TRUE(subgraph.getAdjacents(0).contains(1));
    EXPECT_TRUE(subgraph.getAdjacents(1).contains(3));

    EXPECT_FALSE(subgraph.getAdjacents(0).contains(2));
    EXPECT_TRUE(graph.getAdjacents(0).contains(2));
}

TEST(DigraphTest, Reorganize) {
    using namespace anagraph::graph;
    spdlog::set_level(spdlog::level::debug);
    Digraph graph;
    graph.setNode(0);
    graph.setNode(2);
    graph.setNode(4);

    graph.addEdge(0, 2);
    graph.addEdge(2, 4);

    graph.reorganize();
    EXPECT_EQ(graph.size(), static_cast<size_t>(3));
    EXPECT_TRUE(graph.getAdjacents(0).contains(1));
    EXPECT_TRUE(graph.getAdjacents(1).contains(2));

    EXPECT_FALSE(graph.getAdjacents(0).contains(2));
    EXPECT_FALSE(graph.getAdjacents(2).contains(4));
}

TEST(DigraphTest, ReadGraph) {
    using namespace anagraph::graph;
    Digraph graph;
    // output current directory
    spdlog::info("current directory: {}", std::filesystem::current_path().string());
    const std::string inputPath = datasetDirectory + "/graph.txt";
    graph.readGraph(inputPath, anagraph::FileExtension::TXT);

    EXPECT_EQ(graph.size(), static_cast<size_t>(6));
    EXPECT_TRUE(graph.getAdjacents(0).contains(1));
    EXPECT_TRUE(graph.getAdjacents(0).contains(2));
    EXPECT_TRUE(graph.getAdjacents(1).contains(2));
    EXPECT_TRUE(graph.getAdjacents(2).contains(3));
    EXPECT_TRUE(graph.getAdjacents(2).contains(4));
    EXPECT_TRUE(graph.getAdjacents(4).contains(5));

    spdlog::set_level(spdlog::level::debug);
    EXPECT_FALSE(graph.getAdjacents(1).contains(0));
    EXPECT_THROW(graph.getNode(6), std::out_of_range);
}

TEST(DigraphTest, WriteGraph) {
    using namespace anagraph::graph;
    Digraph graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);
    graph.setNode(4);
    graph.setNode(5);

    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    graph.addEdge(2, 4);
    graph.addEdge(4, 5);

    const std::string outputPath = datasetDirectory + "/output/digraph_output.txt";
    graph.writeGraph(outputPath, anagraph::FileExtension::TXT);
}