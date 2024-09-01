#pragma once

#ifndef WEIGHTED_HETERO_NODE_HPP
#define WEIGHTED_HETERO_NODE_HPP

#include "anagraph/components/weighted_node.hpp"
#include "anagraph/interfaces/weighted_hetero_node_interface.hpp"

#include <functional>
#include <map>

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
    int id; /**< The id of the node */
    std::unordered_map<int, double> adjacentIds; /**< The adjacent nodes of the node */
    std::map<int, std::reference_wrapper<WeightedHeteroNode<T>>> adjacentNodes; /**< The adjacent nodes of the node */
    T attributes;
    bool isAttrEnabled;

public:
    /**
     * @brief Constructs a weightedNode object.
     */
    WeightedHeteroNode() : 
        id(interface::IWeightedHeteroNode<T>::UNUSED_ID),
        adjacentIds(std::unordered_map<int, double>()),
        adjacentNodes(std::map<int, std::reference_wrapper<WeightedHeteroNode>>()), 
        attributes(T()), 
        isAttrEnabled(false) 
    {};

    /**
     * @brief Constructs a weightedNode object with the specified id.
     * @param id The id of the node.
     */
    WeightedHeteroNode(int id) :
        id(id), 
        adjacentIds(std::unordered_map<int, double>()), 
        adjacentNodes(std::map<int, std::reference_wrapper<WeightedHeteroNode>>()), 
        attributes(T()), 
        isAttrEnabled(false) 
    {};

    /**
     * @brief Constructs a weightedNode object with the specified id and attributes.
     * @param id The id of the node.
     * @param attributes The attributes of the node.
     */
    WeightedHeteroNode(int id, T attributes) : 
        id(id), 
        adjacentIds(std::unordered_map<int, double>()), 
        adjacentNodes(std::map<int, std::reference_wrapper<WeightedHeteroNode>>()), 
        attributes(attributes), 
        isAttrEnabled(true) 
    {};

    /**
     * @brief Copy constructor for the weightedNode object.
     */
    WeightedHeteroNode(const WeightedHeteroNode<T> &node) = default;

    /**
     * @brief Move constructor for the weightedNode object.
     */
    WeightedHeteroNode(WeightedHeteroNode<T> &&node) noexcept : 
        id(node.id), 
        adjacentIds(std::move(node.adjacentIds)), 
        adjacentNodes(std::move(node.adjacentNodes)), 
        attributes(node.attributes), 
        isAttrEnabled(node.isAttrEnabled) 
    {
        node.clear();
    };

    /**
     * @brief Assignment operator for the weightedNode object.
     */
    WeightedHeteroNode<T>& operator=(const WeightedHeteroNode<T>& node) = default;

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
     * This method is used to retrieve the adjacent nodes of a node.
     */
    const std::map<int, std::reference_wrapper<WeightedHeteroNode<T>>>& getAdjacentNodes() const;

    /**
     * @brief Set an adjacent node to the node.
     * @param adjacent The adjacent node.
     */
    void setAdjacentNode(WeightedHeteroNode<T>& adjacent, double weight);

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