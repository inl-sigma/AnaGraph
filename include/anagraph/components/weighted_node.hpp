#pragma once

#ifndef WEIGHTED_NODE_HPP
#define WEIGHTED_NODE_HPP

#include "anagraph/interfaces/weighted_node_interface.hpp"

#include <spdlog/spdlog.h>

#include <functional>
#include <map>

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
    static inline int nodesCount = 0; /**< The number of nodes created */

    int id; /**< The id of the node */
    std::unordered_map<int, double> adjacentIds; /**< The adjacent nodes of the node */
    std::map<int, std::reference_wrapper<WeightedNode>> adjacentNodes; /**< The adjacent nodes of the node */

public:
    /**
     * @brief Constructs a weightedNode object.
     */
    WeightedNode() : 
        id(nodesCount++), 
        adjacentIds(std::unordered_map<int, double>()), 
        adjacentNodes(std::map<int, std::reference_wrapper<WeightedNode>>()) 
    {}

    /**
     * @brief Constructs a weightedNode object with the specified id.
     * @param id The id of the node.
     */
    WeightedNode(int id) : 
        id(id), 
        adjacentIds(std::unordered_map<int, double>()), 
        adjacentNodes(std::map<int, std::reference_wrapper<WeightedNode>>()) 
    {
        if (id >= nodesCount) {nodesCount = id + 1;}
    }

    /**
     * @brief Copy constructor for the weightedNode object.
     */
    WeightedNode(const WeightedNode &node) = default;

    /**
     * @brief Assignment operator for the weightedNode object.
     */
    WeightedNode& operator=(const WeightedNode& node) = default;

    /**
     * @brief Move constructor for the weightedNode object.
     */
    WeightedNode(WeightedNode &&node) noexcept : 
        id(node.id), 
        adjacentIds(std::move(node.adjacentIds)), 
        adjacentNodes(std::move(node.adjacentNodes)) 
    {
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
     * @brief Get the adjacent nodes of a node.
     * @return A map of integers representing the adjacent nodes.
     * 
     * This method is used to access the adjacent nodes of a node.
     */
    const std::map<int, std::reference_wrapper<WeightedNode>>& getAdjacentNodes() const;

    /**
     * @brief Set an adjacent node to the node.
     * @param adjacent The id of the adjacent node.
     * @param weight The weight of the edge between the nodes.
     */
    void setAdjacentNode(WeightedNode& adjacent, double weight);

    /**
     * @brief Clear the attributes of the node.
     */
    void clear() override;

    /**
     * @brief Reset the nodes count.
     * 
     * This method is used to reset the nodes count.
     */
    static void resetNodesCount() {
        nodesCount = 0;
    }
};

} // namespace graph
} // namespace anagraph

#endif // WEIGHTED_NODE_HPP