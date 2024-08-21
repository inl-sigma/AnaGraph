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

std::tuple<std::vector<double>, std::vector<double>> forwardPush(const IWeightedDigraph &graph, const std::vector<double> source, const double alpha, const double thr) {
    const int size = graph.size();
    std::vector<double> ppr(size, 0); /**< the vector meaning the reserve/approximated PPR */
    std::vector<double> residue = source; /**< the vector meaning the residue */

    // initialize/normalize the source vector
    const double sourceSum = std::reduce(source.begin(), source.end());
    if (sourceSum == 0) {
        throw std::invalid_argument("The source vector must have at least one non-zero element");
    }
    std::unordered_map<int, double> sourceMap;
    for (int i = 0; i < size; i++) {
        if (source[i] > 0) {
            sourceMap[i] = source[i] / sourceSum;
        }
    }

    std::vector<double> outWeightSum(size, 0); /**< the vector meaning the sum of the out weights */
    for (int src = 0; src < size; src++) {
        for (const auto &[_, weight] : graph.getAdjacents(src)) {
            outWeightSum[src] += weight;
        }
    }

    std::multimap<double, int> residueMap; /**< the map meaning the residue value, treated as heap */
    for (const auto &[src, weight] : sourceMap) {
        residueMap.insert({weight, src});
    }

    while (residueMap.size() > 0) {
        // get the node with the maximum residue value
        auto it = residueMap.rbegin();
        const double estimatedResidue = it->first;
        const int currentNode = it->second;

        if (estimatedResidue < thr) { // no more nodes to update
            break;
        }
        residueMap.erase(estimatedResidue);

        const double actualResidue = residue[currentNode];
        if (estimatedResidue - actualResidue > thr) { // check if the node is already updated
            continue;
        }

        // flow the residue value to the adjacents
        residue[currentNode] = 0;
        auto &adjacents = graph.getAdjacents(currentNode);
        if (adjacents.empty()) {
            // dangling node
            for (const auto &[src, srcWeight] : sourceMap) {
                residue[src] += (1 - alpha) * srcWeight * actualResidue;
                residueMap.insert({residue[src], src});
            }
        } else {
            for (const auto &[dst, weight] : graph.getAdjacents(currentNode)) {
                residue[dst] += (1 - alpha) * (weight / outWeightSum[currentNode]) * actualResidue;
                residueMap.insert({residue[dst], dst});
            }
        }
        ppr[currentNode] += alpha * actualResidue;
    }
    return std::tie(ppr, residue);
}

std::vector<double> fora(const IWeightedDigraph &graph, const std::vector<double> source, const double alpha, const double epsilon) {
    const int size = graph.size();
    std::vector<double> outWeightSum(size, 0); /**< the vector meaning the sum of the out weights */
    for (int src = 0; src < size; src++) {
        for (const auto &[dst, weight] : graph.getAdjacents(src)) {
            outWeightSum[src] += weight;
        }
    }
    const double totalEdges = std::reduce(outWeightSum.begin(), outWeightSum.end());

    std::vector<std::map<double, int>> outEdgesTable(size, std::map<double, int>()); /**< the outedges of each node */
    for (int src = 0; src < size; src++) {
        auto &outEdgesMap = outEdgesTable[src];
        const double weightSum = outWeightSum[src];

        // cumulative probability
        double cumWeight = 0;
        for (const auto &[dst, weight] : graph.getAdjacents(src)) {
            cumWeight += weight;
            const double cumProbabilty = alpha + (1 - alpha) * cumWeight / weightSum;
            outEdgesMap[cumProbabilty] = dst; // add upper bound of the interval
        }
    }
    // const double coef = (2*epsilon/3 + 2) * std::log(2/faultProbability) / (epsilon*epsilon * delta); 
    const double coef = (2*epsilon/3 + 2) * std::log(2*size) * size / epsilon / epsilon; /**< note: the coefficient but what? */
    spdlog::debug("coef = {}", coef);

    // forward push
    const double localThr = 1 / std::sqrt(coef * totalEdges); /**< threshold of forward push */
    auto [ppr, residue] = forwardPush(graph, source, alpha, localThr);

    // random walk
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> intDis(0, size - 1); // right inclusive
    std::uniform_real_distribution<double> realDis(0, 1); // right exclusive

    const double residueSum = std::reduce(residue.begin(), residue.end());
    const double omega = residueSum * coef;
    spdlog::debug("residueSum = {}, omega = {}", residueSum, omega);
    for (int src = 0; src < size; src++) {
        if (residue[src] == 0) {
            continue;
        }
        const int omegaSrc = std::ceil(residue[src] * coef);
        spdlog::debug("src =  {}, omegaSrc = {}", src, omegaSrc);
        const double randomWalkWeight = residue[src] / omegaSrc;

        const std::unordered_map<int, double> &adjacents = graph.getAdjacents(src);
        for (int i = 0; i < omegaSrc; i++) {
            int currentNode = src;

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
            ppr[currentNode] += randomWalkWeight;
        }
    }
    return ppr;
}

