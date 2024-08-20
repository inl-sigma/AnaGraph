#pragma once

#ifndef PAGERANK_HPP
#define PAGERANK_HPP

#include <vector>

#include "graph_interface.hpp"
#include "heterogeneous_graph_if.hpp"

/**
 * @brief Compute the PageRank of a graph
 * 
 * @param graph The graph to compute the PageRank
 * @param alpha The damping factor
 * @param iter The number of iterations
 * 
 * @return The PageRank of the nodes
 * 
 * @note The graph must be connected, and has sequential node ids
 */
std::vector<double> pageRank(const IWeightedDigraph &graph, const double alpha, const int iter);

/**
 * @brief Compute the PageRank of a graph
 * 
 * @param graph The graph to compute the PageRank
 * @param alpha The damping factor, default is 0.15
 * @param iter The maximum number of iterations, default is 100000
 * 
 * @return The PageRank of the nodes
 * 
 * @note The graph must be connected, and has sequential node ids
 */
std::vector<double> pageRank(const IWeightedDigraph &graph /**, const double alpha = 0.15, int iter = 100000 */);

/**
 * @brief Compute the PageRank of a graph
 * 
 * @param graph The graph to compute the PageRank
 * @param alpha The damping factor
 * @param iter The maximum number of iterations
 * 
 * @return The PageRank of the nodes
 * 
 * @note The graph must be connected, and has sequential node ids
 */
std::vector<double> pageRank(const IWeightedGraph &graph, const double alpha = 0.15, int iter = 100000);

#endif // PAGERANK_HPP