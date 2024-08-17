#include "supergraph.hpp"

const int WeightedSupernode::UNUSED_ID = -1;
const int WeightedSupernode::ROOT = -1;

int WeightedSupernode::getId() const {
    return node.getId();
}

void WeightedSupernode::setId(int id) {
    node.setId(id);
}

bool WeightedSupernode::isUsed() const {
    return node.isUsed();
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

void WeightedSupernode::addChild(int child) {
    children.insert(child);
}

const std::unordered_map<int, double>& WeightedSupernode::getAdjacents() const {
    return node.getAdjacents();
}

void WeightedSupernode::setAdjacent(int adjacent, double weight) {
    node.setAdjacent(adjacent, weight);
}

void WeightedSupernode::updateAdjacent(int adjacent, double weight) {
    node.updateAdjacent(adjacent, weight);
}

void WeightedSupernode::removeAdjacent(int adjacent) {
    node.removeAdjacent(adjacent);
}

void WeightedSupernode::clear() {
    node.clear();
    parent = ROOT;
    children.clear();
}