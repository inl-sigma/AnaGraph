#pragma once

#ifndef WEIGHTED_HETERO_NODE_HPP
#define WEIGHTED_HETERO_NODE_HPP

#include "anagraph/components/weighted_node.hpp"
#include "anagraph/interfaces/weighted_hetero_node_interface.hpp"

namespace anagraph {
namespace graph_structure {

/**
 * @class WeightedHeteroNode
 * @brief Represents a weighted node data structure with attributes.
 * 
 * The weightedNode class provides a way to represent a weighted node using an adjacency list.
 * It supports adding adjacent nodes and accessing the attributes of the node.
 */
template <typename T>
class WeightedHeteroNode : public interface::IWeightedHeteroNode<T> {
private:
    WeightedNode weightedNode;
    T attributes;
    bool isAttrEnabled;

public:
    /**
     * @brief Constructs a weightedNode object.
     */
    WeightedHeteroNode() : weightedNode(WeightedNode()), attributes(T()), isAttrEnabled(false) {
    };

    /**
     * @brief Constructs a weightedNode object with the specified id.
     * @param id The id of the node.
     */
    WeightedHeteroNode(int id) : weightedNode(WeightedNode(id)), attributes(T()), isAttrEnabled(false) {
    };

    /**
     * @brief Constructs a weightedNode object with the specified id and attributes.
     * @param id The id of the node.
     * @param attributes The attributes of the node.
     */
    WeightedHeteroNode(int id, T attributes) : weightedNode(WeightedNode(id)), attributes(attributes), isAttrEnabled(true) {
    };

    /**
     * @brief Copy constructor for the weightedNode object.
     */
    WeightedHeteroNode(const WeightedHeteroNode<T> &node) 
        : weightedNode(node.weightedNode), attributes(node.attributes), isAttrEnabled(node.isAttrEnabled) {
    }

    /**
     * @brief Move constructor for the weightedNode object.
     */
    WeightedHeteroNode(WeightedHeteroNode<T> &&node) noexcept
        : weightedNode(std::move(node.weightedNode)), attributes(std::move(node.attributes)), 
        isAttrEnabled(std::move(node.isAttrEnabled)) 
    {};

    /**
     * @brief Assignment operator for the weightedNode object.
     */
    WeightedHeteroNode<T>& operator=(const WeightedHeteroNode<T>& node);

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
     * @brief Get the attribute of the node.
     * @return A pointer to the attribute of the node.
     * 
     * This method is used to access the attribute of a node.
     */
    T getAttributes() const override;

    /**
     * @brief Set the attributes of the node.
     * @param attribute The attributes of the node.
     * 
     * This method is used to set the attributes of a node.
     */
    void setAttributes(T attributes) override;

    /**
     * @brief Clear the attributes of the node.
     */
    void clear() override;
};

} // namespace graph_structure
} // namespace anagraph

#endif // WEIGHTED_HETERO_NODE_HPP