std::vector<double> singleSourcePersonalizedPageRank(const IWeightedDigraph &graph, const int source, const double alpha, const double epsilon) {
    const int size = graph.size();
    const double delta = 1 / size;
    const double faultProbability = 1 / size;
    std::vector<double> residue(size, 0); /**< the vector meaning the residue */
    residue[source] = 1;
    std::vector<double> ppr(size, 0); /**< the vector meaning the reserve/approximated PPR */

    int totalEdges = 0;
    std::vector<double> outWeightSum(size, 0); /**< the vector meaning the sum of the out weights */
    for (int src = 0; src < size; src++) {
        for (const auto &[dst, weight] : graph.getAdjacents(src)) {
            outWeightSum[src] += weight;
            totalEdges++;
        }
    }

    std::vector<std::map<double, int>> outEdgesTable(size, std::map<double, int>()); /**< the outedges of each node */
    for (int src = 0; src < size; src++) {
        auto &outEdgesMap = outEdgesTable[src];
        const double weightSum = outWeightSum[src];

        // cumulative probability
        double cumWeight = 0;
        for (const auto &[dst, weight] : graph.getAdjacents(src)) {
            cumWeight += weight;
            const double cumProbabilty = alpha + (1 - alpha) * cumWeight / weightSum;
            outEdgesMap[cumProbabilty] = dst; // add upper bound of the interval
            spdlog::debug("outEdgesTable[{}][{}] = {}", src, cumProbabilty, dst);
        }
    }

    // forward push
    const double maximumResidue = 1 / std::sqrt(totalEdges) * std::sqrt(delta / ((2*epsilon/3 + 2) * std::log(2/faultProbability))); /**< threshold of forward push */
    std::map<double, int> residueMap; /**< the map meaning the residue value, treated as heap */
    residueMap[outWeightSum[source]] = source;
    while (true) {
        // get the node with the maximum residue value
        auto it = residueMap.rbegin();
        const double estimatedResidue = it->first;
        const int currentNode = it->second;

        // check if the residue value is more than the threshold
        if (it == residueMap.rend() || estimatedResidue < maximumResidue) { // no more nodes to update
            break;
        }
        residueMap.erase(estimatedResidue);

        // check if the node is already updated
        const double actualResidue = residue[currentNode];
        if (estimatedResidue - actualResidue > maximumResidue) {
            continue;
        }

        // update the residue value
        auto &adjacents = graph.getAdjacents(currentNode);
        if (adjacents.empty()) {
            residue[source] += (1 - alpha) * actualResidue;
        } else {
            for (const auto &[dst, weight] : graph.getAdjacents(currentNode)) {
                residue[dst] += (1 - alpha) * (weight / outWeightSum[currentNode]) * actualResidue;
                residueMap[residue[dst]] = dst;
            }
        }
        ppr[currentNode] += alpha * actualResidue;
        residue[currentNode] = 0;
    }

    // random walk
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> intDis(0, size - 1); // right inclusive
    std::uniform_real_distribution<double> realDis(0, 1); // right exclusive

    const double residueSum = std::accumulate(residue.begin(), residue.end(), 0);
    const double omega = residueSum * (2*epsilon/3 + 2) * std::log(2/faultProbability) / (epsilon*epsilon * delta);
    for (int src = 0; src < size; src++) {
        if (residue[src] == 0) {
            continue;
        }
        const int omegaSrc = std::ceil(omega * residue[src] / residueSum);
        const double randomWalkWeight = residue[src] / omegaSrc;

        const std::unordered_map<int, double> &adjacents = graph.getAdjacents(src);
        for (int i = 0; i < omegaSrc; i++) {
            int currentNode = src;

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
            ppr[currentNode] += randomWalkWeight;
        }
    }
    return ppr;
}