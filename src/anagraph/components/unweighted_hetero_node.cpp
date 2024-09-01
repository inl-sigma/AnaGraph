#include "anagraph/components/unweighted_hetero_node.hpp"

#include <spdlog/spdlog.h>

#include <any>
#include <stdexcept>
#include <string>

namespace anagraph {
namespace graph_structure {

template <typename T>
int HeteroNode<T>::getId() const {
    return id;
}

template <typename T>
void HeteroNode<T>::setId(int id) {
    if (id < 0) {
        spdlog::warn("Node ID cannot be negative. ID not set.");
    } else {
        this->id = id;
    }
}

template <typename T>
bool HeteroNode<T>::isUsed() const {
    return id != interface::IHeteroNode<T>::UNUSED_ID;
}

template <typename T>
const std::unordered_set<int>& HeteroNode<T>::getAdjacents() const {
    return adjacentIds;
}

template <typename T>
void HeteroNode<T>::setAdjacent(int adjacent) {
    adjacentIds.insert(adjacent);
}

template <typename T>
void HeteroNode<T>::removeAdjacent(int adjacent) {
    adjacentIds.erase(adjacent);
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
const std::map<int, std::reference_wrapper<HeteroNode<T>>>& HeteroNode<T>::getAdjacentNodes() const {
    return adjacentNodes;
}

template <typename T>
void HeteroNode<T>::setAdjacentNode(HeteroNode<T>& adjacent) {
    const int id = adjacent.getId();
    adjacentIds.insert(id);
    adjacentNodes.insert({id, adjacent});
}

template <typename T>
void HeteroNode<T>::clear() {
    id = interface::IHeteroNode<T>::UNUSED_ID;
    adjacentIds.clear();
    adjacentNodes.clear();
    attributes = T();
    isAttrEnabled = false;
}

template class HeteroNode<int>;
template class HeteroNode<std::string>;
template class HeteroNode<std::any>;

} // namespace graph_structure
} // namespace anagraph