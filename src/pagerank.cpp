#include "pagerank.hpp"

#include <spdlog/spdlog.h>

#include <map>
#include <random>
#include <unordered_map>
#include <vector>

std::vector<double> pageRank(const IWeightedDigraph &graph, const double alpha, const int iter) {
    spdlog::debug("called pageRank with alpha = {} and iter = {}", alpha, iter);
    const int size = graph.size();
    std::vector<double> pr(size, 0); /**< the vector meaning the pagerank */

    spdlog::debug("initialize the outEdgesTable");
    std::vector<std::map<double, int>> outEdgesTable(size, std::map<double, int>()); /**< the outedges of each node */
    for (int src = 0; src < size; src++) {
        auto &outEdgesMap = outEdgesTable[src];

        const std::unordered_map<int, double> &adjacents = graph.getAdjacents(src);
        double weightSum = 0;
        for (const auto &[_, weight] : adjacents) {
            weightSum += weight;
        }

        // cumulative probability
        double cumWeight = 0;
        for (const auto &[dst, weight] : adjacents) {
            cumWeight += weight;
            const double cumProbabilty = alpha + (1 - alpha) * cumWeight / weightSum;
            outEdgesMap[cumProbabilty] = dst; // add upper bound of the interval
            spdlog::debug("outEdgesTable[{}][{}] = {}", src, cumProbabilty, dst);
        }
    }

    spdlog::debug("start random walk to compute the pagerank");
    // random walk
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> intDis(0, size - 1); // right inclusive
    std::uniform_real_distribution<double> realDis(0, 1); // right exclusive
    for (int i = 0; i < iter; i++) {
        int currentNode = intDis(gen);

        while (true) {
            const double rand = realDis(gen);
            if (rand < alpha) {
                break;
            }

            std::map<double, int> &outEdges = outEdgesTable[currentNode];
            if (outEdges.empty()) {
                currentNode = intDis(gen);
            } else {
                std::map<double, int>::iterator it = outEdges.lower_bound(rand);
                currentNode = it->second;
            }
        }
        pr[currentNode] += 1;
    }

    for (int i = 0; i < size; i++) {
        pr[i] /= iter;
    }
    return pr;
}

std::vector<double> pageRank(const IWeightedDigraph &graph) {
    return pageRank(graph, 0.15, 1000000);
}