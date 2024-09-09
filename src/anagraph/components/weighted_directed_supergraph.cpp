#include "anagraph/components/weighted_directed_supergraph.hpp"

#include <spdlog/spdlog.h>

#include <stdexcept>

namespace anagraph {
namespace graph_structure {

WeightedSuperDigraph::WeightedSuperDigraph(std::string filepath, FileExtension extName) {
    readGraph(filepath, extName);
}

WeightedSupernode& WeightedSuperDigraph::getNode(int id) {
    return nodes.at(id);
}

void WeightedSuperDigraph::setNode(int id) {
    nodes.insert({id, WeightedSupernode(id)});
}

void WeightedSuperDigraph::setNode(const WeightedSupernode &node) {
    const int nodeId = node.getId();
    nodes.insert({nodeId, node});
}

void WeightedSuperDigraph::removeNode(int id) {
    nodes.erase(id);
}

void WeightedSuperDigraph::mergeNode(int first, int second, mergeLambda mergeFunc) {
    spdlog::debug("Merging nodes {} and {}", first, second);
    if (!mergeFunc) {
        throw std::bad_function_call();
    }
    if (!nodes.contains(first) || !nodes.contains(second)) {
        throw std::out_of_range("Node does not exist");
    }
    spdlog::debug("before merge: {}", nodes.size());
    WeightedSupernode mergedNode = mergeFunc(nodes[first], nodes[second]);
    spdlog::debug("after merge: {}", nodes.size());
    setNode(mergedNode);
}

void WeightedSuperDigraph::mergeNode(int first, int second) {
    if (!mergeNodeFunc) {
        throw std::bad_function_call();
    }
    mergeNode(first, second, mergeNodeFunc);
}

void WeightedSuperDigraph::setMergeNodeFunction(mergeLambda mergeFunc) {
    mergeNodeFunc = mergeFunc;
}

std::unordered_set<int> WeightedSuperDigraph::getIds() const {
    std::unordered_set<int> ids;
    for (auto &[id, _] : nodes) {
        ids.insert(id);
    }
    return ids;
}

void WeightedSuperDigraph::setEdge(int src, int dst, double weight) {
    if (!nodes.contains(src)) {
        setNode(src);
    }
    if (!nodes.contains(dst)) {
        setNode(dst);
    }
    nodes[src].setAdjacentNode(nodes[dst], weight);
}

void WeightedSuperDigraph::removeEdge(int src, int dst) {
    if (!nodes.contains(src) || !nodes.contains(dst)) {
        throw std::out_of_range("Node does not exist");
    }
    nodes[src].removeAdjacent(dst);
}

double WeightedSuperDigraph::getWeight(int src, int dst) const {
    if (!nodes.contains(src) || !nodes.contains(dst)) {
        throw std::out_of_range("Node does not exist");
    }
    return nodes.at(src).getWeight(dst);
}

void WeightedSuperDigraph::setWeight(int src, int dst, double weight) {
    if (!nodes.contains(src) || !nodes.contains(dst)) {
        throw std::out_of_range("Node does not exist");
    }
    nodes[src].setWeight(dst, weight);
}

void WeightedSuperDigraph::addWeight(int src, int dst, double weight) {
    if (!nodes.contains(src) || !nodes.contains(dst)) {
        throw std::out_of_range("Node does not exist");
    }
    nodes[src].updateWeight(dst, weight);
}

int WeightedSuperDigraph::getParent(int id) const {
    if (!nodes.contains(id)) {
        throw std::out_of_range("Node does not exist");
    }
    return nodes.at(id).getParent();
}

void WeightedSuperDigraph::setParent(int child, int parent) {
    if (!nodes.contains(child)) {
        setNode(child);
    }
    if (!nodes.contains(parent)) {
        setNode(parent);
    }
    nodes[child].setParent(parent);
    nodes[parent].addChild(child);
}

void WeightedSuperDigraph::updateParent(int child, int parent) {
    if (!nodes.contains(child) || !nodes.contains(parent)) {
        throw std::out_of_range("Node does not exist");
    }
    const int oldParent = nodes[child].getParent();
    nodes[child].setParent(parent);
    nodes[parent].addChild(child);
    nodes[oldParent].removeChild(child);
}

void WeightedSuperDigraph::removeParent(int child) {
    if (!nodes.contains(child)) {
        throw std::out_of_range("Node does not exist");
    }
    const int parent = nodes[child].getParent();
    if (parent == WeightedSupernode::ROOT) {
        spdlog::warn("Node has no parent");
        return;
    }
    if (!nodes.contains(parent)) {
        throw std::out_of_range("Parent node does not exist");
    }
    nodes[child].setParent(WeightedSupernode::ROOT);
    nodes[parent].removeChild(child);
}

std::unordered_set<int> WeightedSuperDigraph::getChildren(int id) const {
    if (!nodes.contains(id)) {
        throw std::out_of_range("Node does not exist");
    }
    return nodes.at(id).getChildren();
}

const std::unordered_map<int, double>& WeightedSuperDigraph::getAdjacents(int id) const {
    return nodes.at(id).getAdjacents();
}

size_t WeightedSuperDigraph::size() const {
    return nodes.size();
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
        setEdge(src, dst, weight);
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
    for (auto &[src, node] : nodes) {
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