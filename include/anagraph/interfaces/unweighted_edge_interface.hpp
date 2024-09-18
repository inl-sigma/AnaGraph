#pragma once

#ifndef UNWEIGHTED_EDGE_INTERFACE_HPP
#define UNWEIGHTED_EDGE_INTERFACE_HPP

#include <functional>

namespace anagraph {
namespace interface {

template <typename T>
class IEdge {
public:
    virtual ~IUnweightedEdge() = default;

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
};

} // namespace interface
} // namespace anagraph

#endif // UNWEIGHTED_EDGE_INTERFACE_HPP