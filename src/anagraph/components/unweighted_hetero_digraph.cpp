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
HeteroNode<T> HeteroDigraph<T>::getNode(int id) const {
    return nodes[id];
}

template <typename T>
void HeteroDigraph<T>::setNode(int id) {
    if (id >= static_cast<int>(nodes.size())) {
        nodes.resize(id + 1);
    }
    nodes[id] = HeteroNode<T>(id);
    usedNodes.insert(id);
}

template <typename T>
void HeteroDigraph<T>::setNode(HeteroNode<T> &node) {
    // If the ID is not initialized, add it to the end
    const int nodeId = (!node.isUsed()) ? nodes.size() : node.getId();
    if (static_cast<int>(nodeId >= static_cast<int>(nodes.size()))) {
        nodes.resize(nodeId + 1);
    }
    nodes[nodeId] = node;
    usedNodes.insert(nodeId);
}

template <typename T>
void HeteroDigraph<T>::removeNode(int id) {
    nodes[id].clear();
    usedNodes.erase(id);
}

template <typename T>
std::unordered_set<int> HeteroDigraph<T>::getIds() const {
    return usedNodes;
}

template <typename T>
void HeteroDigraph<T>::addEdge(int src, int dst) {
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

template <typename T>
void HeteroDigraph<T>::removeEdge(int src, int dst) {
    const int maxId = std::max(src, dst);
    if (maxId >= static_cast<int>(nodes.size())) {
        throw std::out_of_range("Node does not exist");
    }
    nodes[src].removeAdjacent(dst);
}

template <typename T>
const std::unordered_set<int> HeteroDigraph<T>::getAdjacents(int id) const {
    if (id >= static_cast<int>(nodes.size())) {
        throw std::out_of_range("Node does not exist");
    }
    return nodes[id].getAdjacents();
}

template <typename T>
HeteroDigraph<T> HeteroDigraph<T>::getSubgraph(std::unordered_set<int> indices) const {
    HeteroDigraph<T> subgraph;
    // copy necessary nodes
    for (auto idx : indices) {
        if (idx >= static_cast<int>(nodes.size())) {
            throw std::out_of_range("Node does not exist");
        }
        HeteroNode<T> node = nodes[idx];
        subgraph.setNode(node);
    }
    
    // remove unnecessary edges
    for (int idx : indices) {
        const auto &adjacents = getAdjacents(idx);
        for (int adj : adjacents) {
            if (!indices.contains(adj)) {
                subgraph.removeEdge(idx, adj);
            }
        }
    }
    return subgraph;
}

template <typename T>
void HeteroDigraph<T>::reorganize() {
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
        std::unordered_set<int> newAdjacents = std::unordered_set<int>();

        // update the adjacents
        for (auto adj : adjacents) {
            spdlog::debug("update adjacent {} from {}", adj, id);
            newAdjacents.insert(idMap[adj]);
        }

        // remove old adjacents
        for (auto adj : adjacents) {
            spdlog::debug("remove adjacent {} from {}", adj, id);
            newNode.removeAdjacent(adj);
        }

        // add new adjacents
        for (auto &adj : newAdjacents) {
            spdlog::debug("add adjacent {} to {}", adj, id);
            newNode.setAdjacent(adj);
        }
    }

    // resize the nodes
    spdlog::debug("resize nodes");
    nodes.resize(idMap.size());
}

template <typename T>
T HeteroDigraph<T>::getAttributes(int id) const {
    return nodes[id].getAttributes();
}

template <typename T>
void HeteroDigraph<T>::setAttributes(int id, T attributes) {
    nodes[id].setAttributes(attributes);
}

template <typename T>
size_t HeteroDigraph<T>::size() const {
    return usedNodes.size();
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
    for (int src : usedNodes) {
        for (int dst : getAdjacents(src)) {
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
        addEdge(src, dst);
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