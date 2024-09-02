#include "anagraph/components/weighted_hetero_graph.hpp"

#include <any>

namespace anagraph {
namespace graph_structure {

template <typename T>
WeightedHeteroGraph<T>::WeightedHeteroGraph(std::string filePath, FileExtension extName) {
    readGraph(filePath, extName);
}

template <typename T>
WeightedHeteroNode<T>& WeightedHeteroGraph<T>::getNode(int id) {
    return digraph.getNode(id);
}

template <typename T>
void WeightedHeteroGraph<T>::setNode(int id) {
    digraph.setNode(id);
}

template <typename T>
void WeightedHeteroGraph<T>::setNode(WeightedHeteroNode<T> &node) {
    digraph.setNode(node);
}

template <typename T>
void WeightedHeteroGraph<T>::removeNode(int id) {
    digraph.removeNode(id);
}

template <typename T>
std::unordered_set<int> WeightedHeteroGraph<T>::getIds() const {
    return digraph.getIds();
}

template <typename T>
void WeightedHeteroGraph<T>::addEdge(int src, int dst, double weight) {
    digraph.addEdge(src, dst, weight);
    digraph.addEdge(dst, src, weight);
}

template <typename T>
void WeightedHeteroGraph<T>::removeEdge(int src, int dst) {
    digraph.removeEdge(src, dst);
    digraph.removeEdge(dst, src);
}

template <typename T>
double WeightedHeteroGraph<T>::getWeight(int src, int dst) const { 
    return digraph.getWeight(src, dst);
}

template <typename T>
void WeightedHeteroGraph<T>::setWeight(int src, int dst, double weight) {
    digraph.setWeight(src, dst, weight);
    digraph.setWeight(dst, src, weight);
}

template <typename T>
void WeightedHeteroGraph<T>::addWeight(int src, int dst, double weight) {
    digraph.addWeight(src, dst, weight);
    digraph.addWeight(dst, src, weight);
}

template <typename T>
const std::unordered_map<int, double> WeightedHeteroGraph<T>::getAdjacents(int id) const {
    return digraph.getAdjacents(id);
}

template <typename T>
WeightedHeteroGraph<T> WeightedHeteroGraph<T>::getSubgraph(std::unordered_set<int> indices) const {
    WeightedHeteroGraph<T> subgraph;
    subgraph.digraph = digraph.getSubgraph(indices);
    return subgraph;
}

template <typename T>
void WeightedHeteroGraph<T>::reorganize() {
    digraph.reorganize();
}

template <typename T>
T WeightedHeteroGraph<T>::getAttributes(int id) const {
    return digraph.getAttributes(id);
}

template <typename T>
void WeightedHeteroGraph<T>::setAttributes(int id, T attributes) {
    digraph.setAttributes(id, attributes);
}

template <typename T>
size_t WeightedHeteroGraph<T>::size() const {
    return digraph.size();
}

template <typename T>
WeightedHeteroDigraph<T> WeightedHeteroGraph<T>::toDigraph() const {
    return digraph;
}

template <typename T>
void WeightedHeteroGraph<T>::readGraph(std::string filePath, FileExtension extName) {
    digraph.readGraph(filePath, extName);
    WeightedHeteroDigraph deepCopy = digraph;
    for (auto id : digraph.getIds()) {
        for (auto &[adj, weight] : deepCopy.getAdjacents(id)) {
            digraph.addEdge(adj, id, weight);
        }
    }
}

template <typename T>
void WeightedHeteroGraph<T>::writeGraph(std::string filePath, FileExtension extName) const {
    WeightedHeteroDigraph<T> digraph = toDigraph();
    for (auto id : digraph.getIds()) {
        auto adjacents = digraph.getAdjacents(id);
        for (auto [adj, _] : adjacents) {
            if (id > adj) {
                digraph.removeEdge(id, adj);
            }
        }
    }
    digraph.writeGraph(filePath, extName);
}

template class WeightedHeteroGraph<int>;
template class WeightedHeteroGraph<std::string>;
template class WeightedHeteroGraph<std::any>;

} // namespace graph_structure
} // namespace anagraph