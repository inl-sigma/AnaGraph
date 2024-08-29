#include "anagraph/components/unweighted_hetero_node.hpp"

#include <any>
#include <stdexcept>
#include <string>

namespace anagraph {
namespace graph_structure {

template <typename T>
int HeteroNode<T>::getId() const {
    return node.getId();
}

template <typename T>
void HeteroNode<T>::setId(int id) {
    node.setId(id);
}

template <typename T>
bool HeteroNode<T>::isUsed() const {
    return node.isUsed();
}

template <typename T>
const std::unordered_set<int>& HeteroNode<T>::getAdjacents() const {
    return node.getAdjacents();
}

template <typename T>
void HeteroNode<T>::setAdjacent(int adjacent) {
    node.setAdjacent(adjacent);
}

template <typename T>
void HeteroNode<T>::removeAdjacent(int adjacent) {
    node.removeAdjacent(adjacent);
}

template <typename T>
T HeteroNode<T>::getAttributes() const {
    if (!isAttrEnabled) {
        throw std::runtime_error("Attributes are not enabled");
    }
    return attributes;
}

template <typename T>
void HeteroNode<T>::setAttributes(T attributes) {
    this->attributes = attributes;
    isAttrEnabled = true;
}

template <typename T>
void HeteroNode<T>::clear() {
    node.clear();
    attributes = T();
    isAttrEnabled = false;
}

template class HeteroNode<int>;
template class HeteroNode<std::string>;
template class HeteroNode<std::any>;

} // namespace graph_structure
} // namespace anagraph