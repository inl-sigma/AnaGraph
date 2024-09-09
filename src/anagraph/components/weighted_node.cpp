#include "anagraph/components/weighted_node.hpp"

#include <spdlog/spdlog.h>

namespace anagraph {
namespace graph_structure {

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

const std::map<int, std::reference_wrapper<WeightedNode>>& WeightedNode::getAdjacentNodes() const {
    return adjacentNodes;
}

const std::unordered_map<int, double>& WeightedNode::getAdjacents() const {
    return adjacentIds;
}

void WeightedNode::setAdjacentNode(WeightedNode& adjacent, double weight) {
    const int id = adjacent.getId();
    if (id == UNUSED_ID) {
        spdlog::warn("setAdjacentNode: adjacent node has not been initialized");
        return;
    }
    adjacentIds[id] = weight;
    adjacentNodes.insert({id, adjacent});
}

void WeightedNode::removeAdjacent(int adjacent) {
    adjacentIds.erase(adjacent);
}

double WeightedNode::getWeight(int adjacent) const {
    if (adjacentIds.contains(adjacent)) {
        return adjacentIds.at(adjacent);
    } else {
        spdlog::debug("getWeight: no edge between {} and {}", id, adjacent);
        return 0.0;
    }
}

void WeightedNode::setWeight(int adjacent, double weight) {
    adjacentIds[adjacent] = weight;
}

void WeightedNode::updateWeight(int adjacent, double weight) {
    if (!adjacentIds.contains(adjacent)) {
        spdlog::debug("updateAdjacent: adding edge between {} and {}", id, adjacent);
        adjacentIds[adjacent] = weight;
    } else {
        spdlog::debug("updateAdjacent: updating edge between {} and {}", id, adjacent);
        adjacentIds[adjacent] += weight;
    }
}

void WeightedNode::clear() {
    id = UNUSED_ID;
    adjacentIds.clear();
}

} // namespace graph
} // namespace anagraph