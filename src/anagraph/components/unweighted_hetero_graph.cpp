#include "anagraph/components/unweighted_hetero_graph.hpp"

#include <any>

namespace anagraph {
namespace graph_structure {

template <typename T>
HeteroGraph<T>::HeteroGraph(std::string filePath, FileExtension extName) {
    digraph.readGraph(filePath, extName);
}

template <typename T>
HeteroNode<T>& HeteroGraph<T>::getNode(int id) {
    return digraph.getNode(id);
}

template <typename T>
void HeteroGraph<T>::setNode(int id) {
    digraph.setNode(id);
}

template <typename T>
void HeteroGraph<T>::setNode(HeteroNode<T> &node) {
    digraph.setNode(node);
}

template <typename T>
void HeteroGraph<T>::removeNode(int id) {
    digraph.removeNode(id);
}

template <typename T>
std::unordered_set<int> HeteroGraph<T>::getIds() const {
    return digraph.getIds();
}

template <typename T>
void HeteroGraph<T>::setEdge(int src, int dst) {
    digraph.setEdge(src, dst);
    digraph.setEdge(dst, src);
}

template <typename T>
void HeteroGraph<T>::removeEdge(int src, int dst) {
    digraph.removeEdge(src, dst);
    digraph.removeEdge(dst, src);
}

template <typename T>
const std::unordered_set<int> HeteroGraph<T>::getAdjacents(int id) const {
    return digraph.getAdjacents(id);
}

template <typename T>
HeteroGraph<T> HeteroGraph<T>::getSubgraph(std::unordered_set<int> indices) const {
    HeteroGraph<T> subgraph;
    subgraph.digraph = digraph.getSubgraph(indices);
    return subgraph;
}

template <typename T>
void HeteroGraph<T>::reorganize() {
    digraph.reorganize();
}

template <typename T>
T HeteroGraph<T>::getAttributes(int id) const {
    return digraph.getAttributes(id);
}

template <typename T>
void HeteroGraph<T>::setAttributes(int id, T attributes) {
    digraph.setAttributes(id, attributes);
}

template <typename T>
HeteroDigraph<T> HeteroGraph<T>::toDigraph() const {
    return digraph;
}

template <typename T> 
size_t HeteroGraph<T>::size() const {
    return digraph.size();
}

template <typename T>
void HeteroGraph<T>::readGraph(std::string filename, FileExtension extName) {
    digraph.readGraph(filename, extName);
    HeteroDigraph<T> deepCopy = digraph;
    for (int id : digraph.getIds()) {
        for (int adj : deepCopy.getAdjacents(id)) {
            digraph.setEdge(adj, id);
        }
    }
}

template <typename T>
void HeteroGraph<T>::writeGraph(std::string filename, FileExtension extName) const {
    HeteroDigraph<T> digraph = toDigraph();
    for (int id : digraph.getIds()) {
        for (int adj : digraph.getAdjacents(id)) {
            if (id > adj) {
                digraph.removeEdge(id, adj);
            }
        }
    }
    digraph.writeGraph(filename, extName);
}

template class HeteroGraph<int>;
template class HeteroGraph<std::string>;
template class HeteroGraph<std::any>;

} // namespace graph_structure
} // namespace anagraph