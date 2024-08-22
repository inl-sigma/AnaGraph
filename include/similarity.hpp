#pragma once

#ifndef SIMILARITY_HPP
#define SIMILARITY_HPP

#include <vector>

namespace similarity {
    /**
     * Calculates the cosine similarity between two vectors.
     *
     * @param v1 The first vector.
     * @param v2 The second vector.
     * @return The cosine similarity between v1 and v2.
     */
    double cosineSimilarity(const std::vector<double>& v1, const std::vector<double>& v2);

    /**
     * Calculates the normalized Discounted Cumulative Gain (nDCG) for a given set of expected and answer values.
     * 
     * The nDCG is calculated by the difference of ranking between the expected and answer values.
     *
     * @param expected The vector of expected values.
     * @param answer The vector of answer values.
     * @param k The number of elements to consider in the calculation.
     * @return The nDCG value.
     * 
     * @note The vectors must have the same size. The value of k must be less than or equal to the size of the vectors.
     */
    double nDCG(const std::vector<double>& expected, const std::vector<double>& answer, int k);

    /**
     * Calculates the normalized Discounted Cumulative Gain (nDCG) for a given set of expected and answer values.
     * 
     * The nDCG is calculated by the difference of ranking between the expected and answer values.
     *
     * @param expected The vector of expected values.
     * @param answer The vector of answer values.
     * @return The nDCG value.
     * 
     * @note The vectors must have the same size.
     */
    double nDCG(const std::vector<double>& expected, const std::vector<double>& answer);

} // namespace similarity

#endif // SIMILARITY_HPP