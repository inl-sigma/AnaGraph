#include "anagraph/components/unweighted_graph.hpp"

#include <spdlog/spdlog.h>

#include <set>

namespace anagraph {
namespace graph_structure {

Graph::Graph(std::string filePath, FileExtension extName) {
    readGraph(filePath, extName);
}

Node Graph::getNode(int id) const {
    return digraph.getNode(id);
}

void Graph::setNode(int id) {
    digraph.setNode(id);
}

void Graph::setNode(Node &node) {
    digraph.setNode(node);
}

void Graph::removeNode(int id) {
    digraph.removeNode(id);
}

std::unordered_set<int> Graph::getIds() const {
    return digraph.getIds();
}

void Graph::addEdge(int src, int dst) {
    digraph.addEdge(src, dst);
    digraph.addEdge(dst, src);
}

void Graph::removeEdge(int src, int dst) {
    digraph.removeEdge(src, dst);
    digraph.removeEdge(dst, src);
}

const std::unordered_set<int> Graph::getAdjacents(int id) const {
    return digraph.getAdjacents(id);
}

Graph Graph::getSubgraph(std::unordered_set<int> indices) const {
    Graph subgraph;
    subgraph.digraph = digraph.getSubgraph(indices);
    return subgraph;
}

void Graph::reorganize() {
    digraph.reorganize();
}

Digraph Graph::toDigraph() const {
    return digraph;
}

size_t Graph::size() const {
    return digraph.size();
}

void Graph::readGraph(std::string filePath, FileExtension extName) {
    digraph.readGraph(filePath, extName);
    Digraph deepCopy = digraph;
    for (auto id : digraph.getIds()) {
        for (auto adj : deepCopy.getAdjacents(id)) {
            digraph.addEdge(adj, id);
        }
    }
}

void Graph::writeGraph(std::string filePath, FileExtension extName) const {
    auto digraph = toDigraph();
    for (auto id : digraph.getIds()) {
        for (auto adj : digraph.getAdjacents(id)) {
            if (id > adj) {
                digraph.removeEdge(id, adj);
            }
        }
    }
    digraph.writeGraph(filePath, extName);
}

} // namespace graph
} // namespace anagraph