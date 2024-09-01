#include "anagraph/components/weighted_hetero_digraph.hpp"

#include <spdlog/spdlog.h>

#include <any>
#include <set>
#include <stdexcept>

namespace anagraph {
namespace graph_structure {

template <typename T>
WeightedHeteroDigraph<T>::WeightedHeteroDigraph() {
    usedNodes = std::unordered_set<int>();
    nodes = std::vector<WeightedHeteroNode<T>>();
}

template <typename T>
WeightedHeteroDigraph<T>::WeightedHeteroDigraph(std::string filePath, FileExtension extName) {
    usedNodes = std::unordered_set<int>();
    readGraph(filePath, extName);
}

template <typename T>
WeightedHeteroDigraph<T>& WeightedHeteroDigraph<T>::operator=(const WeightedHeteroDigraph<T>& graph) {
    usedNodes = graph.usedNodes;
    nodes = graph.nodes;
    return *this;
}

template <typename T>
WeightedHeteroNode<T> WeightedHeteroDigraph<T>::getNode(int id) const {
    return nodes[id];
}

template <typename T>
void WeightedHeteroDigraph<T>::setNode(int id) {
    if (id >= static_cast<int>(nodes.size())) {
        nodes.resize(id + 1);
    }
    nodes[id] = WeightedHeteroNode<T>(id);
    usedNodes.insert(id);
}

template <typename T>
void WeightedHeteroDigraph<T>::setNode(WeightedHeteroNode<T> &node) {
    // If the ID is not initialized, add it to the end
    const int nodeId = (!node.isUsed()) ? nodes.size() : node.getId();
    if (static_cast<int>(nodeId >= static_cast<int>(nodes.size()))) {
        nodes.resize(nodeId + 1);
    }
    nodes[nodeId] = node;
    usedNodes.insert(nodeId);
}

template <typename T>
void WeightedHeteroDigraph<T>::removeNode(int id) {
    nodes[id].clear();
    usedNodes.erase(id);
}

template <typename T>
std::unordered_set<int> WeightedHeteroDigraph<T>::getIds() const {
    return usedNodes;
}

template <typename T>
void WeightedHeteroDigraph<T>::addEdge(int src, int dst, double weight) {
    const int maxId = std::max(src, dst);
    if (maxId >= static_cast<int>(nodes.size())) {
        setNode(maxId);
    }
    const int minId = std::min(src, dst);
    if (!usedNodes.contains(minId)) {
        setNode(minId);
    }
    nodes[src].updateAdjacent(dst, weight);
}

template <typename T>
void WeightedHeteroDigraph<T>::removeEdge(int src, int dst) {
    const int maxId = std::max(src, dst);
    if (maxId >= static_cast<int>(nodes.size())) {
        throw std::out_of_range("Node does not exist");
    }
    nodes[src].removeAdjacent(dst);
}

template <typename T>
double WeightedHeteroDigraph<T>::getWeight(int src, int dst) const {
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
void WeightedHeteroDigraph<T>::setWeight(int src, int dst, double weight) {
    const int maxId = std::max(src, dst);
    if (maxId >= static_cast<int>(nodes.size())) {
        throw std::out_of_range("Node does not exist, use setNode() instead");
    }
    nodes[src].setAdjacent(dst, weight);
}

template <typename T>
void WeightedHeteroDigraph<T>::addWeight(int src, int dst, double weight) {
    const int maxId = std::max(src, dst);
    if (maxId >= static_cast<int>(nodes.size())) {
        throw std::out_of_range("Node does not exist");
    }
    nodes[src].updateAdjacent(dst, weight);
}

template <typename T>
const std::unordered_map<int, double> WeightedHeteroDigraph<T>::getAdjacents(int id) const {
    if (id >= static_cast<int>(nodes.size())) {
        throw std::out_of_range("Node does not exist");
    }
    return nodes[id].getAdjacents();
}

template <typename T>
WeightedHeteroDigraph<T> WeightedHeteroDigraph<T>::getSubgraph(std::unordered_set<int> indices) const {
    WeightedHeteroDigraph<T> subgraph;
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
void WeightedHeteroDigraph<T>::reorganize() {
    spdlog::debug("called reorganize");

    // Create a map from old id to new id
    std::set<int> oldNodes = std::set<int>(this->usedNodes.begin(), this->usedNodes.end());
    spdlog::debug("clear usedNodes");
    usedNodes.clear();

    spdlog::debug("create idMap and update usedNodes");
    std::unordered_map<int, int> idMap;
    int newId = 0;

    for (int oldId : oldNodes) {
        idMap[oldId] = newId;
        nodes[newId] = std::move(nodes[oldId]);
        usedNodes.insert(newId);
        newId++;
    }

    // Update the adjacent nodes
    spdlog::debug("update adjacents");
    for (int id = 0; id < newId; id++) {
        spdlog::debug("update adjacents for {}", id);
        auto &newNode = nodes[id];

        auto adjacents = newNode.getAdjacents();
        std::unordered_map<int, double> newAdjacents = std::unordered_map<int, double>();

        // update the adjacents
        for (auto &[adj, weight] : adjacents) {
            spdlog::debug("update adjacent {} from {}", adj, id);
            newAdjacents[idMap[adj]] = weight;
        }

        // remove old adjacents
        for (auto [adj, weight] : adjacents) {
            spdlog::debug("remove adjacent {} from {}", adj, id);
            newNode.removeAdjacent(adj);
        }

        // add new adjacents
        for (auto &[adj, weight] : newAdjacents) {
            spdlog::debug("add adjacent {} to {}", adj, id);
            newNode.setAdjacent(adj, weight);
        }
    }

    // resize the nodes
    spdlog::debug("resize nodes");
    nodes.resize(idMap.size());
}

template <typename T>
T WeightedHeteroDigraph<T>::getAttributes(int id) const {
    return nodes[id].getAttributes();
}

template <typename T>
void WeightedHeteroDigraph<T>::setAttributes(int id, T attributes) {
    nodes[id].setAttributes(attributes);
}

template <typename T>
size_t WeightedHeteroDigraph<T>::size() const {
    return usedNodes.size();
}

template <typename T>
void WeightedHeteroDigraph<T>::readGraph(std::string filePath, FileExtension extName) {
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
void WeightedHeteroDigraph<T>::readGraphHelper(std::string filePath, IGraphParser &parser) {
    for (auto &[src, dst, weight] : parser.parseWeightedGraph(filePath)) {
        addEdge(src, dst, weight);
    }
}

template <typename T>
void WeightedHeteroDigraph<T>::writeGraph(std::string filePath, FileExtension extName) const {
    // convert the graph to a list of edges
    // note : implement as function in weighted_graph.hpp if needed
    std::vector<WeightedEdgeObject> edges;
    for (int src : usedNodes) {
        for (auto [dst, weight] : getAdjacents(src)) {
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

template <typename T>
void WeightedHeteroDigraph<T>::writeGraphHelper(std::string filePath, IGraphWriter &writer, std::vector<WeightedEdgeObject> &edges) const {
    writer.writeWeightedGraph(filePath, edges);
}

template class WeightedHeteroDigraph<int>;
template class WeightedHeteroDigraph<std::string>;
template class WeightedHeteroDigraph<std::any>;

} // namespace graph_structure
} // namespace anagraph