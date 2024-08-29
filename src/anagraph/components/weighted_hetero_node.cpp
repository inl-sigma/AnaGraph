#include "anagraph/components/weighted_hetero_node.hpp"

#include <any>
#include <stdexcept>
#include <string>

namespace anagraph {
namespace graph_structure {

template <typename T>
WeightedHeteroNode<T>& WeightedHeteroNode<T>::operator=(const WeightedHeteroNode<T>& node) {
    weightedNode = node.weightedNode;
    attributes = node.attributes;
    isAttrEnabled = node.isAttrEnabled;
    return *this;
}

template <typename T>
const int WeightedHeteroNode<T>::UNUSED_ID = -1;

template <typename T>
int WeightedHeteroNode<T>::getId() const{
    return weightedNode.getId();
}

template <typename T>
void WeightedHeteroNode<T>::setId(int id) {
    weightedNode.setId(id);
}

template <typename T>
bool WeightedHeteroNode<T>::isUsed() const {
    return weightedNode.isUsed();
}

template <typename T>
const std::unordered_map<int, double>& WeightedHeteroNode<T>::getAdjacents() const {
    return weightedNode.getAdjacents();
}

template <typename T>
void WeightedHeteroNode<T>::setAdjacent(int adjacent, double weight) {
    weightedNode.setAdjacent(adjacent, weight);
}

template <typename T>
void WeightedHeteroNode<T>::updateAdjacent(int adjacent, double weight) {
    weightedNode.updateAdjacent(adjacent, weight);
}

template <typename T>
void WeightedHeteroNode<T>::removeAdjacent(int adjacent) {
    weightedNode.removeAdjacent(adjacent);
}

template <typename T>
T WeightedHeteroNode<T>::getAttributes() const {
    if (!isAttrEnabled) {
        throw std::runtime_error("Attributes are not enabled");
    }
    return attributes;
}

template <typename T>
void WeightedHeteroNode<T>::setAttributes(T attributes){
    this->attributes = attributes;
    isAttrEnabled = true;
}

template <typename T>
void WeightedHeteroNode<T>::clear() {
    weightedNode.clear();
    attributes = T();
    isAttrEnabled = false;
}

template class WeightedHeteroNode<int>;
template class WeightedHeteroNode<std::string>;
template class WeightedHeteroNode<std::any>;

} // namespace graph_structure
} // namespace anagraph