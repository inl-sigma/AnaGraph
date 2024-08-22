#include "similarity.hpp"

#include <gtest/gtest.h>

TEST(SimilarityTest, CosineSimilarity) {
    std::vector<double> v1 = {1.0, 2.0, 3.0};
    std::vector<double> v2 = {4.0, 5.0, 6.0};
    double result = similarity::cosineSimilarity(v1, v2);
    ASSERT_NEAR(result, 0.9746318461970762, 1e-9);
}

TEST(NDCGTest, NDCG) {
    std::vector<double> expected = {3.0, 2.0, 3.0, 0.0, 1.0, 2.0, 4.0, 5.0};
    std::vector<double> answer = {3.0, 2.0, 3.0, 0.0, 1.0, 2.0, 6.0, 7.0};
    double result = similarity::nDCG(expected, answer, 4);
    ASSERT_NEAR(result, 1.0, 1e-9);

    result = similarity::nDCG(expected, answer);
    ASSERT_NEAR(result, 1.0, 1e-9);
}