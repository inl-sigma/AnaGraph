#include "anagraph/components/weighted_supergraph.hpp"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

namespace {
    const std::string datasetDirectory = PROJECT_SOURCE_DIR + std::string("/dataset");
}

TEST(WeightedSupergraphTest, SetNode) {
    using namespace anagraph::graph_structure;
    spdlog::set_level(spdlog::level::debug);
    WeightedSupergraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(4);

    EXPECT_EQ(graph.size(), static_cast<size_t>(3));
}

TEST(WeightedSupergraphTest, GetNode) {
    using namespace anagraph::graph_structure;
    spdlog::set_level(spdlog::level::debug);
    WeightedSupergraph graph;
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

TEST(WeightedSupergraphTest, RemoveNode) {
    using namespace anagraph::graph_structure;
    spdlog::set_level(spdlog::level::debug);
    WeightedSupergraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    graph.removeNode(2);

    EXPECT_EQ(graph.size(), static_cast<size_t>(2));

    WeightedSupernode node1 = graph.getNode(1);
    EXPECT_EQ(node1.getId(), 1);
}

TEST(WeightedSupergraphTest, GetIds) {
    using namespace anagraph::graph_structure;
    spdlog::set_level(spdlog::level::debug);
    WeightedSupergraph graph;
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

TEST(WeightedSupergraphTest, SetEdge) {
    using namespace anagraph::graph_structure;
    spdlog::set_level(spdlog::level::debug);
    WeightedSupergraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    graph.setEdge(1, 2, 5.0);
    graph.setEdge(2, 3, 3.0);

    EXPECT_EQ(graph.getWeight(1, 2), 5.0);
    EXPECT_EQ(graph.getWeight(2, 1), 5.0);
    EXPECT_EQ(graph.getWeight(2, 3), 3.0);
    EXPECT_EQ(graph.getWeight(3, 2), 3.0);
}

TEST(WeightedSupergraphTest, RemoveEdge) {
    using namespace anagraph::graph_structure;
    spdlog::set_level(spdlog::level::debug);
    WeightedSupergraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    graph.setEdge(1, 2, 5.0);
    graph.setEdge(2, 3, 3.0);

    graph.removeEdge(1, 2);

    EXPECT_DOUBLE_EQ(graph.getWeight(1, 2), 0.0);
    EXPECT_DOUBLE_EQ(graph.getWeight(2, 1), 0.0);
    EXPECT_DOUBLE_EQ(graph.getWeight(2, 3), 3.0);
    EXPECT_DOUBLE_EQ(graph.getWeight(3, 2), 3.0);
}

TEST(WeightedSupergraphTest, GetWeight) {
    using namespace anagraph::graph_structure;
    spdlog::set_level(spdlog::level::debug);
    WeightedSupergraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    graph.setEdge(1, 2, 5.0);
    graph.setEdge(2, 3, 3.0);

    EXPECT_DOUBLE_EQ(graph.getWeight(1, 2), 5.0);
    EXPECT_DOUBLE_EQ(graph.getWeight(2, 1), 5.0);
    EXPECT_DOUBLE_EQ(graph.getWeight(2, 3), 3.0);
    EXPECT_DOUBLE_EQ(graph.getWeight(3, 2), 3.0);
}

TEST(WeightedSupergraphTest, SetWeight) {
    using namespace anagraph::graph_structure;
    spdlog::set_level(spdlog::level::debug);
    WeightedSupergraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    graph.setEdge(1, 2, 5.0);
    graph.setEdge(2, 3, 3.0);

    graph.setWeight(1, 2, 10.0);
    graph.setWeight(2, 3, 7.0);

    EXPECT_DOUBLE_EQ(graph.getWeight(1, 2), 10.0);
    EXPECT_DOUBLE_EQ(graph.getWeight(2, 1), 10.0);
    EXPECT_DOUBLE_EQ(graph.getWeight(2, 3), 7.0);
    EXPECT_DOUBLE_EQ(graph.getWeight(3, 2), 7.0);
}

TEST(WeightedSupergraphTest, AddWeight) {
    using namespace anagraph::graph_structure;
    spdlog::set_level(spdlog::level::debug);
    WeightedSupergraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    graph.setEdge(1, 2, 5.0);
    graph.setEdge(2, 3, 3.0);

    graph.addWeight(1, 2, 2.0);
    graph.addWeight(2, 3, -1.0);

    EXPECT_DOUBLE_EQ(graph.getWeight(1, 2), 7.0);
    EXPECT_DOUBLE_EQ(graph.getWeight(2, 1), 7.0);
    EXPECT_DOUBLE_EQ(graph.getWeight(2, 3), 2.0);
    EXPECT_DOUBLE_EQ(graph.getWeight(3, 2), 2.0);
}

TEST(WeightedSupergraphTest, GetAndSetParent) {
    using namespace anagraph::graph_structure;
    spdlog::set_level(spdlog::level::debug);
    WeightedSupergraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    graph.setParent(2, 1);
    graph.setParent(3, 2);

    EXPECT_EQ(graph.getParent(2), 1);
    EXPECT_EQ(graph.getParent(3), 2);

    EXPECT_EQ(graph.getParent(1), WeightedSupernode::ROOT);
}

TEST(WeightedSupergraphTest, UpdateParent) {
    using namespace anagraph::graph_structure;
    spdlog::set_level(spdlog::level::debug);
    WeightedSupergraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    graph.setParent(2, 1);
    graph.setParent(3, 2);
    EXPECT_EQ(graph.getParent(3), 2);

    graph.updateParent(3, 1);
    EXPECT_EQ(graph.getParent(3), 1);
}

TEST(WeightedSupergraphTest, RemoveParent) {
    using namespace anagraph::graph_structure;
    spdlog::set_level(spdlog::level::debug);
    WeightedSupergraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    graph.setParent(2, 1);
    graph.setParent(3, 2);

    graph.removeParent(3);

    EXPECT_EQ(graph.getParent(3), WeightedSupernode::ROOT);
}

TEST(WeightedSupergraphTest, GetChildren) {
    using namespace anagraph::graph_structure;
    spdlog::set_level(spdlog::level::debug);
    WeightedSupergraph graph;
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

TEST(WeightedSupergraphTest, GetAdjacents) {
    using namespace anagraph::graph_structure;
    spdlog::set_level(spdlog::level::debug);
    WeightedSupergraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);
    graph.setNode(4);
    graph.setNode(5);
    graph.setNode(6);
    graph.setNode(7);

    graph.setParent(1, 5);
    graph.setParent(2, 5);
    graph.setParent(3, 6);
    graph.setParent(4, 6);
    graph.setParent(5, 7);
    graph.setParent(6, 7);

    graph.setEdge(1, 2, 1.0);
    graph.setEdge(2, 3, -1.0);
    graph.setEdge(2, 4, 1.5);
    graph.setEdge(3, 5, -2.5);
    graph.setEdge(4, 5, 2.5);
    graph.setEdge(5, 6, 2.0);
    graph.setEdge(6, 6, 1.0);

    const std::unordered_map<int, double>& adjacents1 = graph.getAdjacents(1);
    EXPECT_EQ(adjacents1.size(), static_cast<size_t>(1));
    EXPECT_DOUBLE_EQ(adjacents1.at(2), 1.0);

    const std::unordered_map<int, double>& adjacents2 = graph.getAdjacents(2);
    EXPECT_EQ(adjacents2.size(), static_cast<size_t>(3));
    EXPECT_DOUBLE_EQ(adjacents2.at(1), 1.0);
    EXPECT_DOUBLE_EQ(adjacents2.at(3), -1.0);
    EXPECT_DOUBLE_EQ(adjacents2.at(4), 1.5);

    const std::unordered_map<int, double>& adjacents3 = graph.getAdjacents(3);
    EXPECT_EQ(adjacents3.size(), static_cast<size_t>(2));
    EXPECT_DOUBLE_EQ(adjacents3.at(2), -1.0);
    EXPECT_DOUBLE_EQ(adjacents3.at(5), -2.5);

    const std::unordered_map<int, double>& adjacents4 = graph.getAdjacents(4);
    EXPECT_EQ(adjacents4.size(), static_cast<size_t>(2));
    EXPECT_DOUBLE_EQ(adjacents4.at(2), 1.5);
    EXPECT_DOUBLE_EQ(adjacents4.at(5), 2.5);

    const std::unordered_map<int, double>& adjacents5 = graph.getAdjacents(5);
    EXPECT_EQ(adjacents5.size(), static_cast<size_t>(3));
    EXPECT_DOUBLE_EQ(adjacents5.at(3), -2.5);
    EXPECT_DOUBLE_EQ(adjacents5.at(4), 2.5);
    EXPECT_DOUBLE_EQ(adjacents5.at(6), 2.0);

    const std::unordered_map<int, double>& adjacents6 = graph.getAdjacents(6);
    EXPECT_EQ(adjacents6.size(), static_cast<size_t>(2));
    EXPECT_DOUBLE_EQ(adjacents6.at(5), 2.0);
    EXPECT_DOUBLE_EQ(adjacents6.at(6), 1.0);

    const std::unordered_map<int, double>& adjacents7 = graph.getAdjacents(7);
    EXPECT_EQ(adjacents7.size(), static_cast<size_t>(0));
}

TEST(WeightedSupergraphTest, Merge) {
    using namespace anagraph::graph_structure;
    spdlog::set_level(spdlog::level::debug);
    WeightedSupergraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);
    graph.setNode(4);
    graph.setNode(5);

    graph.setEdge(1, 3, 1.0);
    graph.setEdge(1, 5, 2.0);
    graph.setEdge(2, 4, 1.5);
    graph.setEdge(2, 5, 1.0);

    graph.mergeNode(1, 2, [](WeightedSupernode &first, WeightedSupernode &second) {
        WeightedSupernode node;
        int newId = node.getId();
        auto &adjacents1 = first.getAdjacentNodes();
        auto &adjacents2 = second.getAdjacentNodes();
        spdlog::debug("adjacents1.size() = {}", adjacents1.size());
        spdlog::debug("adjacents2.size() = {}", adjacents2.size());
        for (const auto &[id, adjNodeRef] : adjacents1) {
            spdlog::debug("adjacents1: id = {}", id);
            auto &adjNode = adjNodeRef.get();
            const double weight = adjNode.getWeight(first.getId());
            spdlog::debug("weight = {}", weight);
            node.setAdjacentNode(adjNode, weight);
            adjNode.setAdjacentNode(node, weight);
        }
        for (const auto &[id, adjNodeRef] : adjacents2) {
            spdlog::debug("adjacents2: id = {}", id);
            auto &adjNode = adjNodeRef.get();
            const double weight = adjNode.getWeight(second.getId());
            spdlog::debug("weight = {}", weight);
            node.updateAdjacentNode(adjNode, weight);
            adjNode.updateAdjacentNode(node, weight);
        }
        first.setParent(newId);
        second.setParent(newId);
        spdlog::info("finish merge");
        for (auto &adj : node.getAdjacents()) {
            spdlog::debug("adj = {}, weight= {}", adj.first, adj.second);
        }
        return node;
    });

    EXPECT_EQ(graph.size(), static_cast<size_t>(6));
    EXPECT_EQ(graph.getParent(1), 6);
    EXPECT_EQ(graph.getParent(2), 6);
    EXPECT_EQ(graph.getWeight(6, 3), 1.0);
    EXPECT_EQ(graph.getWeight(3, 6), 1.0);
    EXPECT_EQ(graph.getWeight(6, 4), 1.5);
    EXPECT_EQ(graph.getWeight(4, 6), 1.5);
    EXPECT_EQ(graph.getWeight(6, 5), 3.0);
    EXPECT_EQ(graph.getWeight(5, 6), 3.0);
}

TEST(WeightedSupergraphTest, setMergeNodeFunction) {
    using namespace anagraph::graph_structure;
    WeightedSupergraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);
    graph.setNode(4);
    graph.setNode(5);

    graph.setEdge(1, 3, 1.0);
    graph.setEdge(1, 5, 2.0);
    graph.setEdge(2, 4, 1.5);
    graph.setEdge(2, 5, 1.0);

    EXPECT_THROW(graph.mergeNode(1, 2), std::bad_function_call);

    graph.setMergeNodeFunction([](WeightedSupernode &first, WeightedSupernode &second) {
        WeightedSupernode node;
        auto &adjacents1 = first.getAdjacentNodes();
        auto &adjacents2 = second.getAdjacentNodes();
        spdlog::debug("adjacents1.size() = {}", adjacents1.size());
        spdlog::debug("adjacents2.size() = {}", adjacents2.size());
        for (const auto &[id, adjNodeRef] : adjacents1) {
            spdlog::debug("adjacents1: id = {}", id);
            auto &adjNode = adjNodeRef.get();
            const double weight = adjNode.getWeight(first.getId());
            node.setAdjacentNode(adjNode, weight);
            adjNode.setAdjacentNode(node, weight);
        }
        for (const auto &[id, adjNodeRef] : adjacents2) {
            spdlog::debug("adjacents2: id = {}", id);
            auto &adjNode = adjNodeRef.get();
            const double weight = adjNode.getWeight(second.getId());
            node.updateAdjacentNode(adjNode, weight);
            adjNode.updateAdjacentNode(node, weight);
        }
        spdlog::info("finish merge");
        for (auto &adj : node.getAdjacents()) {
            spdlog::debug("adj = {}, weight= {}", adj.first, adj.second);
        }
        return node;
    });

    graph.mergeNode(1, 2);
    EXPECT_EQ(graph.size(), static_cast<size_t>(6));
    EXPECT_EQ(graph.getWeight(6, 3), 1.0);
    EXPECT_EQ(graph.getWeight(3, 6), 1.0);
    EXPECT_EQ(graph.getWeight(6, 4), 1.5);
    EXPECT_EQ(graph.getWeight(4, 6), 1.5);
    EXPECT_EQ(graph.getWeight(6, 5), 3.0);
    EXPECT_EQ(graph.getWeight(5, 6), 3.0);
}

