#include "anagraph/components/unweighted_node.hpp"

#include <spdlog/spdlog.h>

namespace anagraph {
namespace graph {

const int Node::UNUSED_ID = -1;

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

const std::unordered_set<int>& Node::getAdjacents() const {
    return adjacents;
}

void Node::setAdjacent(int adjacent) {
    this->adjacents.insert(adjacent);
}

void Node::removeAdjacent(int adjacent) {
    this->adjacents.erase(adjacent);
}

void Node::clear() {
    id = UNUSED_ID;
    adjacents.clear();
}

} // namespace graph
} // namespace anagraph