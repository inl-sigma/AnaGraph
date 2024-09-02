#include "anagraph/components/unweighted_hetero_digraph.hpp"

#include <spdlog/spdlog.h>

#include <any>
#include <set>
#include <stdexcept>

namespace anagraph {
namespace graph_structure {

template <typename T>
HeteroDigraph<T>::HeteroDigraph(std::string filename, FileExtension extName) {
    readGraph(filename, extName);
}

template <typename T>
HeteroNode<T>& HeteroDigraph<T>::getNode(int id) {
    if (!nodes.contains(id)) {
        throw std::out_of_range("Node does not exist");
    }
    return nodes.at(id);
}

template <typename T>
void HeteroDigraph<T>::setNode(int id) {
    nodes.insert({id, HeteroNode<T>(id)});
}

template <typename T>
void HeteroDigraph<T>::setNode(HeteroNode<T> &node) {
    const int nodeId = node.getId();
    nodes.insert({nodeId, node});
}

template <typename T>
void HeteroDigraph<T>::removeNode(int id) {
    nodes.erase(id);
}

template <typename T>
std::unordered_set<int> HeteroDigraph<T>::getIds() const {
    std::unordered_set<int> ids;
    for (auto &[id, _] : nodes) {
        ids.insert(id);
    }
    return ids;
}

template <typename T>
void HeteroDigraph<T>::setEdge(int src, int dst) {
    if (!nodes.contains(src)) {
        setNode(src);
    }
    if (!nodes.contains(dst)) {
        setNode(dst);
    }
    nodes[src].setAdjacentNode(getNode(dst));
}

template <typename T>
void HeteroDigraph<T>::removeEdge(int src, int dst) {
    if (!nodes.contains(src) || !nodes.contains(dst)) {
        throw std::out_of_range("Node does not exist");
    }
    nodes[src].removeAdjacent(dst);
}

template <typename T>
const std::unordered_set<int> HeteroDigraph<T>::getAdjacents(int id) const {
    return nodes.at(id).getAdjacents();
}

template <typename T>
HeteroDigraph<T> HeteroDigraph<T>::getSubgraph(std::unordered_set<int> indices) const {
    HeteroDigraph<T> subgraph;
    subgraph.nodes = this->nodes;

    // remove unnecessary edges
    for (int idx : indices) {
        const auto& adjacents = getAdjacents(idx);
        for (int adj : adjacents) {
            if (!indices.contains(adj)) {
                subgraph.removeEdge(idx, adj);
            }
        }
    }

    // remove unnecessary nodes
    for (int idx : getIds()) {
        if (!indices.contains(idx)) {
            subgraph.removeNode(idx);
        }
    }
    return subgraph;
}

template <typename T>
void HeteroDigraph<T>::reorganize() {
    spdlog::debug("called reorganize");

    // Create a map from old id to new id
    spdlog::debug("create idMap and update usedNodes");
    std::unordered_map<int, int> idMap;
    int newId = 0;
    for (auto &[oldId, _] : nodes) {
        idMap[oldId] = newId;
        nodes[newId] = std::move(nodes[oldId]);
        nodes[newId].setId(newId);
        newId++;
    }

    // Update the adjacent nodes
    spdlog::debug("update adjacents");
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

template <typename T>
T HeteroDigraph<T>::getAttributes(int id) const {
    return nodes.at(id).getAttributes();
}

template <typename T>
void HeteroDigraph<T>::setAttributes(int id, T attributes) {
    nodes[id].setAttributes(attributes);
}

template <typename T>
size_t HeteroDigraph<T>::size() const {
    return nodes.size();
}

template <typename T>
void HeteroDigraph<T>::readGraph(std::string filename, FileExtension extName) {
    switch (extName) {
    case FileExtension::TXT: {
        TextGraphParser parser;
        readGraphHelper(filename, parser);
    }
        break;

    case FileExtension::CSV: {
        CSVGraphParser parser;
        readGraphHelper(filename, parser);
    }
        break;

    // case FileExtension::GML:
    //     readGraphHelper(filename, GMLGraphParser());
    //     break;
    
    default:
        throw std::invalid_argument("Invalid file extension");
        break;
    }
}

template <typename T>
void HeteroDigraph<T>::writeGraph(std::string filename, FileExtension extName) const {
    std::vector<EdgeObject> edges;
    for (auto &[src, node] : nodes) {
        for (int dst : node.getAdjacents()) {
            edges.emplace_back(EdgeObject(src, dst));
        }
    }

    switch (extName) {
    case FileExtension::TXT: {
        TextGraphWriter writer;
        writeGraphHelper(filename, writer, edges);
    }
        break;

    case FileExtension::CSV: {
        CSVGraphWriter writer;
        writeGraphHelper(filename, writer, edges);
    }
        break;

    // case FileExtension::GML:
    //     writeGraphHelper(filename, GMLGraphWriter());
    //     break;

    default:
        throw std::invalid_argument("Invalid file extension");
        break;
    }
}

template <typename T>
void HeteroDigraph<T>::readGraphHelper(std::string filename, IGraphParser &parser) {
    for (auto &[src, dst] : parser.parseGraph(filename)) {
        setEdge(src, dst);
    }
}

template <typename T>
void HeteroDigraph<T>::writeGraphHelper(std::string filename, IGraphWriter &writer, std::vector<EdgeObject> &edges) const {
    writer.writeGraph(filename, edges);
}

template class HeteroDigraph<int>;
template class HeteroDigraph<std::string>;
template class HeteroDigraph<std::any>;

} // namespace graph_structure
} // namespace anagraph