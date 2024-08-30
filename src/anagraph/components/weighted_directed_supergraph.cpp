#include "anagraph/components/weighted_directed_supergraph.hpp"

#include <spdlog/spdlog.h>

#include <stdexcept>

namespace anagraph {
namespace graph_structure {

WeightedSuperDigraph::WeightedSuperDigraph(std::string filepath, FileExtension extName) {
    readGraph(filepath, extName);
}

void WeightedSuperDigraph::setNode(int id) {
    if (id >= static_cast<int>(nodes.size())) {
        nodes.resize(id + 1);
    }
    nodes[id] = WeightedSupernode(id);
    usedNodes.insert(id);
}

void WeightedSuperDigraph::setNode(WeightedSupernode &node) {
    // If the ID is not initialized, add it to the end
    int nodeId;
    if (!node.isUsed()) {
        nodeId = nodes.size();
        node.setId(nodeId);
    } else {
        nodeId = node.getId();
    }
    if (nodeId >= static_cast<int>(nodes.size())) {
        nodes.resize(nodeId + 1);
    }
    nodes[nodeId] = node;
    usedNodes.insert(nodeId);
}

WeightedSupernode WeightedSuperDigraph::getNode(int id) const {
    return nodes[id];
}

void WeightedSuperDigraph::removeNode(int id) {
    nodes[id].clear();
    usedNodes.erase(id);
}

std::unordered_set<int> WeightedSuperDigraph::getIds() const {
    return usedNodes;
}

void WeightedSuperDigraph::addEdge(int src, int dst, double weight) {
    const int maxId = std::max(src, dst);
    if (maxId >= static_cast<int>(nodes.size())) {
        setNode(maxId);
    }
    const int minId = std::min(src, dst);
    const auto &minNode = getNode(minId);
    if (!minNode.isUsed()) {
        setNode(minId);
    }
    nodes[src].setAdjacent(dst, weight);
}

void WeightedSuperDigraph::removeEdge(int src, int dst) {
    const int maxId = std::max(src, dst);
    if (maxId >= static_cast<int>(nodes.size())) {
        throw std::out_of_range("Node does not exist");
    }
    nodes[src].removeAdjacent(dst);
}

double WeightedSuperDigraph::getWeight(int src, int dst) const {
    const int maxId = std::max(src, dst);
    if (maxId >= static_cast<int>(nodes.size())) {
        throw std::out_of_range("Node does not exist");
    }
    const auto &node = getNode(src);
    const auto adjacents = node.getAdjacents();
    if (!adjacents.contains(dst)) {
        spdlog::debug("Edge does not exist between {} and {}", src, dst);
        return 0.0;
    } else {
        return adjacents.at(dst);
    }
}

void WeightedSuperDigraph::setWeight(int src, int dst, double weight) {
    const int maxId = std::max(src, dst);
    if (maxId >= static_cast<int>(nodes.size())) {
        throw std::out_of_range("Node does not exist, use setNode() instead");
    }
    nodes[src].setAdjacent(dst, weight);
}

void WeightedSuperDigraph::addWeight(int src, int dst, double weight) {
    const int maxId = std::max(src, dst);
    if (maxId >= static_cast<int>(nodes.size())) {
        throw std::out_of_range("Node does not exist");
    }
    nodes[src].updateAdjacent(dst, weight);
}

int WeightedSuperDigraph::getParent(int id) const {
    if (!usedNodes.contains(id)) {
        throw std::out_of_range("Node does not exist");
    }
    return nodes[id].getParent();
}

void WeightedSuperDigraph::setParent(int child, int parent) {
    const int maxId = std::max(child, parent);
    if (maxId >= static_cast<int>(nodes.size())) {
        setNode(maxId);
    }
    const int minId = std::min(child, parent);
    const auto minNode = getNode(minId);
    if (!minNode.isUsed()) {
        setNode(minId);
    }
    nodes[child].setParent(parent);
    nodes[parent].addChild(child);
}

void WeightedSuperDigraph::updateParent(int child, int parent) {
    if ((!usedNodes.contains(child)) || (!usedNodes.contains(parent))) {
        throw std::out_of_range("Node does not exist");
    }
    const int oldParent = nodes[child].getParent();
    nodes[child].setParent(parent);
    nodes[parent].addChild(child);
    nodes[oldParent].removeChild(child);
}

void WeightedSuperDigraph::removeParent(int child) {
    if (!usedNodes.contains(child)) {
        throw std::out_of_range("Node does not exist");
    }
    const int parent = nodes[child].getParent();
    if (parent == WeightedSupernode::ROOT) {
        spdlog::warn("Node has no parent");
        return;
    }
    if (!usedNodes.contains(parent)) {
        throw std::out_of_range("Node does not exist");
    }
    nodes[child].setParent(WeightedSupernode::ROOT);
    nodes[parent].removeChild(child);
}

std::unordered_set<int> WeightedSuperDigraph::getChildren(int id) const {
    if (!usedNodes.contains(id)) {
        throw std::out_of_range("Node does not exist");
    }
    return nodes[id].getChildren();
}

const std::unordered_map<int, double> WeightedSuperDigraph::getAdjacents(int id) const {
    return nodes[id].getAdjacents();
}

size_t WeightedSuperDigraph::size() const {
    return usedNodes.size();
}

void WeightedSuperDigraph::readGraph(std::string filePath, FileExtension extName) {
    switch (extName) {
    case FileExtension::TXT: {
        TextGraphParser parser;
        readEdgeHelper(filePath + "/edges.txt", parser);
        readHierarchyHelper(filePath + "/parents.txt", parser);
    }
        break;

    case FileExtension::CSV: {
        CSVGraphParser parser;
        readEdgeHelper(filePath + "/edges.csv", parser);
        readHierarchyHelper(filePath + "parents.csv", parser);
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

void WeightedSuperDigraph::readEdgeHelper(std::string filePath, IGraphParser &parser) {
    // read normal edges from the file
    for (auto &[src, dst, weight] : parser.parseWeightedGraph(filePath)) {
        addEdge(src, dst, weight);
    }
}

void WeightedSuperDigraph::readHierarchyHelper(std::string filePath, IGraphParser &parser) {
    // read the hierarchical edges from the file
    for (auto &[parent, child] : parser.parseGraph(filePath)) {
        nodes[child].setParent(parent);
        nodes[parent].addChild(child);
    }
}

void WeightedSuperDigraph::writeGraph(std::string filePath, FileExtension extName) const {
    std::vector<WeightedEdgeObject> normalEdges;
    std::vector<EdgeObject> hierarchicalEdges;
    for (int src : usedNodes) {
        auto &node = nodes[src];
        for (auto [dst, weight] : node.getAdjacents()) {
            normalEdges.push_back(WeightedEdgeObject(src, dst, weight));
        }
        if (!node.isRoot()) {
            hierarchicalEdges.push_back(EdgeObject(node.getParent(), src));
        }
    }

    switch (extName) {
    case FileExtension::TXT: {
        TextGraphWriter writer;
        // write the normal edges to the file
        writer.writeWeightedGraph(filePath + "/edges.txt", normalEdges);
        // write the hierarchical edges to the file
        writer.writeGraph(filePath + "/parents.txt", hierarchicalEdges);
    }
        break;

    case FileExtension::CSV: {
        CSVGraphWriter writer;
        // write the normal edges to the file
        writer.writeWeightedGraph(filePath + "/edges.csv", normalEdges);
        // write the hierarchical edges to the file
        writer.writeGraph(filePath + "/parents.csv", hierarchicalEdges);
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

} // namespace graph_structure
} // namespace anagraph