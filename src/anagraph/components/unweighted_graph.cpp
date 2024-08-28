#include "anagraph/components/unweighted_graph.hpp"

#include <spdlog/spdlog.h>

#include <set>

namespace anagraph {

const int Node::UNUSED_ID = -1;

int Node::getId() const {
    return id;
}

void Node::setId(int id) {
    if (id < 0) {
        spdlog::warn("Node ID cannot be negative. ID not set.");
    } else {
        this->id = id;
    }
}

bool Node::isUsed() const {
    return id != UNUSED_ID;
}

const std::unordered_set<int>& Node::getAdjacents() const {
    return adjacents;
}

void Node::setAdjacent(int adjacent) {
    this->adjacents.insert(adjacent);
}

void Node::removeAdjacent(int adjacent) {
    this->adjacents.erase(adjacent);
}

void Node::clear() {
    id = UNUSED_ID;
    adjacents.clear();
}

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

Graph::Graph(std::string filePath, FileExtension extName) {
    readGraph(filePath, extName);
}

Node Graph::getNode(int id) const {
    return digraph.getNode(id);
}

void Graph::setNode(int id) {
    digraph.setNode(id);
}

void Graph::setNode(Node &node) {
    digraph.setNode(node);
}

void Graph::removeNode(int id) {
    digraph.removeNode(id);
}

std::unordered_set<int> Graph::getIds() const {
    return digraph.getIds();
}

void Graph::addEdge(int src, int dst) {
    digraph.addEdge(src, dst);
    digraph.addEdge(dst, src);
}

void Graph::removeEdge(int src, int dst) {
    digraph.removeEdge(src, dst);
    digraph.removeEdge(dst, src);
}

const std::unordered_set<int> Graph::getAdjacents(int id) const {
    return digraph.getAdjacents(id);
}

Graph Graph::getSubgraph(std::unordered_set<int> indices) const {
    Graph subgraph;
    subgraph.digraph = digraph.getSubgraph(indices);
    return subgraph;
}

void Graph::reorganize() {
    digraph.reorganize();
}

Digraph Graph::toDigraph() const {
    return digraph;
}

size_t Graph::size() const {
    return digraph.size();
}

void Graph::readGraph(std::string filePath, FileExtension extName) {
    digraph.readGraph(filePath, extName);
    Digraph deepCopy = digraph;
    for (auto id : digraph.getIds()) {
        for (auto adj : deepCopy.getAdjacents(id)) {
            digraph.addEdge(adj, id);
        }
    }
}

void Graph::writeGraph(std::string filePath, FileExtension extName) const {
    auto digraph = toDigraph();
    for (auto id : digraph.getIds()) {
        for (auto adj : digraph.getAdjacents(id)) {
            if (id > adj) {
                digraph.removeEdge(id, adj);
            }
        }
    }
    digraph.writeGraph(filePath, extName);
}

} // namespace anagraph