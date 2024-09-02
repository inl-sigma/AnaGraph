#include "anagraph/components/weighted_hetero_digraph.hpp"

#include <spdlog/spdlog.h>

#include <any>
#include <set>
#include <stdexcept>

namespace anagraph {
namespace graph_structure {

template <typename T>
WeightedHeteroDigraph<T>::WeightedHeteroDigraph(std::string filePath, FileExtension extName) {
    readGraph(filePath, extName);
}

template <typename T>
WeightedHeteroNode<T>& WeightedHeteroDigraph<T>::getNode(int id) {
    return nodes.at(id);
}

template <typename T>
void WeightedHeteroDigraph<T>::setNode(int id) {
    nodes.insert({id, WeightedHeteroNode<T>(id)});
}

template <typename T>
void WeightedHeteroDigraph<T>::setNode(WeightedHeteroNode<T> &node) {
    const int nodeId = node.getId();
    nodes.insert({nodeId, node});
}

template <typename T>
void WeightedHeteroDigraph<T>::removeNode(int id) {
    nodes.erase(id);
}

template <typename T>
std::unordered_set<int> WeightedHeteroDigraph<T>::getIds() const {
    std::unordered_set<int> ids;
    for (auto &[id, _] : nodes) {
        ids.insert(id);
    }
    return ids;
}

template <typename T>
void WeightedHeteroDigraph<T>::setEdge(int src, int dst, double weight) {
    if (!nodes.contains(src)) {
        setNode(src);
    }
    if (!nodes.contains(dst)) {
        setNode(dst);
    }
    nodes[src].setAdjacentNode(getNode(dst), weight);
}

template <typename T>
void WeightedHeteroDigraph<T>::removeEdge(int src, int dst) {
    if (!nodes.contains(src)) {
        throw std::out_of_range("Node does not exist");
    }
    nodes[src].removeAdjacent(dst);
}

template <typename T>
double WeightedHeteroDigraph<T>::getWeight(int src, int dst) const {
    if (!nodes.contains(src) || !nodes.contains(dst)) {
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
    if (!nodes.contains(src)) {
        setNode(src);
    }
    nodes[src].setAdjacent(dst, weight);
}

template <typename T>
void WeightedHeteroDigraph<T>::addWeight(int src, int dst, double weight) {
    if (!nodes.contains(src)) {
        setNode(src);
    }
    nodes[src].updateAdjacent(dst, weight);
}

template <typename T>
const std::unordered_map<int, double> WeightedHeteroDigraph<T>::getAdjacents(int id) const {
    if (!nodes.contains(id)) {
        throw std::out_of_range("Node does not exist");
    }
    return nodes.at(id).getAdjacents();
}

template <typename T>
WeightedHeteroDigraph<T> WeightedHeteroDigraph<T>::getSubgraph(std::unordered_set<int> indices) const {
    WeightedHeteroDigraph<T> subgraph;
    subgraph.nodes = nodes;
    // remove unnecessary edges
    for (auto idx : indices) {
        const auto &adjacents = getAdjacents(idx);
        for (auto [adj, weight] : adjacents) {
            if (!indices.contains(adj)) {
                subgraph.removeEdge(idx, adj);
            }
        }
    }

    // remove unnecessary nodes
    for (const auto &[id, _] : nodes) {
        if (!indices.contains(id)) {
            subgraph.removeNode(id);
        }
    }
    return subgraph;
}

template <typename T>
void WeightedHeteroDigraph<T>::reorganize() {
    spdlog::debug("called reorganize");

    spdlog::debug("create idMap and update usedNodes");
    std::unordered_map<int, int> idMap;
    int newId = 0;
    for (auto [oldId, _] : nodes) {
        idMap[oldId] = newId;
        nodes[newId] = std::move(nodes[oldId]);
        nodes[newId].setId(newId);
        newId++;
    }

    // Update the adjacent nodes
    spdlog::debug("update adjacents");
    for (int id : getIds()) {
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

template <typename T>
T WeightedHeteroDigraph<T>::getAttributes(int id) const {
    return nodes.at(id).getAttributes();
}

template <typename T>
void WeightedHeteroDigraph<T>::setAttributes(int id, T attributes) {
    nodes[id].setAttributes(attributes);
}

template <typename T>
size_t WeightedHeteroDigraph<T>::size() const {
    return nodes.size();
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
        setEdge(src, dst, weight);
    }
}

template <typename T>
void WeightedHeteroDigraph<T>::writeGraph(std::string filePath, FileExtension extName) const {
    // convert the graph to a list of edges
    // note : implement as function in weighted_graph.hpp if needed
    std::vector<WeightedEdgeObject> edges;
    for (auto &[src, node] : nodes) {
        for (auto [dst, weight] : node.getAdjacents()) {
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