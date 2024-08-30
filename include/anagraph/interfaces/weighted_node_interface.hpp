#pragma once

#ifndef WEIGHTED_NODE_INTERFACE_HPP
#define WEIGHTED_NODE_INTERFACE_HPP

#include <unordered_map>

namespace anagraph {
namespace interface {

/**
 * @class IWeightedNode
 * @brief Represents a node in a weighted graph.
 * 
 * The IWeightedNode class provides a way to represent a node in a weighted graph.
 * It supports adding edges between nodes and accessing the adjacency list of a node.
 */
class IWeightedNode {
public:
    virtual ~IWeightedNode() = default;

    static inline const int UNUSED_ID = -1; /**< The default value(-1) for an unused node */

    /**
     * @brief Get the id of the node.
     * @return The id of the node.
     */
    virtual int getId() const = 0;

    /**
     * @brief Set the id of the node.
     * @param id The id of the node.
     */
    virtual void setId(int id) = 0;

    /**
     * @brief Check if the node is used.
     * @return True if the node is used, false otherwise.
     */
    virtual bool isUsed() const = 0;

    /**
     * @brief Get the adjacent nodes of a node.
     * @return A map of integers representing the adjacent nodes and their corresponding weights.
     * 
     * This method is used to retrieve the adjacent nodes of a node.
     */
    virtual const std::unordered_map<int, double>& getAdjacents() const = 0;

    /**
     * @brief Set the adjacent node of a node.
     * @param adjacent The id of the adjacent node.
     * @param weight The weight of the edge between the nodes.
     */
    virtual void setAdjacent(int adjacent, double weight) = 0;

    /**
     * @brief Update the weight of an adjacent node.
     * @param adjacent The id of the adjacent node.
     * @param weight The new weight of the edge between the nodes.
     */
    virtual void updateAdjacent(int adjacent, double weight) = 0;

    /**
     * @brief Remove an adjacent node from the node.
     * @param adjacent The id of the adjacent node.
     */
    virtual void removeAdjacent(int adjacent) = 0;

    /**
     * @brief Clear the node.
     * 
     * This method is used to clear the node.
     */
    virtual void clear() = 0;
};

} // namespace interface
} // namespace anagraph

#endif // WEIGHTED_NODE_INTERFACE_HPP