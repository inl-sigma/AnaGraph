#include "weighted_graph.hpp"

#include "graph_parser.hpp"
#include "graph_utils.hpp"
#include "graph_writer.hpp"

#include <spdlog/spdlog.h>

#include <fstream>
#include <set>
#include <unordered_map>
#include <unordered_set>

WeightedNode::WeightedNode() {
    id = UNUSED_ID;
    adjacents = std::unordered_map<int, double>();
}

WeightedNode::WeightedNode(int id) {
    setId(id);
    adjacents = std::unordered_map<int, double>();
}

WeightedNode& WeightedNode::operator=(const WeightedNode& node) {
    id = node.id;
    adjacents = node.adjacents;
    return *this;
}

const int WeightedNode::UNUSED_ID = -1;

int WeightedNode::getId() const{
    return id;
}

void WeightedNode::setId(int id) {
    if (id < 0) {
        spdlog::warn("setId: negative id({}) detected, use clear() to reset node", id);
    } else {
        this->id = id;
    }
}

bool WeightedNode::isUsed() const {
    return id != UNUSED_ID;
}

const std::unordered_map<int, double>& WeightedNode::getAdjacents() const {
    return adjacents;
}

void WeightedNode::setAdjacent(int adjacent, double weight) {
    adjacents[adjacent] = weight;
}

void WeightedNode::updateAdjacent(int adjacent, double weight) {
    if (!adjacents.contains(adjacent)) {
        spdlog::debug("updateAdjacent: adding edge between {} and {}", id, adjacent);
        adjacents[adjacent] = weight;
    } else {
        spdlog::debug("updateAdjacent: updating edge between {} and {}", id, adjacent);
        adjacents[adjacent] += weight;
    }
}

void WeightedNode::removeAdjacent(int adjacent) {
    adjacents.erase(adjacent);
}

void WeightedNode::clear() {
    id = UNUSED_ID;
    adjacents.clear();
}

WeightedDigraph::WeightedDigraph() {
    usedNodes = std::unordered_set<int>();
    nodes = std::vector<WeightedNode>();
}

WeightedDigraph::WeightedDigraph(std::string filePath, FileExtension extName) {
    usedNodes = std::unordered_set<int>();
    readGraph(filePath, extName);
}

WeightedDigraph& WeightedDigraph::operator=(const WeightedDigraph& graph) {
    usedNodes = graph.usedNodes;
    nodes = graph.nodes;
    return *this;
}

WeightedNode WeightedDigraph::getNode(int id) const {
    return nodes[id];
}

void WeightedDigraph::setNode(int id) {
    if (id >= static_cast<int>(nodes.size())) {
        nodes.resize(id + 1);
    }
    nodes[id] = WeightedNode(id);
    usedNodes.insert(id);
}

