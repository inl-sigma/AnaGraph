#pragma once

#ifndef GRAPH_HASHES_HPP
#define GRAPH_HASHES_HPP

#include "anagraph/components/weighted_digraph.hpp"
#include "anagraph/components/weighted_graph.hpp"
#include "anagraph/components/unweighted_digraph.hpp"
#include "anagraph/components/unweighted_graph.hpp"

#include <vector>

namespace anagraph {
namespace hashes {


/**
 * @brief Computes the MinHash signatures for the given directed graph.
 * 
 * This function takes a directed graph as input and returns a vector of vectors,
 * where each inner vector represents the MinHash signature of a node in the graph.
 * MinHash is a technique used to estimate the similarity between sets.
 * 
 * @param graph The directed graph for which MinHash signatures are to be computed.
 * @return A vector of vectors containing the MinHash signatures for each node in the graph.
 * 
 * @note The graph must be connected, and has sequential node ids.
 */
std::vector<std::vector<int>> minHash(const graph_structure::Digraph &graph);

/**
 * @brief Computes the MinHash signatures for the given directed graph.
 * 
 * This function takes a directed graph as input and returns a vector of vectors,
 * where each inner vector represents the MinHash signature of a node in the graph.
 * MinHash is a technique used to estimate the similarity between sets.
 * 
 * @param graph The directed graph for which MinHash signatures are to be computed.
 * @return A vector of vectors containing the MinHash signatures for each node in the graph.
 * 
 * @note The graph must be connected, and has sequential node ids.
 */
std::vector<std::vector<int>> minHash(const graph_structure::Graph &graph);

/**
 * @brief Computes the MinHash signatures for the given directed graph.
 * 
 * This function takes a directed graph as input and returns a vector of vectors,
 * where each inner vector represents the MinHash signature of a node in the graph.
 * MinHash is a technique used to estimate the similarity between sets.
 * 
 * @param graph The directed graph for which MinHash signatures are to be computed.
 * @return A vector of vectors containing the MinHash signatures for each node in the graph.
 * 
 * @note The graph must be connected, and has sequential node ids.
 */
std::vector<std::vector<int>> minHash(const graph_structure::WeightedDigraph &graph);

/**
 * @brief Computes the MinHash signatures for the given directed graph.
 * 
 * This function takes a directed graph as input and returns a vector of vectors,
 * where each inner vector represents the MinHash signature of a node in the graph.
 * MinHash is a technique used to estimate the similarity between sets.
 * 
 * @param graph The directed graph for which MinHash signatures are to be computed.
 * @return A vector of vectors containing the MinHash signatures for each node in the graph.
 * 
 * @note The graph must be connected, and has sequential node ids.
 */
std::vector<std::vector<int>> minHash(const graph_structure::WeightedGraph &graph);


} // namespace hashes
} // namespace anagraph

#endif // GRAPH_HASHES_HPP