#pragma once

#ifndef UNWEIGHTED_HETERO_NODE_INTERFACE_HPP
#define UNWEIGHTED_HETERO_NODE_INTERFACE_HPP

namespace anagraph {
namespace interface {

/**
 * @class IHeteroNode
 * @brief Represents a node in an unweighted graph.
 * 
 * The IHeteroNode class provides a way to represent a node in an unweighted graph.
 */
template <typename T>
class IHeteroNode {
public:
    virtual ~IHeteroNode() = default;

    static inline const int UNUSED_ID = -1; /**< The default value(-1) for an unused node */

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
     * @return A set of integers representing the adjacent nodes.
     * 
     * This method is used to retrieve the adjacent nodes of a node.
     */
    virtual const std::unordered_set<int>& getAdjacents() const = 0;

    /**
     * @brief Remove an adjacent node from the node.
     * @param adjacent The id of the adjacent node.
     */
    virtual void removeAdjacent(int adjacent) = 0;

    /**
     * @brief Get the attribute of the node.
     * @return The attribute of the node.
     * 
     * This method is used to access the attribute of a node.
     */
    virtual T getAttributes() const = 0;

    /**
     * @brief Set the attribute of the node.
     * @param attribute The attribute of the node.
     */
    virtual void setAttributes(T attributes) = 0;

    /**
     * @brief Clear the node.
     * 
     * This method is used to clear the node.
     */
    virtual void clear() = 0;
};

} // namespace interface
} // namespace anagraph

#endif // UNWEIGHTED_HETERO_NODE_INTERFACE_HPP