void WeightedDigraph::setNode(WeightedNode &node) {
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

void WeightedDigraph::removeNode(int id) {
    nodes[id].clear();
    usedNodes.erase(id);
}

std::unordered_set<int> WeightedDigraph::getIds() const {
    return usedNodes;
}

void WeightedDigraph::addEdge(int src, int dst, double weight) {
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
}

void WeightedDigraph::removeEdge(int src, int dst) {
    const int maxId = std::max(src, dst);
    if (maxId >= static_cast<int>(nodes.size())) {
        throw std::out_of_range("Node does not exist");
    }
    nodes[src].removeAdjacent(dst);
}

double WeightedDigraph::getWeight(int src, int dst) const {
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

void WeightedDigraph::setWeight(int src, int dst, double weight) {
    const int maxId = std::max(src, dst);
    if (maxId >= static_cast<int>(nodes.size())) {
        throw std::out_of_range("Node does not exist, use setNode() instead");
    }
    nodes[src].setAdjacent(dst, weight);
}

void WeightedDigraph::addWeight(int src, int dst, double weight) {
    const int maxId = std::max(src, dst);
    if (maxId >= static_cast<int>(nodes.size())) {
        throw std::out_of_range("Node does not exist");
    }
    nodes[src].updateAdjacent(dst, weight);
}

const std::unordered_map<int, double> WeightedDigraph::getAdjacents(int id) const {
    if (id >= static_cast<int>(nodes.size())) {
        throw std::out_of_range("Node does not exist");
    }
    return nodes[id].getAdjacents();
}

WeightedDigraph WeightedDigraph::getSubgraph(std::unordered_set<int> indices) const {
    WeightedDigraph subgraph;
    // copy necessary nodes
    for (auto idx : indices) {
        if (idx >= static_cast<int>(nodes.size())) {
            throw std::out_of_range("Node does not exist");
        }
        WeightedNode node = nodes[idx];
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

void WeightedDigraph::reorganize() {
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
        auto newNode = WeightedNode(id);

        // set the adjacents
        for (auto [oldAdj, weight] : oldNode.getAdjacents()) {
            if (idMap.contains(oldAdj)) {
                newNode.setAdjacent(idMap[oldAdj], weight);
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

size_t WeightedDigraph::size() const {
    return usedNodes.size();
}

void WeightedDigraph::readGraph(std::string filePath, FileExtension extName) {
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

void WeightedDigraph::readGraphHelper(std::string filePath, IGraphParser &parser) {
    for (auto &[src, dst, weight] : parser.parseWeightedGraph(filePath)) {
        addEdge(src, dst, weight);
    }
}

void WeightedDigraph::writeGraph(std::string filePath, FileExtension extName) const {
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

void WeightedDigraph::writeGraphHelper(std::string filePath, IGraphWriter &writer, std::vector<WeightedEdgeObject> edges) const {
    writer.writeWeightedGraph(filePath, edges);
}

WeightedGraph::WeightedGraph() {
    digraph = WeightedDigraph();
}

WeightedGraph::WeightedGraph(std::string filePath, FileExtension extName) {
    readGraph(filePath, extName);
}

WeightedGraph& WeightedGraph::operator=(const WeightedGraph& graph) {
    digraph = graph.digraph;
    return *this;
}

WeightedNode WeightedGraph::getNode(int id) const {
    return digraph.getNode(id);
}

void WeightedGraph::setNode(int id) {
    digraph.setNode(id);
}

void WeightedGraph::setNode(WeightedNode &node) {
    digraph.setNode(node);
}

void WeightedGraph::removeNode(int id) {
    digraph.removeNode(id);
}

std::unordered_set<int> WeightedGraph::getIds() const {
    return digraph.getIds();
}

void WeightedGraph::addEdge(int src, int dst, double weight) {
    digraph.addEdge(src, dst, weight);
    digraph.addEdge(dst, src, weight);
}

void WeightedGraph::removeEdge(int src, int dst) {
    digraph.removeEdge(src, dst);
    digraph.removeEdge(dst, src);
}

double WeightedGraph::getWeight(int src, int dst) const {
    return digraph.getWeight(src, dst);
}

void WeightedGraph::setWeight(int src, int dst, double weight) {
    digraph.setWeight(src, dst, weight);
    digraph.setWeight(dst, src, weight);
}

void WeightedGraph::addWeight(int src, int dst, double weight) {
    digraph.addWeight(src, dst, weight);
    digraph.addWeight(dst, src, weight);
}

const std::unordered_map<int, double> WeightedGraph::getAdjacents(int id) const {
    return digraph.getAdjacents(id);
}

WeightedGraph WeightedGraph::getSubgraph(std::unordered_set<int> indices) const {
    WeightedGraph subgraph;
    subgraph.digraph = digraph.getSubgraph(indices);
    return subgraph;
}

void WeightedGraph::reorganize() {
    digraph.reorganize();
}

size_t WeightedGraph::size() const {
    return digraph.size();
}

void WeightedGraph::readGraph(std::string filePath, FileExtension extName) {
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

void WeightedGraph::readGraphHelper(std::string filePath, IGraphParser &parser) {
    for (auto &[src, dst, weight] : parser.parseWeightedGraph(filePath)) {
        addEdge(src, dst, weight);
    }
}

void WeightedGraph::writeGraph(std::string filePath, FileExtension extName) const {
    // convert the graph to a list of edges
    // note : implement as function in weighted_graph.hpp if needed

    std::vector<WeightedEdgeObject> edges;
    for (int src : digraph.getIds()) {
        for (auto [dst, weight] : getAdjacents(src)) {
            if (src <= dst) {
                // avoid duplicate edges, only add the edge if src <= dst
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

void WeightedGraph::writeGraphHelper(std::string filePath, IGraphWriter &writer, std::vector<WeightedEdgeObject> edges) const {
    writer.writeWeightedGraph(filePath, edges);
}