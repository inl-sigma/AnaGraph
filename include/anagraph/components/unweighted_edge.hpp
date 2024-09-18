#pragma once

#ifndef UNWEIGHTED_EDGE_HPP
#define UNWEIGHTED_EDGE_HPP

#include "anagraph/interfaces/unweighted_edge_interface.hpp"

namespace anagraph {
namespace graph_structure {

template <typename T>
class UnweightedEdge : public interface::IUnweightedEdge<T> {
private:
    T& sourceNode;
    T& destinationNode;

public:
    UnweightedEdge(T& sourceNode, T& destinationNode)
        : sourceNode(sourceNode), destinationNode(destinationNode) {}

    std::reference_wrapper<T> getSourceNode() override {
        return std::ref(sourceNode);
    }

    std::reference_wrapper<T> getDestinationNode() override {
        return std::ref(destinationNode);
    }
};

} // namespace graph_structure
} // namespace anagraph

#endif // UNWEIGHTED_EDGE_HPP