#pragma once

#ifndef SUPERGRAPH_HPP
#define SUPERGRAPH_HPP

#include "graph_interface.hpp"
#include "weighted_graph.hpp"

#include <unordered_map>

class WeightedSuperNode : IWeightedNode {
private:
    WeightedNode node;
    int parent = ROOT; /**< parent is implemented as type int, because this relation is a kind of attributed edge */
    std::unordered_set<int> children;

public:
    // todo : declare the constants as utils?
    static const int UNUSED_ID = -1; /**< UNUSED_ID is a constant that represents that the node is not used */
    static const int ROOT = -1; /**< ROOT is a constant that represents that the node has no parent node */
    /**
     * @brief Construct a new WeightedSuperNode object.
     */
    WeightedSuperNode() = default;

    /**
     * @brief Construct a new WeightedSuperNode object.
     * @param id The id of the node.
     */
    WeightedSuperNode(int id) : node(id) {};

    /**
     * @brief Copy constructor for WeightedSuperNode.
     *
     * This constructor creates a new WeightedSuperNode object by copying the contents of another WeightedSuperNode object.
     *
     * @param other The WeightedSuperNode object to be copied.
     */
    WeightedSuperNode(const WeightedSuperNode&) = default;

    /**
     * @brief Move constructor for WeightedSuperNode.
     *
     * This constructor creates a new WeightedSuperNode object by moving the contents of another WeightedSuperNode object.
     *
     * @param other The WeightedSuperNode object to be moved.
     */
    WeightedSuperNode(WeightedSuperNode&&) = default;

    /**
     * @brief Assignment operator for WeightedSuperNode.
     * 
     * This operator assigns the values of a WeightedSuperNode object to another WeightedSuperNode object.
     * 
     * @param other The WeightedSuperNode object to be assigned.
     * @return Reference to the assigned WeightedSuperNode object.
     */
    WeightedSuperNode& operator=(const WeightedSuperNode&) = default;

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
     * @brief Get the adjacent nodes of a node.
     * @return A map of integers representing the adjacent nodes and their corresponding weights.
     * 
     * This method is used to retrieve the adjacent nodes of a node.
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
     * @brief Remove an adjacent node from the node.
     * @param adjacent The id of the adjacent node.
     */
    void removeAdjacent(int adjacent) override;

    /**
     * @brief Clear the node.
     * 
     * This method is used to clear the node.
     */
    void clear() override;
};

// Implement the member functions here


/**
 * @brief The SuperGraph class.
 * 
 * This class is used to represent a supergraph.
 * A supergraph is a graph that contains a set of nodes and a set of edges.
 * Each node in the supergraph is a supernode that contains a set of nodes and edges.
 * The supergraph is used to represent a kind of hierarchy of graph.
 */
class SuperGraph : public IWeightedGraph {
};

#endif // SUPERGRAPH_HPP