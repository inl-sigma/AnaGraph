#pragma once

#ifndef WEIGHTED_HETERO_GRAPH_INTERFACE_HPP
#define WEIGHTED_HETERO_GRAPH_INTERFACE_HPP

namespace anagraph {
namespace interface {

/**
 * @class IWeightedHeteroGraph
 * @brief Represents a weighted graph data structure.
 * 
 * The IWeightedHeteroGraph class provides a way to represent a weighted graph using an adjacency list.
 * It supports adding edges between nodes and accessing the adjacency list of a node.
 */
template <typename T>
class IWeightedHeteroGraph {
public:
    virtual ~IWeightedHeteroGraph() = default;

    // TODO : Implement this function
    // /** 
    //  * @brief Get the attributes of a node.
    //  * @param id The node to get the attributes of
    //  * @return A reference to the attributes of the node
    //  * 
    //  * This method is used to access the attributes of a node.
    //  */
    // virtual IWeightedHeteroNode<T> getNode(int id) const = 0;

    // TODO : Implement this function
    // /**
    //  * @brief Get the node of the specific id.
    //  * @param id The id of the node
    //  * @return The node of the specific id
    //  */
    // virtual IWeightedHeteroNode<T>& operator[](int id) = 0;

    /**
     * @brief Set a node to the graph.
     * @param id The node to add
     */
    virtual void setNode(int id) = 0;

    // TODO : Implement this function
    // /**
    //  * @brief Set a node to the graph.
    //  * @param node The node to add
    //  */
    // virtual void setNode(IWeightedHeteroNode<T> &node) = 0;

    /**
     * @brief Remove a node from the graph.
     * @param id The node to remove
     */
    virtual void removeNode(int id) = 0;

    /**
     * @brief Get the id of the graph.
     */
    virtual std::unordered_set<int> getIds() const = 0;

    /**
     * @brief Add an edge between two nodes.
     * @param src The source node
     * @param dst The destination node
     * @param weight The weight of the edge
     */
    virtual void setEdge(int src, int dst, double weight) = 0;

    /**
     * @brief Remove an edge between two nodes.
     * @param src The source node
     * @param dst The destination node
     */
    virtual void removeEdge(int src, int dst) = 0;

    /**
     * @brief Get the weight of an edge between two nodes.
     * @param src The source node
     * @param dst The destination node
     * @return The weight of the edge
     */
    virtual double getWeight(int src, int dst) const = 0;

    /**
     * @brief Set the weight of an edge between two nodes.
     * @param src The source node
     * @param dst The destination node
     * @param weight The weight of the edge
     * 
     * @note this function will set the edge weight, not update it.
     */
    virtual void setWeight(int src, int dst, double weight) = 0;

    /**
     * @brief Add the weight of an edge between two nodes.
     * @param src The source node
     * @param dst The destination node
     * @param weight The weight of the edge
     * 
     * @note this function will update edge weight.
     * if the result equals to 0, the edge will be removed.
     */
    virtual void addWeight(int src, int dst, double weight) = 0;

    /**
     * @brief Get the adjacent nodes of a node.
     * @param id The source node
     * @return A pairs of integers, where the first means the adjacent node and the second means the weight of the edge.
     */
    virtual const std::unordered_map<int, double> getAdjacents(int id) const = 0;

    /**
     * @brief Get the attributes of the nodes. 
     * @param id The id of the node to get the attributes of
     * @return A vector of the attributes of the nodes
     */
    virtual T getAttributes(int id) const = 0;

    /**
     * @brief Set the attributes of the nodes.
     * @param id The id of the node to set the attributes of
     * @param attributes The attributes of the nodes
     */
    virtual void setAttributes(int id, T attributes) = 0;

    /**
     * @brief Get the number of nodes in the graph.
     */
    virtual size_t size() const = 0;

    /**
     * @brief Read a graph from a file.
     * @param filename The name of the file to import the graph from
     * @param extName The extension of the file
     */
    virtual void readGraph(std::string filename, FileExtension extName) = 0;

    /**
     * @brief Write the graph to a file.
     * @param filename The name of the file to export the graph to
     * @param extName The extension of the file
     */
    virtual void writeGraph(std::string filename, FileExtension extName) const = 0;
};

} // namespace interface
} // namespace anagraph

#endif // WEIGHTED_HETERO_GRAPH_INTERFACE_HPP