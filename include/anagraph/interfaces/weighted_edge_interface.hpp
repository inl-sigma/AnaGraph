#pragma once

#ifndef WEIGHTED_EDGE_INTERFACE_HPP
#define WEIGHTED_EDGE_INTERFACE_HPP

#include <functional>

namespace anagraph {
namespace interface {

template <typename T>
class IWeightedEdge {
public:
    virtual ~IWeightedEdge() = default;

    /**
     * @brief Get the source node of the edge.
     * @return The source node of the edge
     */
    virtual std::reference_wrapper<T> getSourceNode() = 0;

    /**
     * @brief Get the destination node of the edge.
     * @return The destination node of the edge
     */
    virtual std::reference_wrapper<T> getDestinationNode() = 0;

    /**
     * @brief Get the weight of the edge.
     * @return The weight of the edge
     */
    virtual double getWeight() = 0;

    /**
     * @brief Set the weight of the edge.
     * @param weight The weight of the edge
     */
    virtual void setWeight(double weight) = 0;
};

} // namespace interface
} // namespace anagraph

#endif // WEIGHTED_EDGE_INTERFACE_HPP