#include "anagraph/components/weighted_hetero_node.hpp"

#include <spdlog/spdlog.h>

#include <any>
#include <stdexcept>
#include <string>

namespace anagraph {
namespace graph_structure {

template <typename T>
int WeightedHeteroNode<T>::getId() const{
    return id;
}

template <typename T>
void WeightedHeteroNode<T>::setId(int id) {
    if (id < 0) {
        spdlog::warn("setId: negative id({}) detected, use clear() to reset node", id);
    } else {
        this->id = id;
    }
}

template <typename T>
bool WeightedHeteroNode<T>::isUsed() const {
    return id != interface::IWeightedHeteroNode<T>::UNUSED_ID;
}

template <typename T>
const std::map<int, std::reference_wrapper<WeightedHeteroNode<T>>>& WeightedHeteroNode<T>::getAdjacentNodes() const {
    return adjacentNodes;
}

template <typename T>
const std::unordered_map<int, double>& WeightedHeteroNode<T>::getAdjacents() const {
    return adjacentIds;
}

template <typename T>
void WeightedHeteroNode<T>::setAdjacentNode(WeightedHeteroNode<T>& adjacent, double weight) {
    const int id = adjacent.getId();
    adjacentIds[id] = weight;
    adjacentNodes.insert({id, adjacent});
}

template <typename T>
void WeightedHeteroNode<T>::removeAdjacent(int adjacent) {
    adjacentIds.erase(adjacent);
}

template <typename T>
void WeightedHeteroNode<T>::setWeight(int adjacent, double weight) {
    adjacentIds[adjacent] = weight;
}

template <typename T>
void WeightedHeteroNode<T>::updateWeight(int adjacent, double weight) {
    if (adjacentIds.contains(adjacent)) {
        adjacentIds[adjacent] += weight;
    } else {
        adjacentIds[adjacent] = weight;
    }
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
    id = interface::IWeightedHeteroNode<T>::UNUSED_ID;
    adjacentIds.clear();
    adjacentNodes.clear();
    attributes = T();
    isAttrEnabled = false;
}

template class WeightedHeteroNode<int>;
template class WeightedHeteroNode<std::string>;
template class WeightedHeteroNode<std::any>;

} // namespace graph_structure
} // namespace anagraph