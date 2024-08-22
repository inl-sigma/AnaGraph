#pragma once

#ifndef GRAPH_INTERFACE_HPP
#define GRAPH_INTERFACE_HPP

#include "graph_utils.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/**
 * @class INode
 * @brief Represents a node in an unweighted graph.
 * 
 * The INode class provides a way to represent a node in an unweighted graph.
 */
class INode {
public:
    virtual ~INode() = default;

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
     * @brief Set an adjacent node to the node.
     * @param adjacent The id of the adjacent node.
     */
    virtual void setAdjacent(int adjacent) = 0;

    /**
     * @brief Remove an adjacent node from the node.
     * @param adjacent The id of the adjacent node.
     */
    virtual void removeAdjacent(int adjacent) = 0;

    /**
     * @brief Clear the node.
     * 
     * This method is used to clear the node.
     */
    virtual void clear() = 0;
};

/**
 * @class IDigraph
 * @brief Represents a graph data structure.
 * 
 * The IDigraph class provides a way to represent a directed graph using an adjacency list.
 * It supports adding edges between nodes and accessing the adjacency list of a node.
 */
class IDigraph {
public:
    virtual ~IDigraph() = default;

    // TODO : Implement this function
    // /** 
    //  * @brief Get the attributes of a node.
    //  * @param id The node to get the attributes of
    //  * @return A reference to the attributes of the node
    //  * 
    //  * This method is used to modify the attributes of a node.
    //  */
    // virtual INode getNode(int id) const = 0;

    // TODO : Implement this function
    // /**
    //  * @brief Set the node of the graph.
    //  * @param node The node to set
    //  */
    // virtual void setNode(INode node) = 0;

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
     */
    virtual void addEdge(int src, int dst) = 0;

    /**
     * @brief Remove an edge between two nodes.
     * @param src The source node
     * @param dst The destination node
     */
    virtual void removeEdge(int src, int dst) = 0;

    /**
     * @brief Get the adjacent nodes of a node.
     * @param id The source node
     */
    virtual const std::unordered_set<int> getAdjacents(int id) const = 0;

    /**
     * @brief Get the number of nodes in the graph.
     */
    virtual size_t size() const = 0;

    // TODO : Implement this function
    // /**
    //  * @brief Get the node of the specific id.
    //  * @param id The id of the node
    //  * @return The node of the specific id
    //  */
    // virtual INode operator[](int id) = 0;

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

/**
 * @class IGraph
 * @brief Represents a graph data structure.
 * 
 * The IGraph class provides a way to represent a graph using an adjacency list.
 * It supports adding edges between nodes and accessing the adjacency list of a node.
 */
class IGraph {
public:
    virtual ~IGraph() = default;

    // TODO : Implement this function
    // /** 
    //  * @brief Get the attributes of a node.
    //  * @param id The node to get the attributes of
    //  * @return A reference to the attributes of the node
    //  * 
    //  * This method is used to modify the attributes of a node.
    //  */
    // virtual INode getNode(int id) const = 0;

    // TODO : Implement this function
    // /**
    //  * @brief Set the node of the graph.
    //  * @param node The node to set
    //  */
    // virtual void setNode(INode node) = 0;

    /**
     * @brief Get the id of the graph.
     */
    virtual std::unordered_set<int> getIds() const = 0;

    /**
     * @brief Remove a node from the graph.
     * @param id The node to remove
     */
    virtual void removeNode(int id) = 0;

    /**
     * @brief Add an edge between two nodes.
     * @param src The source node
     * @param dst The destination node
     */
    virtual void addEdge(int src, int dst) = 0;

    /**
     * @brief Remove an edge between two nodes.
     * @param src The source node
     * @param dst The destination node
     */
    virtual void removeEdge(int src, int dst) = 0;

    /**
     * @brief Get the adjacent nodes of a node.
     * @param id The source node
     */
    virtual const std::unordered_set<int> getAdjacents(int id) const = 0;

    /**
     * @brief Get the number of nodes in the graph.
     */
    virtual size_t size() const = 0;