TEST(WeightedSupergraphTest, Size) {
    using namespace anagraph::graph_structure;
    spdlog::set_level(spdlog::level::debug);
    WeightedSupergraph graph;
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    EXPECT_EQ(graph.size(), static_cast<size_t>(3));
}

TEST(WeightedSupergraphTest, toDigraph) {
    using namespace anagraph::graph_structure;
    WeightedSupergraph graph;
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

    graph.setEdge(1, 2, 1.0);
    graph.setEdge(2, 3, 1.5);
    graph.setEdge(2, 4, 2.0);
    graph.setEdge(5, 6, 2.5);

    WeightedSuperDigraph digraph = graph.toDigraph();
    EXPECT_EQ(digraph.size(), static_cast<size_t>(6));
    EXPECT_DOUBLE_EQ(digraph.getWeight(1, 2), 1.0);
    EXPECT_DOUBLE_EQ(digraph.getWeight(2, 1), 1.0);
    EXPECT_DOUBLE_EQ(digraph.getWeight(2, 3), 1.5);
    EXPECT_DOUBLE_EQ(digraph.getWeight(3, 2), 1.5);
    EXPECT_DOUBLE_EQ(digraph.getWeight(2, 4), 2.0);
    EXPECT_DOUBLE_EQ(digraph.getWeight(4, 2), 2.0);
    EXPECT_DOUBLE_EQ(digraph.getWeight(5, 6), 2.5);
    EXPECT_DOUBLE_EQ(digraph.getWeight(6, 5), 2.5);
}

