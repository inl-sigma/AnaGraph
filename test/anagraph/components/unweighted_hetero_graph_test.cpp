#include "anagraph/components/unweighted_hetero_graph.hpp"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <any>
#include <string>
#include <unordered_set>

namespace {
    const std::string datasetDirectory = PROJECT_SOURCE_DIR + std::string("/dataset");
}

TEST(HeteroGraphTest, GetNode) {
    using namespace anagraph;
    graph_structure::HeteroGraph<int> graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    const graph_structure::HeteroNode<int>& node1 = graph.getNode(0);
    EXPECT_EQ(node1.getId(), 0);
    const graph_structure::HeteroNode<int>& node2 = graph.getNode(1);
    EXPECT_EQ(node2.getId(), 1);
    const graph_structure::HeteroNode<int>& node3 = graph.getNode(2);
    EXPECT_EQ(node3.getId(), 2);
}

TEST(HeteroGraphTest, SetNode) {
    using namespace anagraph;
    graph_structure::HeteroGraph<int> graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    EXPECT_EQ(graph.size(), static_cast<size_t>(3));
}

TEST(HeteroGraphTest, RemoveNode) {
    using namespace anagraph;
    graph_structure::HeteroGraph<int> graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    graph.removeNode(1);
    EXPECT_EQ(graph.size(), static_cast<size_t>(2));
}

TEST(HeteroGraphTest, GetIDs) {
    using namespace anagraph;
    graph_structure::HeteroGraph<int> graph;
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

TEST(HeteroGraphTest, GetAdjacents) {
    using namespace anagraph;
    spdlog::set_level(spdlog::level::debug);
    graph_structure::HeteroGraph<int> graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    graph.setEdge(0, 1);
    graph.setEdge(0, 2);

    const std::unordered_set<int>& adjacents = graph.getAdjacents(0);
    EXPECT_EQ(adjacents.size(), static_cast<size_t>(2));
    EXPECT_TRUE(adjacents.contains(1));
    EXPECT_TRUE(adjacents.contains(2));

    EXPECT_TRUE(graph.getAdjacents(1).contains(0));
    EXPECT_TRUE(graph.getAdjacents(2).contains(0));
}

TEST(HeteroGraphTest, SetEdge) {
    using namespace anagraph;
    spdlog::set_level(spdlog::level::debug);

    graph_structure::HeteroGraph<int> graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    graph.setEdge(0, 1);
    graph.setEdge(1, 2);

    auto node0 = graph.getNode(0);
    auto node1 = graph.getNode(1);
    auto adjacents0 = node0.getAdjacents();
    auto adjacents1 = node1.getAdjacents();

    EXPECT_TRUE(adjacents0.contains(1));
    EXPECT_FALSE(adjacents0.contains(2));

    EXPECT_TRUE(adjacents1.contains(0));
    EXPECT_TRUE(adjacents1.contains(2));
}

TEST(HeteroGraphTest, RemoveEdge) {
    using namespace anagraph;
    graph_structure::HeteroGraph<int> graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);

    graph.setEdge(0, 1);
    graph.setEdge(1, 2);
    graph.removeEdge(0, 1);

    EXPECT_FALSE(graph.getAdjacents(0).contains(1));
    EXPECT_FALSE(graph.getAdjacents(1).contains(0));
    EXPECT_TRUE(graph.getAdjacents(1).contains(2));
}

TEST(HeteroGraphTest, GetSubgraph) {
    using namespace anagraph;
    spdlog::set_level(spdlog::level::debug);
    graph_structure::HeteroGraph<int> graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    graph.setEdge(0, 1);
    graph.setEdge(0, 2);
    graph.setEdge(1, 3);

    std::unordered_set<int> indices = {0, 1, 3};
    spdlog::debug("create subgraph");
    graph_structure::HeteroGraph<int> subgraph = graph.getSubgraph(indices);

    spdlog::debug("calculate subgraph size");
    EXPECT_EQ(subgraph.size(), static_cast<size_t>(3));
    EXPECT_TRUE(subgraph.getAdjacents(0).contains(1));
    EXPECT_TRUE(subgraph.getAdjacents(1).contains(0));
    EXPECT_TRUE(subgraph.getAdjacents(1).contains(3));
    EXPECT_TRUE(subgraph.getAdjacents(3).contains(1));

    EXPECT_FALSE(subgraph.getAdjacents(0).contains(2));
}

