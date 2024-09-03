#pragma once

#ifndef UNWEIGHTED_HETERO_NODE_HPP
#define UNWEIGHTED_HETERO_NODE_HPP

#include "anagraph/components/unweighted_node.hpp"
#include "anagraph/interfaces/unweighted_hetero_node_interface.hpp"

#include <functional>
#include <map>

namespace anagraph {
namespace graph_structure {

/**
 * @class HeteroNode
 * @brief Represents a heterogeneous node data structure.
 * 
 * @tparam T The type of the attributes of the node.
 * 
 * The HeteroNode class provides a way to represent a heterogeneous node using an adjacency list.
 * It supports adding adjacent nodes to the node and accessing the attributes of the node.
 */
template <typename T>
class HeteroNode : public interface::IHeteroNode<T> {
private:
    static inline int nodesCount = 0;

    int id;
    std::unordered_set<int> adjacentIds;
    std::map<int, std::reference_wrapper<HeteroNode<T>>> adjacentNodes;
    T attributes;
    bool isAttrEnabled;

public:
    /**
     * @brief Constructs a HeteroNode object.
     */
    HeteroNode() : 
        id(nodesCount++), 
        adjacentIds(std::unordered_set<int>()), 
        adjacentNodes(std::map<int, std::reference_wrapper<HeteroNode>>()),
        attributes(T()), 
        isAttrEnabled(false) 
    {};

    /**
     * @brief Constructs a HeteroNode object with the specified id.
     * @param id The id of the node.
     */
    HeteroNode(int id) :
        id(id), 
        adjacentIds(std::unordered_set<int>()), 
        adjacentNodes(std::map<int, std::reference_wrapper<HeteroNode>>()),
        attributes(T()), 
        isAttrEnabled(false)
    {
        if (id >= nodesCount) {nodesCount = id + 1;}
    };

    /**
     * @brief Constructs a HeteroNode object with the specified id and attributes.
     * @param id The id of the node.
     * @param attributes The attributes of the node.
     */
    HeteroNode(int id, T attributes) :
        id(id), 
        adjacentIds(std::unordered_set<int>()), 
        adjacentNodes(std::map<int, std::reference_wrapper<HeteroNode>>()),
        attributes(attributes), 
        isAttrEnabled(true)
    {
        if (id >= nodesCount) {nodesCount = id + 1;}
    };

    /**
     * @brief Copy constructor for the HeteroNode object.
     */
    HeteroNode(const HeteroNode &node) = default;

    /**
     * @brief Move constructor for the HeteroNode object.
     */
    HeteroNode(HeteroNode &&node) : 
        id(node.id), 
        adjacentIds(std::move(node.adjacentIds)), 
        adjacentNodes(std::move(node.adjacentNodes)),
        attributes(node.attributes), 
        isAttrEnabled(node.isAttrEnabled)
    {
        node.clear();
    };

    /**
     * @brief Copy assignment operator for the HeteroNode object.
     */
    HeteroNode& operator=(const HeteroNode &node) = default;

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
    const std::map<int, std::reference_wrapper<HeteroNode<T>>>& getAdjacentNodes() const;

    /**
     * @brief Set an adjacent node to the node.
     * @param adjacent The adjacent node.
     */
    void setAdjacentNode(HeteroNode<T>& adjacent);

    /**
     * @brief Get the attribute of the node.
     * @return The attribute of the node.
     * 
     * This method is used to access the attribute of a node.
     */
    T getAttributes() const override;

    /**
     * @brief Set the attribute of the node.
     * @param attribute The attribute of the node.
     */
    void setAttributes(T attributes) override;

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

#endif // UNWEIGHTED_HETERO_NODE_HPP