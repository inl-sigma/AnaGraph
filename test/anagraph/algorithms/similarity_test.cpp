#include "anagraph/algorithms/similarity.hpp"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

TEST(SimilarityTest, CosineSimilarity) {
    using namespace anagraph;
    std::vector<double> v1 = {1.0, 2.0, 3.0};
    std::vector<double> v2 = {4.0, 5.0, 6.0};
    double result = similarity::cosineSimilarity(v1, v2);
    ASSERT_NEAR(result, 0.9746318461970762, 1e-9);
}

TEST(SimilarityTest, NDCG) {
    using namespace anagraph;
    std::vector<double> expected = {3.0, 2.0, 3.0, 0.0, 1.0, 2.0, 4.0, 5.0};
    std::vector<double> answer = {3.0, 2.0, 3.0, 0.0, 1.0, 2.0, 6.0, 7.0};
    double result = similarity::nDCG(expected, answer, 4);
    ASSERT_NEAR(result, 1.0, 1e-9);

    result = similarity::nDCG(expected, answer);
    ASSERT_NEAR(result, 1.0, 1e-9);
}

TEST(SimilarityTest, KLdivergence) {
    using namespace anagraph;
    std::vector<double> p = {0.1, 0.2, 0.7};
    std::vector<double> q = {0.2, 0.3, 0.5};
    double result = similarity::KLdivergence(p, q);
    ASSERT_NEAR(result, 0.12280627887493795, 1e-9);
}

TEST(SimilarityTest, JSdivergence) {
    using namespace anagraph;
    std::vector<double> p = {0.1, 0.2, 0.7};
    std::vector<double> q = {0.2, 0.3, 0.5};
    double result = similarity::JSdivergence(p, q);
    ASSERT_NEAR(result, 0.031596722287467766, 1e-9);
}

TEST(SimilarityTest, DirectedAccuracy) {
    using namespace anagraph;
    graph_structure::Digraph expected;
    expected.setEdge(1, 2);
    expected.setEdge(2, 3);
    expected.setEdge(3, 1);

    graph_structure::Digraph answer;
    answer.setEdge(1, 2);
    answer.setEdge(2, 3);
    answer.setEdge(3, 1);

    double result = similarity::accuracy(expected, answer);
    ASSERT_DOUBLE_EQ(result, 1.0);

    answer.removeEdge(2, 3);
    result = similarity::accuracy(expected, answer);
    ASSERT_DOUBLE_EQ(result, 5.0/6);
}

TEST(SimilarityTest, UndirectedAccuracy) {
    using namespace anagraph;
    graph_structure::Graph expected;
    expected.setEdge(1, 2);
    expected.setEdge(2, 3);
    expected.setEdge(3, 4);

    graph_structure::Graph answer;
    answer.setEdge(1, 2);
    answer.setEdge(2, 3);
    answer.setEdge(3, 4);

    double result = similarity::accuracy(expected, answer);
    ASSERT_DOUBLE_EQ(result, 1.0);

    answer.removeEdge(2, 3);
    result = similarity::accuracy(expected, answer);
    ASSERT_DOUBLE_EQ(result, 5.0/6);
}


TEST(SimilarityTest, DirectedPrecision) {
    using namespace anagraph;
    graph_structure::Digraph expected;
    expected.setEdge(1, 2);
    expected.setEdge(2, 3);
    expected.setEdge(3, 1);

    graph_structure::Digraph answer;
    answer.setEdge(1, 2);
    answer.setEdge(2, 3);
    answer.setEdge(3, 1);

    double result = similarity::precision(expected, answer);
    ASSERT_DOUBLE_EQ(result, 1.0);

    answer.removeEdge(2, 3);
    result = similarity::precision(expected, answer);
    ASSERT_DOUBLE_EQ(result, 2.0/3);
}

TEST(SimilarityTest, UndirectedPrecision) {
    using namespace anagraph;
    graph_structure::Graph expected;
    expected.setEdge(1, 2);
    expected.setEdge(2, 3);
    expected.setEdge(3, 4);

    graph_structure::Graph answer;
    answer.setEdge(1, 2);
    answer.setEdge(2, 3);
    answer.setEdge(3, 4);

    double result = similarity::precision(expected, answer);
    ASSERT_DOUBLE_EQ(result, 1.0);

    answer.removeEdge(2, 3);
    result = similarity::precision(expected, answer);
    ASSERT_DOUBLE_EQ(result, 2.0/3);
}

TEST(SimilarityTest, DirectedRecall) {
    using namespace anagraph;
    graph_structure::Digraph expected;
    expected.setEdge(1, 2);
    expected.setEdge(2, 3);
    expected.setEdge(3, 1);

    graph_structure::Digraph answer;
    answer.setEdge(1, 2);
    answer.setEdge(2, 3);
    answer.setEdge(3, 1);

    double result = similarity::recall(expected, answer);
    ASSERT_DOUBLE_EQ(result, 1.0);

    answer.removeEdge(2, 3);
    result = similarity::recall(expected, answer);
    ASSERT_DOUBLE_EQ(result, 1.0);
}

TEST(SimilarityTest, UndirectedRecall) {
    using namespace anagraph;
    spdlog::set_level(spdlog::level::debug);
    graph_structure::Graph expected;
    expected.setEdge(1, 2);
    expected.setEdge(2, 3);
    expected.setEdge(3, 4);

    graph_structure::Graph answer;
    answer.setEdge(1, 2);
    answer.setEdge(2, 3);
    answer.setEdge(3, 4);

    double result = similarity::recall(expected, answer);
    ASSERT_DOUBLE_EQ(result, 1.0);

    answer.removeEdge(2, 3);
    result = similarity::recall(expected, answer);
    ASSERT_DOUBLE_EQ(result, 1.0);
}

TEST(SimilarityTest, DirectedFMeasure) {
    using namespace anagraph;
    graph_structure::Digraph expected;
    expected.setEdge(1, 2);
    expected.setEdge(2, 3);
    expected.setEdge(3, 1);

    graph_structure::Digraph answer;
    answer.setEdge(1, 2);
    answer.setEdge(2, 3);
    answer.setEdge(3, 1);

    double result = similarity::fMeasure(expected, answer);
    ASSERT_DOUBLE_EQ(result, 1.0);

    answer.removeEdge(2, 3);
    result = similarity::fMeasure(expected, answer);
    ASSERT_DOUBLE_EQ(result, 0.8);
}

TEST(SimilarityTest, UndirectedFMeasure) {
    using namespace anagraph;
    graph_structure::Graph expected;
    expected.setEdge(1, 2);
    expected.setEdge(2, 3);
    expected.setEdge(3, 4);

    graph_structure::Graph answer;
    answer.setEdge(1, 2);
    answer.setEdge(2, 3);
    answer.setEdge(3, 4);

    double result = similarity::fMeasure(expected, answer);
    ASSERT_DOUBLE_EQ(result, 1.0);

    answer.removeEdge(2, 3);
    result = similarity::fMeasure(expected, answer);
    ASSERT_DOUBLE_EQ(result, 0.8);
}