TEST(HeteroGraphTest, Reorganize) {
    using namespace anagraph;
    spdlog::set_level(spdlog::level::debug);
    graph_structure::HeteroGraph<int> graph;
    graph.setNode(0);
    graph.setNode(2);
    graph.setNode(4);

    graph.setEdge(0, 2);
    graph.setEdge(2, 4);

    graph.reorganize();
    EXPECT_EQ(graph.size(), static_cast<size_t>(3));
    EXPECT_TRUE(graph.getAdjacents(0).contains(1));
    EXPECT_TRUE(graph.getAdjacents(1).contains(0));
    EXPECT_TRUE(graph.getAdjacents(1).contains(2));
    EXPECT_TRUE(graph.getAdjacents(2).contains(1));

    EXPECT_FALSE(graph.getAdjacents(0).contains(2));
    EXPECT_FALSE(graph.getAdjacents(2).contains(4));
}

TEST(HeteroGraphTest, GetAttributes) {
    using namespace anagraph;
    graph_structure::HeteroGraph<std::string> graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    graph.setAttributes(0, "Node 0");
    graph.setAttributes(1, "Node 1");
    graph.setAttributes(2, "Node 2");

    std::string attributes = graph.getAttributes(1);
    EXPECT_EQ(attributes, "Node 1");

    EXPECT_THROW(graph.getAttributes(3), std::runtime_error);
}

TEST(HeteroGraphTest, SetAttributes) {
    using namespace anagraph;
    graph_structure::HeteroGraph<std::string> graph;
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

TEST(HeteroGraphTest, AnyAttributes) {
    using namespace anagraph;
    spdlog::set_level(spdlog::level::debug);
    graph_structure::HeteroGraph<std::any> graph;
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

TEST(HeteroGraphTest, ReadGraph) {
    using namespace anagraph;
    graph_structure::HeteroGraph<int> graph;
    const std::string inputPath = datasetDirectory + "/graph.txt";
    graph.readGraph(inputPath, FileExtension::TXT);

    EXPECT_EQ(graph.size(), static_cast<size_t>(6));
    EXPECT_TRUE(graph.getAdjacents(0).contains(1));
    EXPECT_TRUE(graph.getAdjacents(0).contains(2));
    EXPECT_TRUE(graph.getAdjacents(1).contains(2));
    EXPECT_TRUE(graph.getAdjacents(2).contains(3));
    EXPECT_TRUE(graph.getAdjacents(2).contains(4));
    EXPECT_TRUE(graph.getAdjacents(4).contains(5));

    EXPECT_TRUE(graph.getAdjacents(1).contains(0));
    EXPECT_TRUE(graph.getAdjacents(2).contains(0));
    EXPECT_TRUE(graph.getAdjacents(2).contains(1));
    EXPECT_TRUE(graph.getAdjacents(3).contains(2));
    EXPECT_TRUE(graph.getAdjacents(4).contains(2));
    EXPECT_TRUE(graph.getAdjacents(5).contains(4));
}

TEST(HeteroGraphTest, WriteGraph) {
    using namespace anagraph;
    graph_structure::HeteroGraph<int> graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);
    graph.setNode(4);
    graph.setNode(5);

    graph.setEdge(0, 1);
    graph.setEdge(0, 2);
    graph.setEdge(1, 2);
    graph.setEdge(2, 3);
    graph.setEdge(2, 4);
    graph.setEdge(4, 5);

    const std::string outputPath = datasetDirectory + "/output/unweighted_hetero_graph_output.txt";
    graph.writeGraph(outputPath, FileExtension::TXT);
}