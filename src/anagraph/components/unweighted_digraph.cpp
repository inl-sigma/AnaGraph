#include "anagraph/components/unweighted_digraph.hpp"

#include <spdlog/spdlog.h>

#include <set>

namespace anagraph {
namespace graph_structure {

Digraph::Digraph(std::string filePath, FileExtension extName) {
    readGraph(filePath, extName);
}

Node& Digraph::getNode(int id) {
    if (!nodes.contains(id)) {
        throw std::out_of_range("Node does not exist");
    }
    return nodes.at(id);
}

void Digraph::setNode(int id) {
    nodes[id] = Node(id);
}

void Digraph::setNode(Node &node) {
    // If the ID is not initialized, add it to the end
    const int nodeId = node.getId();
    nodes[nodeId] = node;
}

void Digraph::removeNode(int id) {
    nodes.erase(id);
}

std::unordered_set<int> Digraph::getIds() const {
    std::unordered_set<int> ids;
    for (auto &node : nodes) {
        ids.insert(node.first);
    }
    return ids;
}

void Digraph::addEdge(int src, int dst) {
    if (!nodes.contains(src)) {
        setNode(src);
    }
    if (!nodes.contains(dst)) {
        setNode(dst);
    }
    nodes[src].setAdjacentNode(getNode(dst));
}

void Digraph::removeEdge(int src, int dst) {
    if (!nodes.contains(src) || !nodes.contains(dst)) {
        throw std::out_of_range("Node does not exist");
    }
    spdlog::debug("remove edge {} -> {}", src, dst);
    nodes[src].removeAdjacent(dst);
}

const std::unordered_set<int>& Digraph::getAdjacents(int id) const {
    return nodes.at(id).getAdjacents();
}

Digraph Digraph::getSubgraph(std::unordered_set<int> indices) const {
    Digraph subgraph;
    subgraph.nodes = this->nodes;
    
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
    for (auto idx : getIds()) {
        if (!indices.contains(idx)) {
            subgraph.removeNode(idx);
        }
    }

    return subgraph;
}

void Digraph::reorganize() {
    spdlog::debug("called reorganize");

    // Create a map from old id to new id
    spdlog::debug("create idMap and update usedNodes");
    std::unordered_map<int, int> idMap;
    int newId = 0;
    for (auto &[oldId, node] : nodes) {
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
        for (auto oldAdj : oldAdjacents) {
            node.removeAdjacent(oldAdj);
        }
        for (auto oldAdj : oldAdjacents) {
            const int newAdj = idMap[oldAdj];
            node.setAdjacentNode(nodes[newAdj]);
        }
    }
}

size_t Digraph::size() const {
    return nodes.size();
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
    for (auto &[src, node] : nodes) {
        for (auto dst : node.getAdjacents()) {
            edges.emplace_back(EdgeObject(src, dst));
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