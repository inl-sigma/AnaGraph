#include "anagraph/components/weighted_node.hpp"

#include <spdlog/spdlog.h>

namespace anagraph {
namespace graph {

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

} // namespace graph
} // namespace anagraph