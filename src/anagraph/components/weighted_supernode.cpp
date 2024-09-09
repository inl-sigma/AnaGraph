#include "anagraph/components/weighted_supernode.hpp"

namespace anagraph {
namespace graph_structure {

int WeightedSupernode::getId() const {
    return id;
}

void WeightedSupernode::setId(int id) {
    this->id = id;
}

bool WeightedSupernode::isUsed() const {
    return id != UNUSED_ID;
}

bool WeightedSupernode::isRoot() const {
    return parent == ROOT;
}

int WeightedSupernode::getParent() const {
    return parent;
}

void WeightedSupernode::setParent(int parent) {
    this->parent = parent;
}

bool WeightedSupernode::isLeaf() const {
    return children.empty();
}

std::unordered_set<int> WeightedSupernode::getChildren() const {
    return children;
}

void WeightedSupernode::removeChild(int child) {
    children.erase(child);
}

void WeightedSupernode::addChild(int child) {
    children.insert(child);
}

const std::map<int, std::reference_wrapper<WeightedSupernode>>& WeightedSupernode::getAdjacentNodes() const {
    return adjacentNodes;
}

const std::unordered_map<int, double>& WeightedSupernode::getAdjacents() const {
    return adjacentIds;
}

void WeightedSupernode::setAdjacentNode(WeightedSupernode& adjacent, double weight) {
    const int id = adjacent.getId();
    adjacentIds[id] = weight;
    adjacentNodes.insert({id, adjacent});
}

void WeightedSupernode::updateAdjacentNode(WeightedSupernode& adjacent, double weight) {
    const int id = adjacent.getId();
    if (adjacentIds.contains(id)) {
        adjacentIds[id] += weight;
    } else {
        setAdjacentNode(adjacent, weight);
    }
}

double WeightedSupernode::getWeight(int adjacent) const {
    if (adjacentIds.contains(adjacent)) {
        return adjacentIds.at(adjacent);
    } else {
        return 0.0;
    }
}

void WeightedSupernode::setWeight(int adjacent, double weight) {
    adjacentIds[adjacent] = weight;
}

void WeightedSupernode::updateWeight(int adjacent, double weight) {
    if (adjacentIds.contains(adjacent)) {
        adjacentIds[adjacent] += weight;
    } else {
        adjacentIds[adjacent] = weight;
    }
}

void WeightedSupernode::removeAdjacent(int adjacent) {
    adjacentIds.erase(adjacent);
}

void WeightedSupernode::clear() {
    id = UNUSED_ID;
    adjacentIds.clear();
    adjacentNodes.clear();
    parent = ROOT;
    children.clear();
}

} // namespace graph_structure
} // namespace anagraph