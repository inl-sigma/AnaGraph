#include "heterogeneous_graph.hpp"

#include "graph_parser.hpp"
#include "graph_utils.hpp"
#include "graph_writer.hpp"

#include <spdlog/spdlog.h>

#include <any>
#include <fstream>
#include <memory>
#include <unordered_map>
#include <unordered_set>


template <typename T>
WeightedHeteroNode<T>& WeightedHeteroNode<T>::operator=(const WeightedHeteroNode<T>& node) {
    weightedNode = node.weightedNode;
    attributes = node.attributes;
    isAttrEnabled = node.isAttrEnabled;
    return *this;
}

template <typename T>
const int WeightedHeteroNode<T>::UNUSED_ID = -1;

template <typename T>
int WeightedHeteroNode<T>::getId() const{
    return weightedNode.getId();
}

template <typename T>
void WeightedHeteroNode<T>::setId(int id) {
    weightedNode.setId(id);
}

template <typename T>
bool WeightedHeteroNode<T>::isUsed() const {
    return weightedNode.isUsed();
}

template <typename T>
const std::unordered_map<int, double>& WeightedHeteroNode<T>::getAdjacents() const {
    return weightedNode.getAdjacents();
}

template <typename T>
void WeightedHeteroNode<T>::setAdjacent(int adjacent, double weight) {
    weightedNode.setAdjacent(adjacent, weight);
}

template <typename T>
void WeightedHeteroNode<T>::updateAdjacent(int adjacent, double weight) {
    weightedNode.updateAdjacent(adjacent, weight);
}

template <typename T>
void WeightedHeteroNode<T>::removeAdjacent(int adjacent) {
    weightedNode.removeAdjacent(adjacent);
}

template <typename T>
T WeightedHeteroNode<T>::getAttributes() const {
    if (!isAttrEnabled) {
        throw std::runtime_error("Attributes are not enabled");
    }
    return attributes;
}

template <typename T>
void WeightedHeteroNode<T>::setAttributes(T attributes){
    this->attributes = attributes;
    isAttrEnabled = true;
}

template <typename T>
void WeightedHeteroNode<T>::clear() {
    weightedNode.clear();
    attributes = T();
    isAttrEnabled = false;
}

template <typename T>
WeightedHeteroGraph<T>::WeightedHeteroGraph() {
    usedNodes = std::unordered_set<int>();
    nodes = std::vector<WeightedHeteroNode<T>>();
}

template <typename T>
WeightedHeteroGraph<T>::WeightedHeteroGraph(std::string filePath, FileExtension extName) {
    usedNodes = std::unordered_set<int>();
    readGraph(filePath, extName);
}

template <typename T>
WeightedHeteroGraph<T>& WeightedHeteroGraph<T>::operator=(const WeightedHeteroGraph<T>& graph) {
    usedNodes = graph.usedNodes;
    nodes = graph.nodes;
    return *this;
}

template <typename T>
WeightedHeteroNode<T> WeightedHeteroGraph<T>::getNode(int id) const {
    return nodes[id];
}

template <typename T>
void WeightedHeteroGraph<T>::setNode(int id) {
    if (id >= static_cast<int>(nodes.size())) {
        nodes.resize(id + 1);
    }
    nodes[id] = WeightedHeteroNode<T>(id);
    usedNodes.insert(id);
}

template <typename T>
void WeightedHeteroGraph<T>::setNode(WeightedHeteroNode<T> &node) {
    // If the ID is not initialized, add it to the end
    const int nodeId = (!node.isUsed()) ? nodes.size() : node.getId();
    if (static_cast<int>(nodeId >= static_cast<int>(nodes.size()))) {
        nodes.resize(nodeId + 1);
    }
    nodes[nodeId] = node;
    usedNodes.insert(nodeId);
}

template <typename T>
void WeightedHeteroGraph<T>::removeNode(int id) {
    nodes[id].clear();
    usedNodes.erase(id);
}

template <typename T>
void WeightedHeteroGraph<T>::addEdge(int src, int dst, double weight) {
    const int maxId = std::max(src, dst);
    if (maxId >= static_cast<int>(nodes.size())) {
        setNode(maxId);
    }
    const int minId = std::min(src, dst);
    const auto minNode = getNode(minId);
    if (!minNode.isUsed()) {
        setNode(minId);
    }
    nodes[src].updateAdjacent(dst, weight);
    nodes[dst].updateAdjacent(src, weight);
}

template <typename T>
void WeightedHeteroGraph<T>::removeEdge(int src, int dst) {
    const int maxId = std::max(src, dst);
    if (maxId >= static_cast<int>(nodes.size())) {
        throw std::out_of_range("Node does not exist");
    }
    nodes[src].removeAdjacent(dst);
    nodes[dst].removeAdjacent(src);
}

template <typename T>
double WeightedHeteroGraph<T>::getWeight(int src, int dst) const {
    const int maxId = std::max(src, dst);
    if (maxId >= static_cast<int>(nodes.size())) {
        throw std::out_of_range("Node does not exist");
    }
    const auto &adjacents = getAdjacents(src);
    if (!adjacents.contains(dst)) {
        spdlog::debug("Edge does not exist between {} and {}", src, dst);
        return 0.0;
    } else {
        return adjacents.at(dst);
    }
}

