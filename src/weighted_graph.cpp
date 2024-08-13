#include "weighted_graph.hpp"

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
WeightedNode<T>::WeightedNode() {
    id = UNUSED_ID;
    adjacents = std::unordered_map<int, double>();
}

template <typename T>
WeightedNode<T>::WeightedNode(int id) {
    setId(id);
    adjacents = std::unordered_map<int, double>();
}

template <typename T>
WeightedNode<T>::WeightedNode(int id, T attributes) {
    setId(id);
    setAttributes(attributes);
    adjacents = std::unordered_map<int, double>();
}

template <typename T>
WeightedNode<T>& WeightedNode<T>::operator=(const WeightedNode<T>& node) {
    id = node.id;
    attributes = node.attributes;
    adjacents = node.adjacents;
    return *this;
}

template <typename T>
const int WeightedNode<T>::UNUSED_ID = -1;

template <typename T>
int WeightedNode<T>::getId() const{
    return id;
}

template <typename T>
void WeightedNode<T>::setId(int id) {
    if (id < 0) {
        spdlog::warn("setId: negative id({}) detected, use clear() to reset node", id);
    } else {
        this->id = id;
    }
}

template <typename T>
bool WeightedNode<T>::isUsed() const {
    return id != UNUSED_ID;
}

template <typename T>
const std::unordered_map<int, double>& WeightedNode<T>::getAdjacents() const {
    return adjacents;
}

template <typename T>
void WeightedNode<T>::setAdjacent(int adjacent, double weight) {
    adjacents[adjacent] = weight;
}

template <typename T>
void WeightedNode<T>::updateAdjacent(int adjacent, double weight) {
    if (!adjacents.contains(adjacent)) {
        spdlog::debug("updateAdjacent: adding edge between {} and {}", id, adjacent);
        adjacents[adjacent] = weight;
    } else {
        spdlog::debug("updateAdjacent: updating edge between {} and {}", id, adjacent);
        adjacents[adjacent] += weight;
    }
}

template <typename T>
void WeightedNode<T>::removeAdjacent(int adjacent) {
    adjacents.erase(adjacent);
}

template <typename T>
T WeightedNode<T>::getAttributes() const {
    if (attributes == nullptr) {
        throw std::runtime_error("nullptr dereference: Attributes not set");
    } else {
        return *attributes;
    }
}

template <typename T>
std::shared_ptr<T> WeightedNode<T>::getAttributesPtr() const {
    if (attributes == nullptr) {
        spdlog::warn("Attributes not set");
    }
    return attributes;
}

template <typename T>
void WeightedNode<T>::setAttributes(T attributes){
    this->attributes = std::make_shared<T>(attributes);
}

template <typename T>
void WeightedNode<T>::clear() {
    id = UNUSED_ID;
    adjacents.clear();
    attributes.reset();
}

template <typename T>
WeightedGraph<T>::WeightedGraph() {
    usedNodes = std::unordered_set<int>();
    nodes = std::vector<WeightedNode<T>>();
}

template <typename T>
WeightedGraph<T>::WeightedGraph(std::string filePath, FileExtension extName) {
    usedNodes = std::unordered_set<int>();
    readGraph(filePath, extName);
}

template <typename T>
WeightedGraph<T>& WeightedGraph<T>::operator=(const WeightedGraph<T>& graph) {
    usedNodes = graph.usedNodes;
    nodes = graph.nodes;
    return *this;
}

template <typename T>
WeightedNode<T> WeightedGraph<T>::getNode(int id) const {
    return nodes[id];
}

template <typename T>
void WeightedGraph<T>::setNode(int id) {
    if (id >= static_cast<int>(nodes.size())) {
        nodes.resize(id + 1);
    }
    nodes[id] = WeightedNode<T>(id);
    usedNodes.insert(id);
}

template <typename T>
void WeightedGraph<T>::setNode(WeightedNode<T> &node) {
    // If the ID is not initialized, add it to the end
    const int nodeId = (!node.isUsed()) ? nodes.size() : node.getId();
    if (static_cast<int>(nodeId >= static_cast<int>(nodes.size()))) {
        nodes.resize(nodeId + 1);
    }
    nodes[nodeId] = node;
    usedNodes.insert(nodeId);
}

template <typename T>
void WeightedGraph<T>::removeNode(int id) {
    nodes[id].clear();
    usedNodes.erase(id);
}

template <typename T>
void WeightedGraph<T>::addEdge(int src, int dst, double weight) {
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
void WeightedGraph<T>::removeEdge(int src, int dst) {
    const int maxId = std::max(src, dst);
    if (maxId >= static_cast<int>(nodes.size())) {
        throw std::out_of_range("Node does not exist");
    }
    nodes[src].removeAdjacent(dst);
    nodes[dst].removeAdjacent(src);
}

template <typename T>
double WeightedGraph<T>::getWeight(int src, int dst) const {
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
void WeightedGraph<T>::setWeight(int src, int dst, double weight) {
    const int maxId = std::max(src, dst);
    if (maxId >= static_cast<int>(nodes.size())) {
        throw std::out_of_range("Node does not exist, use setNode() instead");
    }
    nodes[src].setAdjacent(dst, weight);
    nodes[dst].setAdjacent(src, weight);
}

template <typename T>
void WeightedGraph<T>::addWeight(int src, int dst, double weight) {
    const int maxId = std::max(src, dst);
    if (maxId >= static_cast<int>(nodes.size())) {
        throw std::out_of_range("Node does not exist");
    }
    nodes[src].updateAdjacent(dst, weight);
    nodes[dst].updateAdjacent(src, weight);
}

template <typename T>
const std::unordered_map<int, double>& WeightedGraph<T>::getAdjacents(int id) const {
    if (id >= static_cast<int>(nodes.size())) {
        throw std::out_of_range("Node does not exist");
    }
    return nodes[id].getAdjacents();
}

template <typename T>
WeightedGraph<T> WeightedGraph<T>::getSubgraph(std::unordered_set<int> indices) const {
    WeightedGraph<T> subgraph;
    // copy necessary nodes
    for (auto idx : indices) {
        if (idx >= static_cast<int>(nodes.size())) {
            throw std::out_of_range("Node does not exist");
        }
        WeightedNode<T> node = nodes[idx];
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
void WeightedGraph<T>::organize() {
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
        auto newNode = WeightedNode<T>(newId);

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
T WeightedGraph<T>::getAttributes(int id) const {
    return nodes[id].getAttributes();
}

template <typename T>
void WeightedGraph<T>::setAttributes(int id, T attributes) {
    nodes[id].setAttributes(attributes);
}

template <typename T>
size_t WeightedGraph<T>::size() const {
    return usedNodes.size();
}

template <typename T>
void WeightedGraph<T>::readGraph(std::string filePath, FileExtension extName) {
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
void WeightedGraph<T>::readGraphHelper(std::string filePath, IGraphParser &parser) {
    for (auto &[src, dst, weight] : parser.parseWeightedGraph(filePath)) {
        addEdge(src, dst, weight);
    }
}

template <typename T>
void WeightedGraph<T>::writeGraph(std::string filePath, FileExtension extName) const {
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
void WeightedGraph<T>::writeGraphHelper(std::string filePath, IGraphWriter &writer, std::vector<WeightedEdgeObject> edges) const {
    writer.writeWeightedGraph(filePath, edges);
}

// Explicit instantiation
template class WeightedNode<int>;
template class WeightedNode<std::string>;
template class WeightedNode<std::any>;

template class WeightedGraph<int>;
template class WeightedGraph<std::string>;
template class WeightedGraph<std::any>;