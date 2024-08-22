#include "similarity.hpp"

#include <gtest/gtest.h>

TEST(SimilarityTest, CosineSimilarity) {
    std::vector<double> v1 = {1.0, 2.0, 3.0};
    std::vector<double> v2 = {4.0, 5.0, 6.0};
    double result = similarity::cosineSimilarity(v1, v2);
    ASSERT_NEAR(result, 0.9746318461970762, 1e-9);
}