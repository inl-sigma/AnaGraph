#include "anagraph/components/unweighted_node.hpp"

#include <spdlog/spdlog.h>

namespace anagraph {
namespace graph_structure {

int Node::getId() const {
    return id;
}

void Node::setId(int id) {
    if (id < 0) {
        spdlog::warn("Node ID cannot be negative. ID not set.");
    } else {
        this->id = id;
    }
}

bool Node::isUsed() const {
    return id != UNUSED_ID;
}

const std::map<int, std::reference_wrapper<Node>>& Node::getAdjacentNodes() const {
    return adjacentNodes;
}

const std::unordered_set<int>& Node::getAdjacents() const {
    return adjacentIds;
}

void Node::setAdjacentNode(Node& adjacent) {
    const int id = adjacent.getId();
    adjacentIds.insert(id);
    adjacentNodes.insert({id, adjacent});
}

void Node::removeAdjacent(int adjacent) {
    this->adjacentIds.erase(adjacent);
    this->adjacentNodes.erase(adjacent);
}

void Node::clear() {
    id = UNUSED_ID;
    adjacentIds.clear();
}

} // namespace graph
} // namespace anagraph