TEST(WeightedSupergraphTest, ReadGraph) {
    using namespace anagraph::graph_structure;
    spdlog::set_level(spdlog::level::debug);
    WeightedSupergraph graph;
    const std::string inputPath = datasetDirectory + "/input";
    graph.readGraph(inputPath, anagraph::FileExtension::TXT);

    EXPECT_EQ(graph.size(), static_cast<size_t>(6));
    EXPECT_EQ(graph.getWeight(0, 1), 1.0);
    EXPECT_EQ(graph.getWeight(0, 2), 1.0);
    EXPECT_EQ(graph.getWeight(1, 2), 1.0);
    EXPECT_EQ(graph.getWeight(2, 3), 1.0);
    EXPECT_EQ(graph.getWeight(2, 4), 1.0);
    EXPECT_EQ(graph.getWeight(5, 4), 1.0);

    EXPECT_EQ(graph.getWeight(1, 0), 1.0);
    EXPECT_EQ(graph.getWeight(2, 0), 1.0);
    EXPECT_EQ(graph.getWeight(2, 1), 1.0);
    EXPECT_EQ(graph.getWeight(3, 2), 1.0);
    EXPECT_EQ(graph.getWeight(4, 2), 1.0);
    EXPECT_EQ(graph.getWeight(4, 5), 1.0);

    EXPECT_EQ(graph.getParent(0), WeightedSupernode::ROOT);
    EXPECT_EQ(graph.getParent(1), 5);
    EXPECT_EQ(graph.getParent(2), WeightedSupernode::ROOT);
    EXPECT_EQ(graph.getParent(3), WeightedSupernode::ROOT);
    EXPECT_EQ(graph.getParent(4), WeightedSupernode::ROOT);
    EXPECT_EQ(graph.getParent(5), WeightedSupernode::ROOT);
}

TEST(WeightedSupergraphTest, WriteGraph) {
    using namespace anagraph::graph_structure;
    spdlog::set_level(spdlog::level::debug);
    WeightedSupergraph graph;
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

    graph.setEdge(1, 2, 1.0);
    graph.setEdge(3, 4, 3.0);
    graph.setEdge(5, 6, 2.0);

    const std::string outputDir = datasetDirectory + "/output/undirected_supergraph_test";
    graph.writeGraph(outputDir, anagraph::FileExtension::TXT);
}

TEST(WeightedSupergraphTest, GraphIterator) {
    using namespace anagraph::graph_structure;
    WeightedSupergraph graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setNode(2);
    graph.setNode(3);

    int nodeId = 0;
    for (auto &node : graph) {
        EXPECT_EQ(node.getId(), nodeId);
        nodeId++;
    }
}