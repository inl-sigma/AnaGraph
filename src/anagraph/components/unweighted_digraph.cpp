#include "anagraph/components/unweighted_digraph.hpp"

#include <spdlog/spdlog.h>

#include <set>

namespace anagraph {
namespace graph {

Node Digraph::getNode(int id) const {
    if (id < 0 || id >= static_cast<int>(nodes.size())) {
        throw std::out_of_range("Node does not exist");
    }
    return nodes[id];
}

Digraph::Digraph(std::string filePath, FileExtension extName) {
    readGraph(filePath, extName);
}

void Digraph::setNode(int id) {
    if (id >= static_cast<int>(nodes.size())) {
        nodes.resize(id + 1);
    }
    nodes[id] = Node(id);
    usedNodes.insert(id);
}

void Digraph::setNode(Node &node) {
    // If the ID is not initialized, add it to the end
    int nodeId;
    if (!node.isUsed()) {
        nodeId = nodes.size();
        node.setId(nodeId);
    } else {
        nodeId = node.getId();
    }
    if (static_cast<int>(nodeId >= static_cast<int>(nodes.size()))) {
        nodes.resize(nodeId + 1);
    }
    nodes[nodeId] = node;
    usedNodes.insert(nodeId);
}

void Digraph::removeNode(int id) {
    nodes[id].clear();
    usedNodes.erase(id);
}

std::unordered_set<int> Digraph::getIds() const {
    return usedNodes;
}

void Digraph::addEdge(int src, int dst) {
    const int maxId = std::max(src, dst);
    if (maxId >= static_cast<int>(nodes.size())) {
        setNode(maxId);
    }
    const int minId = std::min(src, dst);
    const auto minNode = getNode(minId);
    if (!minNode.isUsed()) {
        setNode(minId);
    }
    nodes[src].setAdjacent(dst);
}

void Digraph::removeEdge(int src, int dst) {
    if (!usedNodes.contains(src) || !usedNodes.contains(dst)) {
        throw std::out_of_range("Node does not exist");
    }
    nodes[src].removeAdjacent(dst);
}

const std::unordered_set<int> Digraph::getAdjacents(int id) const {
    return nodes[id].getAdjacents();
}

Digraph Digraph::getSubgraph(std::unordered_set<int> indices) const {
    Digraph subgraph;
    subgraph.nodes = this->nodes;
    subgraph.usedNodes = this->usedNodes;
    
    // remove unnecessary edges
    for (auto idx : indices) {
        const auto &adjacents = getAdjacents(idx);
        for (auto &adj : adjacents) {
            if (!indices.contains(adj)) {
                subgraph.removeEdge(idx, adj);
            }
        }
    }

    // remove unnecessary nodes
    for (auto idx : usedNodes) {
        if (!indices.contains(idx)) {
            subgraph.removeNode(idx);
        }
    }

    return subgraph;
}

void Digraph::reorganize() {
    spdlog::debug("called reorganize");

    // for sort and copy
    std::set<int> oldNodes = std::set<int>(this->usedNodes.begin(), this->usedNodes.end());
    spdlog::debug("clear usedNodes");
    usedNodes.clear();

    // Create a map from old id to new id
    spdlog::debug("create idMap and update usedNodes");
    std::unordered_map<int, int> idMap;
    int newId = 0;
    for (int oldId : oldNodes) {
        idMap[oldId] = newId;
        // move the node to the new id, with the adjacency list is maintained
        nodes[newId] = std::move(nodes[oldId]);
        usedNodes.insert(newId);
        newId++;
    }

    // Update the nodes and the adjacents
    spdlog::debug("update nodes and adjacents");
    for (int id = 0; id < newId; id++) {
        auto &oldNode = nodes[id];

        // set the new id
        auto newNode = Node(id);

        // set the adjacents
        for (auto &oldAdj : oldNode.getAdjacents()) {
            if (idMap.contains(oldAdj)) {
                newNode.setAdjacent(idMap[oldAdj]);
            } else {
                spdlog::warn("Node {} does not exist in the graph", oldAdj);
            }
        }

        nodes[id] = newNode;
    }

    // resize the nodes
    spdlog::debug("resize nodes");
    nodes.resize(idMap.size());
}

size_t Digraph::size() const {
    return usedNodes.size();
}

void Digraph::readGraph(std::string filePath, FileExtension extName) {
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

void Digraph::readGraphHelper(std::string filePath, IGraphParser &parser) {
    for (auto &[src, dst] : parser.parseGraph(filePath)) {
        addEdge(src, dst);
    }
}

 void Digraph::writeGraph(std::string filePath, FileExtension extName) const {
    // convert the graph to a list of edges
    // note : implement as function in weighted_graph.hpp if needed
    std::vector<EdgeObject> edges;
    for (int src : usedNodes) {
        for (auto &dst : getAdjacents(src)) {
            edges.push_back(EdgeObject(src, dst));
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

void Digraph::writeGraphHelper(std::string filePath, IGraphWriter &writer, std::vector<EdgeObject> &edges) const {
    writer.writeGraph(filePath, edges);
}

} // namespace graph
} // namespace anagraph