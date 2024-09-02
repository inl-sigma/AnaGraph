#include "anagraph/components/weighted_supergraph.hpp"

#include <stdexcept>

namespace anagraph {
namespace graph_structure {

WeightedSupergraph::WeightedSupergraph(std::string filepath, FileExtension extName) {
    readGraph(filepath, extName);
}

void WeightedSupergraph::setNode(WeightedSupernode &node) {
    digraph.setNode(node);
}

WeightedSupernode& WeightedSupergraph::getNode(int id) {
    return digraph.getNode(id);
}

void WeightedSupergraph::setNode(int id) {
    digraph.setNode(id);
}

void WeightedSupergraph::removeNode(int id) {
    digraph.removeNode(id);
}

std::unordered_set<int> WeightedSupergraph::getIds() const {
    return digraph.getIds();
}

void WeightedSupergraph::setEdge(int src, int dst, double weight) {
    digraph.setEdge(src, dst, weight);
    digraph.setEdge(dst, src, weight);
}

void WeightedSupergraph::removeEdge(int src, int dst) {
    digraph.removeEdge(src, dst);
    digraph.removeEdge(dst, src);
}

void WeightedSupergraph::mergeNode(int first, int second, mergeLambda mergeFunc) {
    digraph.mergeNode(first, second, mergeFunc);
}

void WeightedSupergraph::mergeNode(int first, int second) {
    digraph.mergeNode(first, second);
}

void WeightedSupergraph::setMergeNodeFunction(mergeLambda mergeFunc) {
    digraph.setMergeNodeFunction(mergeFunc);
}

double WeightedSupergraph::getWeight(int src, int dst) const {
    return digraph.getWeight(src, dst);
}

void WeightedSupergraph::setWeight(int src, int dst, double weight) {
    digraph.setWeight(src, dst, weight);
    digraph.setWeight(dst, src, weight);
}

void WeightedSupergraph::addWeight(int src, int dst, double weight) {
    digraph.addWeight(src, dst, weight);
    digraph.addWeight(dst, src, weight);
}

int WeightedSupergraph::getParent(int id) const {
    return digraph.getParent(id);
}

void WeightedSupergraph::setParent(int child, int parent) {
    digraph.setParent(child, parent);
}

void WeightedSupergraph::updateParent(int child, int parent) {
    digraph.updateParent(child, parent);
}

void WeightedSupergraph::removeParent(int child) {
    digraph.removeParent(child);
}

std::unordered_set<int> WeightedSupergraph::getChildren(int id) const {
    return digraph.getChildren(id);
}

const std::unordered_map<int, double> WeightedSupergraph::getAdjacents(int id) const {
    return digraph.getAdjacents(id);
}

size_t WeightedSupergraph::size() const {
    return digraph.size();
}

WeightedSuperDigraph WeightedSupergraph::toDigraph() const {
    return digraph;
}

void WeightedSupergraph::readGraph(std::string filePath, FileExtension extName) {
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

void WeightedSupergraph::readEdgeHelper(std::string filePath, IGraphParser &parser) {
    // read normal edges from the file
    for (auto &[src, dst, weight] : parser.parseWeightedGraph(filePath)) {
        setEdge(src, dst, weight);
    }
}

void WeightedSupergraph::readHierarchyHelper(std::string filePath, IGraphParser &parser) {
    // read the hierarchical edges from the file
    for (auto &[parent, child] : parser.parseGraph(filePath)) {
        setParent(child, parent);
    }
}
void WeightedSupergraph::writeGraph(std::string filePath, FileExtension extName) const {
    std::vector<WeightedEdgeObject> normalEdges;
    std::vector<EdgeObject> hierarchicalEdges;
    for (int src : digraph.getIds()) {
        auto adjacents = digraph.getAdjacents(src);
        for (auto [dst, weight] : adjacents) {
            if (src <= dst) {
                normalEdges.push_back(WeightedEdgeObject(src, dst, weight));
            }
        }
        if (digraph.getParent(src) != WeightedSupernode::ROOT) {
            hierarchicalEdges.push_back(EdgeObject(digraph.getParent(src), src));
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