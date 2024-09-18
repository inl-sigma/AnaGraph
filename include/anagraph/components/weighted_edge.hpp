#pragma once

#ifndef WEIGHTED_EDGE_HPP
#define WEIGHTED_EDGE_HPP

#include "anagraph/interfaces/weighted_edge_interface.hpp"

namespace anagraph {
namespace graph_structure {

template <typename T>
class WeightedEdge : public interface::IWeightedEdge<T> {
private:
    T& sourceNode;
    T& destinationNode;
    double weight;

public:
    WeightedEdge(T& sourceNode, T& destinationNode, double weight)
        : sourceNode(sourceNode), destinationNode(destinationNode), weight(weight) {}

    std::reference_wrapper<T> getSourceNode() override {
        return std::ref(sourceNode);
    }

    std::reference_wrapper<T> getDestinationNode() override {
        return std::ref(destinationNode);
    }

    double getWeight() override {
        return weight;
    }

    void setWeight(double weight) override {
        this->weight = weight;
    }
};

} // namespace graph_structure
} // namespace anagraph

#endif // WEIGHTED_EDGE_HPP