template <typename T>
void WeightedHeteroGraph<T>::setWeight(int src, int dst, double weight) {
    const int maxId = std::max(src, dst);
    if (maxId >= static_cast<int>(nodes.size())) {
        throw std::out_of_range("Node does not exist, use setNode() instead");
    }
    nodes[src].setAdjacent(dst, weight);
    nodes[dst].setAdjacent(src, weight);
}

template <typename T>
void WeightedHeteroGraph<T>::addWeight(int src, int dst, double weight) {
    const int maxId = std::max(src, dst);
    if (maxId >= static_cast<int>(nodes.size())) {
        throw std::out_of_range("Node does not exist");
    }
    nodes[src].updateAdjacent(dst, weight);
    nodes[dst].updateAdjacent(src, weight);
}

template <typename T>
const std::unordered_map<int, double>& WeightedHeteroGraph<T>::getAdjacents(int id) const {
    if (id >= static_cast<int>(nodes.size())) {
        throw std::out_of_range("Node does not exist");
    }
    return nodes[id].getAdjacents();
}

template <typename T>
WeightedHeteroGraph<T> WeightedHeteroGraph<T>::getSubgraph(std::unordered_set<int> indices) const {
    WeightedHeteroGraph<T> subgraph;
    // copy necessary nodes
    for (auto idx : indices) {
        if (idx >= static_cast<int>(nodes.size())) {
            throw std::out_of_range("Node does not exist");
        }
        WeightedHeteroNode<T> node = nodes[idx];
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

template <typename T>
void WeightedHeteroGraph<T>::organize() {
    spdlog::debug("called organize");

    spdlog::debug("clear usedNodes");
    usedNodes.clear();

    // Create a map from old id to new id
    spdlog::debug("create idMap and update usedNodes");
    std::unordered_map<int, int> idMap;
    int newId = 0;
    for (int oldId = 0; oldId < static_cast<int>(nodes.size()); oldId++) {
        if (nodes[oldId].isUsed()) {
            idMap[oldId] = newId;
            nodes[newId] = nodes[oldId];
            usedNodes.insert(newId);
            newId++;
        }
    }

    // Update the nodes and the adjacents
    spdlog::debug("update nodes and adjacents");
    for (int newId = 0; newId < static_cast<int>(idMap.size()); newId++) {
        auto &oldNode = nodes[newId];

        // set the new id
        auto newNode = WeightedHeteroNode<T>(newId);

        // set the adjacents
        for (auto [oldAdj, weight] : oldNode.getAdjacents()) {
            if (idMap.contains(oldAdj)) {
                newNode.setAdjacent(idMap[oldAdj], weight);
            } else {
                spdlog::warn("Node {} does not exist in the graph", oldAdj);
            }
        }

        // set the attributes
        if (oldNode.getAttributesPtr() != nullptr) {
            newNode.setAttributes(*oldNode.getAttributesPtr());
        }

        nodes[newId] = newNode;
    }

    // resize the nodes
    spdlog::debug("resize nodes");
    nodes.resize(idMap.size());
}

template <typename T>
T WeightedHeteroGraph<T>::getAttributes(int id) const {
    return nodes[id].getAttributes();
}

template <typename T>
void WeightedHeteroGraph<T>::setAttributes(int id, T attributes) {
    nodes[id].setAttributes(attributes);
}

template <typename T>
size_t WeightedHeteroGraph<T>::size() const {
    return usedNodes.size();
}

template <typename T>
void WeightedHeteroGraph<T>::readGraph(std::string filePath, FileExtension extName) {
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

template <typename T>
void WeightedHeteroGraph<T>::readGraphHelper(std::string filePath, IGraphParser &parser) {
    for (auto &[src, dst, weight] : parser.parseWeightedHeteroGraph(filePath)) {
        addEdge(src, dst, weight);
    }
}

template <typename T>
void WeightedHeteroGraph<T>::writeGraph(std::string filePath, FileExtension extName) const {
    // convert the graph to a list of edges
    // note : implement as function in weighted_graph.hpp if needed
    std::vector<WeightedEdgeObject> edges;
    for (int src = 0; src < static_cast<int>(nodes.size()); src++) {
        if (!nodes[src].isUsed()) {
            continue;
        }
        for (auto [dst, weight] : getAdjacents(src)) {
            if (src <= dst) {
                edges.push_back(WeightedEdgeObject(src, dst, weight));
            }
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

template <typename T>
void WeightedHeteroGraph<T>::writeGraphHelper(std::string filePath, IGraphWriter &writer, std::vector<WeightedEdgeObject> edges) const {
    writer.writeWeightedHeteroGraph(filePath, edges);
}

// Explicit instantiation
template class WeightedHeteroNode<int>;
template class WeightedHeteroNode<std::string>;
template class WeightedHeteroNode<std::any>;

template class WeightedHeteroGraph<int>;
template class WeightedHeteroGraph<std::string>;
template class WeightedHeteroGraph<std::any>;