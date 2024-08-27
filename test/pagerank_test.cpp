#include "anagraph/algorithms/pagerank.hpp"

#include "anagraph/components/weighted_graph.hpp"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <numeric>

namespace {
    // PageRank values with error 1e-6, computed with the directed graph
    const std::vector<double> directedPageRank = {
        0.246448254546578800,
        0.087705946870459460,
        0.074923430367349500,
        0.027793521975148276,
        0.025201150462922830,
        0.031549587475305740,
        0.021286383043794738,
        0.014937946031411825,
        0.024077700774917940,
        0.015684820974045107,
        0.014937946031411825,
        0.014937946031411825,
        0.014937946031411825,
        0.015684820974045107,
        0.016896792037740412,
        0.016896792037740412,
        0.014937946031411825,
        0.014937946031411825,
        0.016896792037740412,
        0.015684820974045107,
        0.016896792037740412,
        0.014937946031411825,
        0.016896792037740412,
        0.036395954990569810,
        0.030846654615652464,
        0.018528400400000590,
        0.022865729711222638,
        0.015684820974045107,
        0.019275275342633870,
        0.016896792037740412,
        0.016896792037740412,
        0.016896792037740412,
        0.015684820974045107,
        0.014937946031411825
    };

    const std::vector<double> undirectedPageRank = {
        0.097001817589837060,
        0.052878391037427000,
        0.057078423047636730,
        0.035860643223064786,
        0.021979406974834498,
        0.029113341663442210,
        0.029113341663442210,
        0.024490758039509180,
        0.029765339186167030,
        0.014308950284462801,
        0.021979406974834498,
        0.009564916863537146,
        0.014645186487916188,
        0.029536314977202980,
        0.014535161524273827,
        0.014535161524273827,
        0.016785378110253487,
        0.014558859774243490,
        0.014535161524273827,
        0.019604416711937293,
        0.014535161524273827,
        0.014558859774243490,
        0.014535161524273827,
        0.031520915311632280,
        0.021075455001162945,
        0.021005628174745790,
        0.015043395360629756,
        0.025638803528350497,
        0.019572960509438540,
        0.026287262837112083,
        0.024589336534292478,
        0.037156635922679410,
        0.071692130065882880,
        0.100917916748712140
    };
}

TEST(PageRankTest, TestDirectedPageRank) {
    // Create a graph
    WeightedDigraph digraph("../../dataset/zackary_karate.txt", FileExtension::TXT);

    spdlog::set_level(spdlog::level::debug);

    // Compute the PageRank
    std::vector<double> pr = pageRank(digraph, 0.15, 1000000);

    for (size_t i = 0; i < digraph.size(); i++) {
        EXPECT_NEAR(pr[i], directedPageRank[i], 1e-3);
    }
}

TEST(PageRankTest, WeightedDigraphForwardPushTest) {
    WeightedDigraph digraph("../../dataset/zackary_karate.txt", FileExtension::TXT);
    spdlog::set_level(spdlog::level::debug);

    // Compute the PageRank
    const int size = digraph.size();
    std::vector<double> source(size, 1.0);
    auto [pr, _] = forwardPush(digraph, source, 0.15, 1e-5);

    for (size_t i = 0; i < digraph.size(); i++) {
        EXPECT_NEAR(pr[i], directedPageRank[i], 1e-5);
    }
}

TEST(PageRankTest, WeightedGraphForwardPushTest) {
    WeightedGraph graph("../../dataset/zackary_karate.txt", FileExtension::TXT);
    spdlog::set_level(spdlog::level::debug);

    // Compute the PageRank
    const int size = graph.size();
    std::vector<double> source(size, 1.0);
    auto [pr, _] = forwardPush(graph, source, 0.15, 1e-5);

    for (size_t i = 0; i < graph.size(); i++) {
        EXPECT_NEAR(pr[i], undirectedPageRank[i], 1e-5);
    }
}

TEST(PageRankTest, DigraphForwardPushTest) {
    Digraph digraph("../../dataset/zackary_karate.txt", FileExtension::TXT);
    spdlog::set_level(spdlog::level::debug);

    // Compute the PageRank
    const int size = digraph.size();
    std::vector<double> source(size, 1.0);
    auto [pr, _] = forwardPush(digraph, source, 0.15, 1e-5);

    for (size_t i = 0; i < digraph.size(); i++) {
        EXPECT_NEAR(pr[i], directedPageRank[i], 1e-5);
    }
}

TEST(PageRankTest, GraphForwardPushTest) {
    Graph graph("../../dataset/zackary_karate.txt", FileExtension::TXT);
    spdlog::set_level(spdlog::level::debug);

    // Compute the PageRank
    const int size = graph.size();
    std::vector<double> source(size, 1.0);
    auto [pr, _] = forwardPush(graph, source, 0.15, 1e-5);

    for (size_t i = 0; i < graph.size(); i++) {
        EXPECT_NEAR(pr[i], undirectedPageRank[i], 1e-5);
    }
}

TEST(PageRankTest, WeightedDigraphForaTest) {
    WeightedDigraph digraph("../../dataset/zackary_karate.txt", FileExtension::TXT);
    spdlog::set_level(spdlog::level::debug);

    // Compute the PageRank
    const int size = digraph.size();
    std::vector<double> source(size, 1.0 / size);
    auto pr = fora(digraph, source, 0.15, 1e-4);
    double prSum = std::reduce(pr.begin(), pr.end());
    spdlog::debug("Sum of PageRank: {}", prSum);

    for (size_t i = 0; i < digraph.size(); i++) {
        EXPECT_NEAR(pr[i], directedPageRank[i], pr[i]*1e-4);
    }
}

TEST(PageRankTest, WeightedGraphForaTest) {
    WeightedGraph graph("../../dataset/zackary_karate.txt", FileExtension::TXT);
    spdlog::set_level(spdlog::level::debug);

    // Compute the PageRank
    const int size = graph.size();
    std::vector<double> source(size, 1.0 / size);
    auto pr = fora(graph, source, 0.15, 1e-4);
    double prSum = std::reduce(pr.begin(), pr.end());
    spdlog::debug("Sum of PageRank: {}", prSum);

    for (size_t i = 0; i < graph.size(); i++) {
        EXPECT_NEAR(pr[i], undirectedPageRank[i], pr[i]*1e-4);
    }
}

TEST(PageRankTest, DigraphForaTest) {
    Digraph digraph("../../dataset/zackary_karate.txt", FileExtension::TXT);
    spdlog::set_level(spdlog::level::debug);

    // Compute the PageRank
    const int size = digraph.size();
    std::vector<double> source(size, 1.0 / size);
    auto pr = fora(digraph, source, 0.15, 1e-4);
    double prSum = std::reduce(pr.begin(), pr.end());
    spdlog::debug("Sum of PageRank: {}", prSum);

    for (size_t i = 0; i < digraph.size(); i++) {
        EXPECT_NEAR(pr[i], directedPageRank[i], pr[i]*1e-4);
    }
}

TEST(PageRankTest, GraphForaTest) {
    Graph graph("../../dataset/zackary_karate.txt", FileExtension::TXT);
    spdlog::set_level(spdlog::level::debug);

    // Compute the PageRank
    const int size = graph.size();
    std::vector<double> source(size, 1.0 / size);
    auto pr = fora(graph, source, 0.15, 1e-4);
    double prSum = std::reduce(pr.begin(), pr.end());
    spdlog::debug("Sum of PageRank: {}", prSum);

    for (size_t i = 0; i < graph.size(); i++) {
        EXPECT_NEAR(pr[i], undirectedPageRank[i], pr[i]*1e-4);
    }
}