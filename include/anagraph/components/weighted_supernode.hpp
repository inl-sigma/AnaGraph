#pragma once

#ifndef WEIGHTED_SUPERNODE_HPP
#define WEIGHTED_SUPERNODE_HPP

#include "anagraph/components/weighted_node.hpp"
#include "anagraph/interfaces/weighted_node_interface.hpp"

#include <spdlog/spdlog.h>

#include <functional>
#include <unordered_set>

namespace anagraph {
namespace graph_structure {

class WeightedSupernode : public interface::IWeightedNode {
private:
    static inline int nodesCount = 0; /**< The number of nodes */

    int id; /**< The id of the node */
    int parent = ROOT; /**< parent is implemented as type int, because this relation is a kind of attributed edge */
    std::unordered_map<int, double> adjacentIds; /**< The adjacent nodes of the node */
    std::map<int, std::reference_wrapper<WeightedSupernode>> adjacentNodes; /**< The adjacent nodes of the node */
    std::unordered_set<int> children;

public:
    static inline const int ROOT = -1; /**< ROOT is a constant that represents that the node has no parent node */
    /**
     * @brief Construct a new WeightedSupernode object.
     */
    WeightedSupernode() :
        id(nodesCount++),
        parent(ROOT),
        adjacentIds(),
        adjacentNodes(),
        children()
    {};

    /**
     * @brief Construct a new WeightedSupernode object.
     * @param id The id of the node.
     */
    WeightedSupernode(int id) : 
        id(id), 
        parent(ROOT),
        adjacentIds(),
        adjacentNodes(),
        children()
    {
        if (id >= nodesCount) {nodesCount = id + 1;}
    };

    /**
     * @brief Copy constructor for WeightedSupernode.
     *
     * This constructor creates a new WeightedSupernode object by copying the contents of another WeightedSupernode object.
     *
     * @param other The WeightedSupernode object to be copied.
     */
    WeightedSupernode(const WeightedSupernode&) = default;

    /**
     * @brief Move constructor for WeightedSupernode.
     *
     * This constructor creates a new WeightedSupernode object by moving the contents of another WeightedSupernode object.
     *
     * @param other The WeightedSupernode object to be moved.
     */
    WeightedSupernode(WeightedSupernode&&) noexcept = default;

    /**
     * @brief Assignment operator for WeightedSupernode.
     * 
     * This operator assigns the values of a WeightedSupernode object to another WeightedSupernode object.
     * 
     * @param other The WeightedSupernode object to be assigned.
     * @return Reference to the assigned WeightedSupernode object.
     */
    WeightedSupernode& operator=(const WeightedSupernode&) = default;

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
     * @brief Check if the node is root node.
     * 
     * This method is used to check if the node is a root node.
     * A root node is a node that has no parent.
     */
    bool isRoot() const;

    /**
     * @brief Get the parent node of the node.
     */
    int getParent() const;

    /**
     * @brief Set the parent node of the node.
     * @param parent The id of the parent node.
     */
    void setParent(int parent);

    /**
     * @brief Check if the node is leef node.
     * 
     * This method is used to check if the node is a leaf node.
     * A leaf node is a node that has no children.
     */
    bool isLeaf() const;

    /**
     * @brief Get the children nodes of the node.
     */
    std::unordered_set<int> getChildren() const;

    /**
     * @brief Add a child node to the node.
     * @param child The id of the child node.
     */
    void addChild(int child);

    /**
     * @brief Remove a child node from the node.
     * @param child The id of the child node.
     */
    void removeChild(int child);

    /**
     * @brief Get the adjacent nodes of a node.
     * @return A map of integers representing the adjacent nodes.
     * 
     * This method is used to access the adjacent nodes of a node.
     */
    const std::map<int, std::reference_wrapper<WeightedSupernode>>& getAdjacentNodes() const;

    /**
     * @brief Get the adjacent nodes of a node.
     * @return A map of integers representing the adjacent nodes and their corresponding weights.
     * 
     * @note this function returns the adjacent nodes of the node without considering the hierarchy.
     * 
     * This method is used to retrieve the adjacent nodes of a node.
     */
    const std::unordered_map<int, double>& getAdjacents() const override;

    /**
     * @brief Set an adjacent node to the node.
     * @param adjacent The id of the adjacent node.
     * @param weight The weight of the edge between the nodes.
     */
    void setAdjacentNode(WeightedSupernode& adjacent, double weight);

    /**
     * @brief Update the weight of an adjacent node.
     * @param adjacent The id of the adjacent node.
     * @param weight The new weight of the edge between the nodes.
     */
    void updateAdjacentNode(WeightedSupernode& adjacent, double weight);

    /**
     * @brief Remove an adjacent node from the node.
     * @param adjacent The id of the adjacent node.
     */
    void removeAdjacent(int adjacent) override;

    /**
     * @brief Get the weight of an adjacent node.
     * @param adjacent The id of the adjacent node.
     * @return The weight of the edge between the nodes.
     */
    double getWeight(int adjacent) const;

    /**
     * @brief Set the adjacent node of a node.
     * @param adjacent The id of the adjacent node.
     * @param weight The weight of the edge between the nodes.
     */
    void setWeight(int adjacent, double weight) override;

    /**
     * @brief Update the weight of an adjacent node.
     * @param adjacent The id of the adjacent node.
     * @param weight The new weight of the edge between the nodes.
     */
    void updateWeight(int adjacent, double weight) override;

    /**
     * @brief Clear the node.
     * 
     * This method is used to clear the node.
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

} // namespace graph_structure
} // namespace anagraph

#endif // WEIGHTED_SUPERNODE_HPP