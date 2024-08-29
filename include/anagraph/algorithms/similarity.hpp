#pragma once

#ifndef SIMILARITY_HPP
#define SIMILARITY_HPP

#include "anagraph/components/unweighted_digraph.hpp"
#include "anagraph/components/unweighted_graph.hpp"

#include <vector>

namespace anagraph {
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

    /**
     * Calculates the Kullback-Leibler divergence between two vectors.
     *
     * @param p The first vector.
     * @param q The second vector.
     * @return The Kullback-Leibler divergence between p and q.
     */
    double KLdivergence(const std::vector<double>& p, const std::vector<double>& q);

    /**
     * Calculates the Jensen-Shannon divergence between two vectors.
     *
     * @param p The first vector.
     * @param q The second vector.
     * @return The Jensen-Shannon divergence between p and q.
     */
    double JSdivergence(const std::vector<double>& p, const std::vector<double>& q);

    /**
     * Calculates the accuracy of a given answer compared to the expected result.
     *
     * @param expected The expected Digraph.
     * @param answer The answer Digraph to be evaluated.
     * @return The accuracy of the answer as a double value.
     */
    double accuracy(const graph::Digraph &expected, const graph::Digraph &answer);

    /**
     * Calculates the accuracy of a given answer compared to the expected result.
     *
     * @param expected The expected graph.
     * @param answer The answer graph to be evaluated.
     * @return The accuracy of the answer as a double value.
     */
    double accuracy(const graph::Graph &expected, const graph::Graph &answer);

    /**
     * Calculates the precision of a given expected digraph and answer digraph.
     *
     * @param expected The expected digraph.
     * @param answer The answer digraph.
     * @return The precision value.
     */
    double precision(const graph::Digraph &expected, const graph::Digraph &answer);

    /**
     * Calculates the precision of a given expected graph and answer graph.
     *
     * @param expected The expected graph.
     * @param answer The answer graph.
     * @return The precision value.
     */
    double precision(const graph::Graph &expected, const graph::Graph &answer);

    /**
     * Calculates the recall value between two directed graphs.
     *
     * @param expected The expected directed graph.
     * @param answer The actual directed graph.
     * @return The recall value between the two graphs.
     */
    double recall(const graph::Digraph &expected, const graph::Digraph &answer);

    /**
     * Calculates the recall value between two graphs.
     *
     * @param expected The expected graph.
     * @param answer The actual graph.
     * @return The recall value between the two graphs.
     */
    double recall(const graph::Graph &expected, const graph::Graph &answer);

    /**
     * Calculates the F-measure between the expected and answer digraphs.
     *
     * @param expected The expected digraph.
     * @param answer The answer digraph.
     * @return The F-measure value.
     */
    double fMeasure(const graph::Digraph &expected, const graph::Digraph &answer);

    /**
     * Calculates the F-measure between the expected and answer graphs.
     *
     * @param expected The expected graph.
     * @param answer The answer graph.
     * @return The F-measure value.
     */
    double fMeasure(const graph::Graph &expected, const graph::Graph &answer);

} // namespace similarity
} // namespace anagraph

#endif // SIMILARITY_HPP