    // TODO : Implement this function
    // /**
    //  * @brief Get the node of the specific id.
    //  * @param id The id of the node
    //  * @return The node of the specific id
    //  */
    // virtual INode operator[](int id) = 0;

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

/**
 * @class IWeightedNode
 * @brief Represents a node in a weighted graph.
 * 
 * The IWeightedNode class provides a way to represent a node in a weighted graph.
 * It supports adding edges between nodes and accessing the adjacency list of a node.
 */
class IWeightedNode {
public:
    virtual ~IWeightedNode() = default;

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
     * @return A map of integers representing the adjacent nodes and their corresponding weights.
     * 
     * This method is used to retrieve the adjacent nodes of a node.
     */
    virtual const std::unordered_map<int, double>& getAdjacents() const = 0;

    /**
     * @brief Set the adjacent node of a node.
     * @param adjacent The id of the adjacent node.
     * @param weight The weight of the edge between the nodes.
     */
    virtual void setAdjacent(int adjacent, double weight) = 0;

    /**
     * @brief Update the weight of an adjacent node.
     * @param adjacent The id of the adjacent node.
     * @param weight The new weight of the edge between the nodes.
     */
    virtual void updateAdjacent(int adjacent, double weight) = 0;

    /**
     * @brief Remove an adjacent node from the node.
     * @param adjacent The id of the adjacent node.
     */
    virtual void removeAdjacent(int adjacent) = 0;

    /**
     * @brief Clear the node.
     * 
     * This method is used to clear the node.
     */
    virtual void clear() = 0;
};

/**
 * @class IWeightedDigraph
 * @brief Represents a weighted graph data structure.
 * 
 * The IWeightedDigraph class provides a way to represent a weighted/directed graph using an adjacency list.
 * It supports adding edges between nodes and accessing the adjacency list of a node.
 */
class IWeightedDigraph {
public:
    virtual ~IWeightedDigraph() = default;

    // TODO : Implement this function
    // /** 
    //  * @brief Get the attributes of a node.
    //  * @param id The node to get the attributes of
    //  * @return A reference to the attributes of the node
    //  * 
    //  * This method is used to access the attributes of a node.
    //  */
    // virtual IWeightedNode<T> getNode(int id) const = 0;

    /**
     * @brief Get the id of the graph.
     */
    virtual std::unordered_set<int> getIds() const = 0;

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
    // virtual void setNode(IWeightedNode<T> &node) = 0;

    /**
     * @brief Remove a node from the graph.
     * @param id The node to remove
     */
    virtual void removeNode(int id) = 0;

    /**
     * @brief Add an edge between two nodes.
     * @param src The source node
     * @param dst The destination node
     * @param weight The weight of the edge
     */
    virtual void addEdge(int src, int dst, double weight) = 0;

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

    // TODO : Implement this function
    // /**
    //  * @brief Get the node of the specific id.
    //  * @param id The id of the node
    //  * @return The node of the specific id
    //  */
    // virtual IWeightedNode<T>& operator[](int id) = 0;

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

/**
 * @class IWeightedGraph
 * @brief Represents a weighted graph data structure.
 * 
 * The IWeightedGraph class provides a way to represent a weighted graph using an adjacency list.
 * It supports adding edges between nodes and accessing the adjacency list of a node.
 */
class IWeightedGraph {
public:
    virtual ~IWeightedGraph() = default;

    // TODO : Implement this function
    // /** 
    //  * @brief Get the attributes of a node.
    //  * @param id The node to get the attributes of
    //  * @return A reference to the attributes of the node
    //  * 
    //  * This method is used to access the attributes of a node.
    //  */
    // virtual IWeightedNode<T> getNode(int id) const = 0;

    /**
     * @brief Get the id of the graph.
     */
    virtual std::unordered_set<int> getIds() const = 0;

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
    // virtual void setNode(IWeightedNode<T> &node) = 0;

    /**
     * @brief Remove a node from the graph.
     * @param id The node to remove
     */
    virtual void removeNode(int id) = 0;

    /**
     * @brief Add an edge between two nodes.
     * @param src The source node
     * @param dst The destination node
     * @param weight The weight of the edge
     */
    virtual void addEdge(int src, int dst, double weight) = 0;

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

    // TODO : Implement this function
    // /**
    //  * @brief Get the node of the specific id.
    //  * @param id The id of the node
    //  * @return The node of the specific id
    //  */
    // virtual IWeightedNode<T>& operator[](int id) = 0;

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

#endif // GRAPH_INTERFACE_HPP
