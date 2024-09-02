#include "anagraph/components/weighted_graph.hpp"

#include <spdlog/spdlog.h>

namespace anagraph {
namespace graph_structure {

WeightedGraph::WeightedGraph(std::string filePath, FileExtension extName) {
    readGraph(filePath, extName);
}

WeightedNode& WeightedGraph::getNode(int id) {
    return digraph.getNode(id);
}

void WeightedGraph::setNode(int id) {
    digraph.setNode(id);
}

void WeightedGraph::setNode(WeightedNode &node) {
    digraph.setNode(node);
}

void WeightedGraph::removeNode(int id) {
    digraph.removeNode(id);
}

std::unordered_set<int> WeightedGraph::getIds() const {
    return digraph.getIds();
}

void WeightedGraph::addEdge(int src, int dst, double weight) {
    digraph.addEdge(src, dst, weight);
    digraph.addEdge(dst, src, weight);
}

void WeightedGraph::removeEdge(int src, int dst) {
    digraph.removeEdge(src, dst);
    digraph.removeEdge(dst, src);
}

double WeightedGraph::getWeight(int src, int dst) const {
    return digraph.getWeight(src, dst);
}

void WeightedGraph::setWeight(int src, int dst, double weight) {
    digraph.setWeight(src, dst, weight);
    digraph.setWeight(dst, src, weight);
}

void WeightedGraph::addWeight(int src, int dst, double weight) {
    digraph.addWeight(src, dst, weight);
    digraph.addWeight(dst, src, weight);
}

const std::unordered_map<int, double> WeightedGraph::getAdjacents(int id) const {
    return digraph.getAdjacents(id);
}

WeightedGraph WeightedGraph::getSubgraph(std::unordered_set<int> indices) const {
    WeightedGraph subgraph;
    subgraph.digraph = digraph.getSubgraph(indices);
    return subgraph;
}

void WeightedGraph::reorganize() {
    digraph.reorganize();
}

size_t WeightedGraph::size() const {
    return digraph.size();
}

WeightedDigraph WeightedGraph::toDigraph() const {
    return digraph;
}

void WeightedGraph::readGraph(std::string filePath, FileExtension extName) {
    digraph.readGraph(filePath, extName);
    WeightedDigraph deepCopy = digraph;
    for (auto src : digraph.getIds()) {
        for (auto [adj, weight] : deepCopy.getAdjacents(src)) {
            digraph.addEdge(adj, src, weight);
        }
    }
}

void WeightedGraph::writeGraph(std::string filePath, FileExtension extName) const {
    auto digraph = toDigraph();
    for (auto id : digraph.getIds()) {
        auto adjacents = digraph.getAdjacents(id);
        for (auto [adj, _] : adjacents) {
            if (id > adj) {
                digraph.removeEdge(id, adj);

            }
        }
    }
    digraph.writeGraph(filePath, extName);
}

} // namespace graph
} // namespace anagraph