#include "anagraph/algorithms/graph_hashes.hpp"

#include <gtest/gtest.h>

TEST(ShingleTest, EmptyGraph) {
    anagraph::graph_structure::Graph graph;
    auto result = anagraph::hashes::minHash(graph);
    EXPECT_TRUE(result.empty());
}

TEST(ShingleTest, SingleNodeGraph) {
    anagraph::graph_structure::Graph graph;
    graph.setNode(0);
    auto result = anagraph::hashes::minHash(graph);
    ASSERT_EQ(result.size(), static_cast<size_t>(1));
    EXPECT_EQ(result[0].size(), static_cast<size_t>(1));
    EXPECT_EQ(result[0][0], 0);
}

TEST(ShingleTest, TwoNodeGraph) {
    anagraph::graph_structure::Graph graph;
    graph.setNode(0);
    graph.setNode(1);
    graph.setEdge(0, 1);
    auto result = anagraph::hashes::minHash(graph);
    for (const auto& group : result) {
        std::string nodes;
        for (const auto& node : group) {
            nodes += std::to_string(node) + " ";
        }
        spdlog::debug("{}", nodes);
    }
    ASSERT_EQ(result.size(), static_cast<size_t>(1));
    EXPECT_EQ(result[0].size(), static_cast<size_t>(2));
    EXPECT_EQ(result[0][0], 0);
    EXPECT_EQ(result[0][1], 1);
}

TEST(ShingleTest, MultipleNodesGraph) {
    anagraph::graph_structure::Graph graph;
    spdlog::set_level(spdlog::level::debug);
    for (int i = 0; i < 10; i++) {
        graph.setNode(i);
    }
    for (int i = 0; i < 9; i++) {
        graph.setEdge(i, i + 1); // path graph
    }
    auto result = anagraph::hashes::minHash(graph);
    int total_nodes = 0;
    for (const auto& group : result) {
        std::string nodes;
        for (const auto& node : group) {
            nodes += std::to_string(node) + " ";
        }
        spdlog::debug("{}", nodes);
        total_nodes += group.size();
    }
    EXPECT_EQ(total_nodes, 10);
}