#include "anagraph/components/weighted_digraph.hpp"

#include <spdlog/spdlog.h>

#include <set>
#include <stdexcept>

namespace anagraph {
namespace graph_structure {


WeightedDigraph::WeightedDigraph(std::string filePath, FileExtension extName) {
    readGraph(filePath, extName);
}

WeightedNode& WeightedDigraph::getNode(int id) {
    return nodes.at(id);
}

void WeightedDigraph::setNode(int id) {
    nodes.insert({id, WeightedNode(id)});
}

void WeightedDigraph::setNode(WeightedNode &node) {
    const int nodeId = node.getId();
    nodes.insert({nodeId, node});
}

void WeightedDigraph::removeNode(int id) {
    nodes.erase(id);
}

std::unordered_set<int> WeightedDigraph::getIds() const {
    std::unordered_set<int> ids;
    for (auto &[id, _] : nodes) {
        ids.insert(id);
    }
    return ids;
}

void WeightedDigraph::setEdge(int src, int dst, double weight) {
    if (!nodes.contains(src)) {
        setNode(src);
    }
    if (!nodes.contains(dst)) {
        setNode(dst);
    }
    nodes[src].setAdjacentNode(getNode(dst), weight);
}

void WeightedDigraph::removeEdge(int src, int dst) {
    if (!nodes.contains(src)) {
        throw std::out_of_range("Node does not exist");
    }
    nodes[src].removeAdjacent(dst);
}

double WeightedDigraph::getWeight(int src, int dst) const {
    if (!nodes.contains(src) || !nodes.contains(dst)) {
        throw std::out_of_range("Node does not exist");
    }
    return nodes.at(src).getWeight(dst);
}

void WeightedDigraph::setWeight(int src, int dst, double weight) {
    nodes[src].setWeight(dst, weight);
}

void WeightedDigraph::addWeight(int src, int dst, double weight) {
    nodes[src].updateWeight(dst, weight);
}

const std::unordered_map<int, double>& WeightedDigraph::getAdjacents(int id) const {
    return nodes.at(id).getAdjacents();
}

WeightedDigraph WeightedDigraph::getSubgraph(std::unordered_set<int> indices) const {
    WeightedDigraph subgraph;
    // copy necessary nodes
    for (auto idx : indices) {
        if (!nodes.contains(idx)) {
            throw std::out_of_range("Node does not exist");
        }
        WeightedNode node = nodes.at(idx);
        subgraph.setNode(node);
    }
    
    // remove unnecessary edges
    for (auto idx : indices) {
        const auto &adjacents = getAdjacents(idx);
        for (auto [adj, weight] : adjacents) {
            if (!indices.contains(adj)) {
                subgraph.removeEdge(idx, adj);
            }
        }
    }
    return subgraph;
}

void WeightedDigraph::reorganize() {
    spdlog::debug("called reorganize");

    // Create a map from old id to new id
    spdlog::debug("create idMap and update usedNodes");
    std::unordered_map<int, int> idMap;
    int newId = 0;
    for (auto &[oldId, _] : nodes) {
        idMap[oldId] = newId;
        // move the node to the new id, with the adjacency list is maintained
        nodes[newId] = std::move(nodes[oldId]);
        nodes[newId].setId(newId);
        newId++;
    }

    // Update the nodes and the adjacents
    spdlog::debug("update nodes and adjacents");
    for (auto id : getIds()) {
        if (id > newId) { // remove the node if it is not used
            nodes.erase(id);
            continue;
        }

        // set the adjacents
        auto &node = nodes.at(id);
        auto oldAdjacents = node.getAdjacents();
        for (auto &[oldAdj, _] : oldAdjacents) {
            node.removeAdjacent(oldAdj);
        }
        for (auto &[oldAdj, weight] : oldAdjacents) {
            const int newAdj = idMap[oldAdj];
            node.setAdjacentNode(nodes[newAdj], weight);
        }
    }
}

size_t WeightedDigraph::size() const {
    return nodes.size();
}

void WeightedDigraph::readGraph(std::string filePath, FileExtension extName) {
    switch (extName) {
    case FileExtension::TXT: {
        TextGraphParser parser;
        readGraphHelper(filePath, parser);
    }
        break;

    case FileExtension::CSV: {
        CSVGraphParser parser;
        readGraphHelper(filePath, parser);
    }
        break;

    // case FileExtension::GML:
    //     readGraphHelper(filePath, GMLGraphParser());
    //     break;
    
    default:
        throw std::invalid_argument("Invalid file extension");
        break;
    }
}

void WeightedDigraph::readGraphHelper(std::string filePath, IGraphParser &parser) {
    for (auto &[src, dst, weight] : parser.parseWeightedGraph(filePath)) {
        setEdge(src, dst, weight);
    }
}

void WeightedDigraph::writeGraph(std::string filePath, FileExtension extName) const {
    // convert the graph to a list of edges
    std::vector<WeightedEdgeObject> edges;
    for (auto &[src, node] : nodes) {
        for (auto &[dst, weight] : node.getAdjacents()) {
            edges.push_back(WeightedEdgeObject(src, dst, weight));
        }
    }

    switch (extName) {
    case FileExtension::TXT: {
        TextGraphWriter writer;
        writeGraphHelper(filePath, writer, edges);
    }
        break;

    case FileExtension::CSV: {
        CSVGraphWriter writer;
        writeGraphHelper(filePath, writer, edges);
    }
        break;

    // case FileExtension::GML:
    //     writeGraphHelper(filePath, GMLGraphWriter(), edges);
    //     break;

    default:
        throw std::invalid_argument("Invalid file extension");
        break;
    }
}

void WeightedDigraph::writeGraphHelper(std::string filePath, IGraphWriter &writer, std::vector<WeightedEdgeObject> edges) const {
    writer.writeWeightedGraph(filePath, edges);
}

} // namespace graph
} // namespace anagraph