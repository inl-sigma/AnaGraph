#pragma once

#ifndef UNWEIGHTED_NODE_HPP
#define UNWEIGHTED_NODE_HPP

#include "anagraph/interfaces/unweighted_node_interface.hpp"

#include <functional>
#include <map>

namespace anagraph {
namespace graph_structure {

class Node : public interface::INode {
private:
    static inline int nodesCount = 0; /**< The number of nodes */

    int id;
    std::unordered_set<int> adjacentIds;
    std::map<int, std::reference_wrapper<Node>> adjacentNodes;

public:
    /**
     * @brief Default constructor for the Node class.
     */
    Node() : 
        id(nodesCount++), 
        adjacentIds(std::unordered_set<int>()), 
        adjacentNodes(std::map<int, std::reference_wrapper<Node>>()) 
    {}

    /**
     * @brief Constructs a new Node object with the given ID.
     *
     * @param id The ID of the node.
     */
    Node(int id) : 
        id(id),
        adjacentIds(std::unordered_set<int>()),
        adjacentNodes(std::map<int, std::reference_wrapper<Node>>())
    {
        if (id >= nodesCount) {nodesCount = id + 1;}
    }

    /**
     * @brief Copy constructor for the Node class.
     */
    Node(const Node& node) = default;

    /**
     * @brief Move constructor for the Node class.
     */
    Node(Node &&node) noexcept
        : id(node.id), adjacentIds(std::move(node.adjacentIds)) {
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
     * @brief Get the adjacent nodes of a node.
     * @return A map of integers representing the adjacent nodes.
     * 
     * This method is used to retrieve the adjacent nodes of a node.
     */
    const std::map<int, std::reference_wrapper<Node>>& getAdjacentNodes() const;

    /**
     * @brief Set an adjacent node to the node.
     * @param adjacent The adjacent node.
     */
    void setAdjacentNode(Node& adjacent);

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

} // namespace graph
} // namespace anagraph

#endif // UNWEIGHTED_NODE_HPP