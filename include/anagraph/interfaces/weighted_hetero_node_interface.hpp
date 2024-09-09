#pragma once

#ifndef WEIGHTED_HETERO_NODE_INTERFACE_HPP
#define WEIGHTED_HETERO_NODE_INTERFACE_HPP

namespace anagraph {
namespace interface {

/**
 * @class IWeightedHeteroNode
 * @brief Represents a node in a weighted graph.
 * 
 * The IWeightedHeteroNode class provides a way to represent a node in a weighted graph.
 * It supports adding edges between nodes and accessing the adjacency list of a node.
 */
template <typename T>
class IWeightedHeteroNode {
public:
    virtual ~IWeightedHeteroNode() = default;

    static inline const int UNUSED_ID = -1; /**< The default value for an unused node */

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
    virtual void setWeight(int adjacent, double weight) = 0;

    /**
     * @brief Update the weight of an adjacent node.
     * @param adjacent The id of the adjacent node.
     * @param weight The new weight of the edge between the nodes.
     */
    virtual void updateWeight(int adjacent, double weight) = 0;

    /**
     * @brief Remove an adjacent node from the node.
     * @param adjacent The id of the adjacent node.
     */
    virtual void removeAdjacent(int adjacent) = 0;

    /**
     * @brief Get the attribute of the node.
     * @return The attribute of the node.
     * 
     * This method is used to retrieve the attribute of a node.
     */
    virtual T getAttributes() const = 0;

    /**
     * @brief Set the attribute of the node.
     * @param attribute The attribute of the node.
     */
    virtual void setAttributes(T attribute) = 0;

    /**
     * @brief Clear the node.
     * 
     * This method is used to clear the node.
     */
    virtual void clear() = 0;
};

} // namespace interface
} // namespace anagraph

#endif // WEIGHTED_HETERO_NODE_INTERFACE_HPP