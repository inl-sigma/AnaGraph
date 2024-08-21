#pragma once

#ifndef PAGERANK_HPP
#define PAGERANK_HPP

#include "graph_interface.hpp"
#include "heterogeneous_graph_if.hpp"

#include <vector>

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
 * @brief Compute the single source personalized PageRank of a graph
 * 
 * @param graph The graph to compute the PageRank
 * @param source The source node
 * @param alpha The damping factor
 * @param epsilon The error tolerance ratio
 * 
 * @return The PageRank of the nodes
 * 
 * @note The graph must be connected, and has sequential node ids
 */
std::vector<double> singleSourcePersonalizedPageRank(const IWeightedDigraph &graph, const int source, const double alpha, const double epsilon);


/**
 * Calculates the forward push algorithm for PageRank.
 *
 * This function takes an input weighted graph, a source vector, an alpha value, and an epsilon value.
 * It performs the forward push algorithm to calculate the PageRank scores for each node in the graph.
 *
 * @param graph The input weighted graph.
 * @param source The source vector containing the initial scores for each node.
 * @param alpha The damping factor for the algorithm.
 * @param thr The convergence threshold for the algorithm.
 * @return A tuple containing two vectors: the final PageRank scores and the residual errors.
 */
std::tuple<std::vector<double>, std::vector<double>> forwardPush(const IWeightedDigraph &graph, const std::vector<double> source, const double alpha, const double thr);

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