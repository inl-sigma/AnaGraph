#include "anagraph/components/weighted_node.hpp"

#include <spdlog/spdlog.h>

namespace anagraph {
namespace graph_structure {

WeightedNode::WeightedNode() {
    id = UNUSED_ID;
    adjacentIds = std::unordered_map<int, double>();
}

WeightedNode::WeightedNode(int id) {
    setId(id);
    adjacentIds = std::unordered_map<int, double>();
}

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
    return adjacentIds;
}

void WeightedNode::setAdjacent(int adjacent, double weight) {
    adjacentIds[adjacent] = weight;
}

void WeightedNode::updateAdjacent(int adjacent, double weight) {
    if (!adjacentIds.contains(adjacent)) {
        spdlog::debug("updateAdjacent: adding edge between {} and {}", id, adjacent);
        adjacentIds[adjacent] = weight;
    } else {
        spdlog::debug("updateAdjacent: updating edge between {} and {}", id, adjacent);
        adjacentIds[adjacent] += weight;
    }
}

void WeightedNode::removeAdjacent(int adjacent) {
    adjacentIds.erase(adjacent);
}

const std::map<int, std::reference_wrapper<WeightedNode>>& WeightedNode::getAdjacentNodes() const {
    return adjacentNodes;
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

void WeightedNode::clear() {
    id = UNUSED_ID;
    adjacentIds.clear();
}

} // namespace graph
} // namespace anagraph