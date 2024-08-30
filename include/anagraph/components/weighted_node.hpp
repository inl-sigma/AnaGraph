#pragma once

#ifndef WEIGHTED_NODE_HPP
#define WEIGHTED_NODE_HPP

#include "anagraph/interfaces/weighted_node_interface.hpp"

namespace anagraph {
namespace graph_structure {

/**
 * @class weightedNode
 * @brief Represents a weighted node data structure.
 * 
 * The weightedNode class provides a way to represent a weighted node using an adjacency list.
 * It supports adding adjacent nodes and accessing the attributes of the node.
 */
class WeightedNode : public interface::IWeightedNode {
private:
    int id; /**< The id of the node */
    std::unordered_map<int, double> adjacents; /**< The adjacent nodes of the node */

public:
    /**
     * @brief Constructs a weightedNode object.
     */
    WeightedNode();

    /**
     * @brief Constructs a weightedNode object with the specified id.
     * @param id The id of the node.
     */
    WeightedNode(int id);

    /**
     * @brief Copy constructor for the weightedNode object.
     */
    WeightedNode(const WeightedNode &node) 
        : id(node.id), adjacents(node.adjacents) {
    }

    /**
     * @brief Assignment operator for the weightedNode object.
     */
    WeightedNode& operator=(const WeightedNode& node);

    /**
     * @brief Move constructor for the weightedNode object.
     */
    WeightedNode(WeightedNode &&node) noexcept
        : id(node.id), adjacents(std::move(node.adjacents)) {
        node.clear();
    }

    /**
     * @brief Get the id of the node.
     * @return The id of the node.
     */
    int getId() const override;

    /**
     * @brief Set the id of the node.
     * @param id The id of the node.
     */
    void setId(int id) override;

    /**
     * @brief Check if the node is used.
     * @return True if the node is used, false otherwise.
     */
    bool isUsed() const override;

    /**
     * @brief Get the adjacent nodes of a node.
     * @return A set of integers representing the adjacent nodes.
     * 
     * This method is used to access the adjacent nodes of a node.
     */
    const std::unordered_map<int, double>& getAdjacents() const override;

    /**
     * @brief Set the adjacent node of a node.
     * @param adjacent The id of the adjacent node.
     * @param weight The weight of the edge between the nodes.
     */
    void setAdjacent(int adjacent, double weight) override;

    /**
     * @brief Update the weight of an adjacent node.
     * @param adjacent The id of the adjacent node.
     * @param weight The new weight of the edge between the nodes.
     */
    void updateAdjacent(int adjacent, double weight) override;

    /**
     * @brief Remove the adjacent nodes of a node.
     * @param adjacent A set of integers representing the adjacent nodes.
     */
    void removeAdjacent(int adjacent) override;

    /**
     * @brief Clear the attributes of the node.
     */
    void clear() override;
};

} // namespace graph
} // namespace anagraph

#endif // WEIGHTED_NODE_HPP