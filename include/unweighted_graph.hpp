#pragma once

#ifndef UNWEIGHTED_GRAPH_HPP
#define UNWEIGHTED_GRAPH_HPP

#include "graph_interface.hpp"

class Node : public INode {
private:
    int id;
    std::unordered_set<int> adjacents;

public:
    static const int UNUSED_ID; /**< The default value for an unused node */

    /**
     * @brief Default constructor for the Node class.
     */
    Node() : id(UNUSED_ID), adjacents(std::unordered_set<int>()) {}

    /**
     * @brief Constructs a new Node object with the given ID.
     *
     * @param id The ID of the node.
     */
    Node(int id) : id(id), adjacents(std::unordered_set<int>()) {}

    /**
     * @brief Copy constructor for the Node class.
     */
    Node(const Node& node) = default;

    /**
     * @brief Move constructor for the Node class.
     */
    Node(Node &&node) noexcept
        : id(node.id), adjacents(std::move(node.adjacents)) {
        node.clear();
    }


    /**
     * @brief Copy assignment operator for the Node class.
     */
    Node& operator=(const Node& node) = default;

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
     * This method is used to retrieve the adjacent nodes of a node.
     */
    const std::unordered_set<int>& getAdjacents() const override;

    /**
     * @brief Set an adjacent node to the node.
     * @param adjacent The id of the adjacent node.
     */
    void setAdjacent(int adjacent) override;

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


#endif // UNWEIGHTED_GRAPH_HPP