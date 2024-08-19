#pragma once

#ifndef SUPERGRAPH_HPP
#define SUPERGRAPH_HPP

#include "graph_interface.hpp"
#include "graph_utils.hpp"
#include "weighted_graph.hpp"

#include <unordered_map>

class WeightedSupernode : IWeightedNode {
private:
    WeightedNode node;
    int parent = ROOT; /**< parent is implemented as type int, because this relation is a kind of attributed edge */
    std::unordered_set<int> children;

public:
    // todo : declare the constants as utils?
    static const int UNUSED_ID; /**< UNUSED_ID is a constant that represents that the node is not used */
    static const int ROOT; /**< ROOT is a constant that represents that the node has no parent node */
    /**
     * @brief Construct a new WeightedSupernode object.
     */
    WeightedSupernode() = default;

    /**
     * @brief Construct a new WeightedSupernode object.
     * @param id The id of the node.
     */
    WeightedSupernode(int id) : node(id), children(std::unordered_set<int>()) {};

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
    WeightedSupernode(WeightedSupernode&&) = default;

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
     * @return A map of integers representing the adjacent nodes and their corresponding weights.
     * 
     * @note this function returns the adjacent nodes of the node without considering the hierarchy.
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

/**
 * @brief The WeightedSuperDigraph class.
 * 
 * This class is used to represent a superdigraph(directed supergraph).
 * A superdigraph is a digraph that contains a set of nodes and a set of edges.
 * Each node in the superdigraph is a supernode that contains a set of nodes and edges.
 * The superdigraph is used to represent a kind of hierarchy of digraph.
 */
class WeightedSuperDigraph : public IWeightedDigraph {
private:
    std::vector<WeightedSupernode> nodes;
    std::unordered_set<int> usedNodes;

public:
    /**
     * @brief Construct a new WeightedSuperDigraph object.
     */
    WeightedSuperDigraph() = default;

    /**
     * @brief Construct a new WeightedSuperDigraph object by files.
     * @param filepath The path of the file to import the graph from
     * @param extName The extension of the file
     */
    WeightedSuperDigraph(std::string filepath, FileExtension extName);

    /**
     * @brief Copy constructor for WeightedSuperDigraph.
     *
     * This constructor creates a new WeightedSuperDigraph object by copying the contents of another WeightedSuperDigraph object.
     *
     * @param other The WeightedSuperDigraph object to be copied.
     */
    WeightedSuperDigraph(const WeightedSuperDigraph&) = default;

    /**
     * @brief Move constructor for WeightedSuperDigraph.
     *
     * This constructor creates a new WeightedSuperDigraph object by moving the contents of another WeightedSuperDigraph object.
     *
     * @param other The WeightedSuperDigraph object to be moved.
     */
    WeightedSuperDigraph(WeightedSuperDigraph&&) = default;

    /**
     * @brief Assignment operator for WeightedSuperDigraph.
     * 
     * This operator assigns the values of a WeightedSuperDigraph object to another WeightedSuperDigraph object.
     * 
     * @param other The WeightedSuperDigraph object to be assigned.
     * @return Reference to the assigned WeightedSuperDigraph object.
     */
    WeightedSuperDigraph& operator=(const WeightedSuperDigraph&) = default;

    /**
     * @brief Set a node to the graph.
     * @param id The node to add
     */
    void setNode(int id) override;

    /**
     * @brief Set a node to the graph.
     * @param node The node to add
     */
    void setNode(WeightedSupernode &node);

    /**
     * @brief Get a node from the graph.
     * @param id The node to get
     * @return The node
     */
    WeightedSupernode getNode(int id) const;

    /**
     * @brief Remove a node from the graph.
     * @param id The node to remove
     */
    void removeNode(int id) override;

    /**
     * @brief Get the id of the graph.
     */
    std::unordered_set<int> getIds() const override;

    /**
     * @brief Add an edge between two nodes.
     * @param src The source node
     * @param dst The destination node
     * @param weight The weight of the edge
     */
    void addEdge(int src, int dst, double weight) override;

    /**
     * @brief Remove an edge between two nodes.
     * @param src The source node
     * @param dst The destination node
     */
    void removeEdge(int src, int dst) override;

    /**
     * @brief Get the weight of an edge between two nodes.
     * @param src The source node
     * @param dst The destination node
     * @return The weight of the edge
     */
    double getWeight(int src, int dst) const override;

    /**
     * @brief Set the weight of an edge between two nodes.
     * @param src The source node
     * @param dst The destination node
     * @param weight The weight of the edge
     * 
     * @note this function will set the edge weight, not update it.
     */
    void setWeight(int src, int dst, double weight) override;

    /**
     * @brief Add the weight of an edge between two nodes.
     * @param src The source node
     * @param dst The destination node
     * @param weight The weight of the edge
     * 
     * @note this function will update edge weight.
     * if the result equals to 0, the edge will be removed.
     */
    void addWeight(int src, int dst, double weight) override;

    /**
     * @brief Returns the parent of the specified node.
     *
     * @param id The ID of the node.
     * @return The ID of the parent node.
     */
    int getParent(int id) const;

    /**
     * @brief Set the parent of the specified node.
     *
     * @param child The ID of the child node.
     * @param parent The ID of the parent node.
     * 
     * @note this function will set the parent and children of the nodes simultaneously.
     */
    void setParent(int child, int parent);

    /**
     * @brief Update the parent of the specified node.
     *
     * @param child The ID of the child node.
     * @param parent The ID of the parent node.
     */
    void updateParent(int child, int parent);

    /**
     * @brief Remove the parent of the specified node.
     *
     * @param child The ID of the child node.
     */
    void removeParent(int child);

    /**
     * @brief Get the children of the specified node.
     *
     * @param id The ID of the node.
     * @return The set of children nodes.
     */
    std::unordered_set<int> getChildren(int id) const;

    /**
     * @brief Get the adjacent nodes of a node.
     * @param id The source node
     * @return A pairs of integers, where the first means the adjacent node and the second means the weight of the edge.
     * 
     * @note this function returns the edges if the weight of the edge is greater than 0.
     */
    const std::unordered_map<int, double> getAdjacents(int id) const override;

    /**
     * @brief Get the number of nodes in the graph.
     */
    size_t size() const override;

    /**
     * @brief Read a graph from a file.
     * @param filePath The name of the file to import the graph from
     * @param extName The extension of the file
     * 
     * @note filePath should be the path to the folder that contains the edges.* and parents.*.
     */
    void readGraph(std::string filePath, FileExtension extName) override;

    /**
     * @brief Write the graph to a file.
     * @param filePath The name of the folder to export the graph to
     * @param extName The extension of the file
     * 
     * @note graph will be exported as the children of the folder.
     */
    void writeGraph(std::string filePath, FileExtension extName) const override;

private:
    /**
     * @brief Read the graph from a file.
     * @param filePath The name of the file to import the graph from
     * @param parser The parser to read the graph
     */
    void readEdgeHelper(std::string filePath, IGraphParser &parser);

    /**
     * @brief Read the hierarchy from a file.
     * @param filePath The name of the file to import the graph from
     * @param parser The parser to read the graph
     */
    void readHierarchyHelper(std::string filePath, IGraphParser &parser);
};

/**
 * @brief The WeightedSupergraph class.
 * 
 * This class is used to represent a supergraph.
 * A supergraph is a graph that contains a set of nodes and a set of edges.
 * Each node in the supergraph is a supernode that contains a set of nodes and edges.
 * The supergraph is used to represent a kind of hierarchy of graph.
 */
class WeightedSupergraph : public IWeightedGraph {
private:
    WeightedSuperDigraph digraph;

public:
    /**
     * @brief Construct a new WeightedSuperDigraph object.
     */
    WeightedSupergraph() = default;

    /**
     * @brief Construct a new WeightedSuperDigraph object by files.
     * @param filepath The path of the file to import the graph from
     * @param extName The extension of the file
     */
    WeightedSupergraph(std::string filepath, FileExtension extName);

    /**
     * @brief Copy constructor for WeightedSuperDigraph.
     *
     * This constructor creates a new WeightedSuperDigraph object by copying the contents of another WeightedSuperDigraph object.
     *
     * @param other The WeightedSuperDigraph object to be copied.
     */
    WeightedSupergraph(const WeightedSupergraph&) = default;

    /**
     * @brief Move constructor for WeightedSuperDigraph.
     *
     * This constructor creates a new WeightedSuperDigraph object by moving the contents of another WeightedSuperDigraph object.
     *
     * @param other The WeightedSuperDigraph object to be moved.
     */
    WeightedSupergraph(WeightedSupergraph&&) = default;

    /**
     * @brief Assignment operator for WeightedSuperDigraph.
     * 
     * This operator assigns the values of a WeightedSuperDigraph object to another WeightedSuperDigraph object.
     * 
     * @param other The WeightedSuperDigraph object to be assigned.
     * @return Reference to the assigned WeightedSuperDigraph object.
     */
    WeightedSupergraph& operator=(const WeightedSupergraph&) = default;

    /**
     * @brief Set a node to the graph.
     * @param id The node to add
     */
    void setNode(int id) override;

    /**
     * @brief Set a node to the graph.
     * @param node The node to add
     */
    void setNode(WeightedSupernode &node);

    /**
     * @brief Get a node from the graph.
     * @param id The node to get
     * @return The node
     */
    WeightedSupernode getNode(int id) const;

    /**
     * @brief Remove a node from the graph.
     * @param id The node to remove
     */
    void removeNode(int id) override;

    /**
     * @brief Get the id of the graph.
     */
    std::unordered_set<int> getIds() const override;

    /**
     * @brief Add an edge between two nodes.
     * @param src The source node
     * @param dst The destination node
     * @param weight The weight of the edge
     */
    void addEdge(int src, int dst, double weight) override;

    /**
     * @brief Remove an edge between two nodes.
     * @param src The source node
     * @param dst The destination node
     */
    void removeEdge(int src, int dst) override;

    /**
     * @brief Get the weight of an edge between two nodes.
     * @param src The source node
     * @param dst The destination node
     * @return The weight of the edge
     */
    double getWeight(int src, int dst) const override;

    /**
     * @brief Set the weight of an edge between two nodes.
     * @param src The source node
     * @param dst The destination node
     * @param weight The weight of the edge
     * 
     * @note this function will set the edge weight, not update it.
     */
    void setWeight(int src, int dst, double weight) override;

    /**
     * @brief Add the weight of an edge between two nodes.
     * @param src The source node
     * @param dst The destination node
     * @param weight The weight of the edge
     * 
     * @note this function will update edge weight.
     * if the result equals to 0, the edge will be removed.
     */
    void addWeight(int src, int dst, double weight) override;

    /**
     * @brief Returns the parent of the specified node.
     *
     * @param id The ID of the node.
     * @return The ID of the parent node.
     */
    int getParent(int id) const;

    /**
     * @brief Set the parent of the specified node.
     *
     * @param child The ID of the child node.
     * @param parent The ID of the parent node.
     */
    void setParent(int child, int parent);

    /**
     * @brief Update the parent of the specified node.
     *
     * @param child The ID of the child node.
     * @param parent The ID of the parent node.
     */
    void updateParent(int child, int parent);

    /**
     * @brief Remove the parent of the specified node.
     *
     * @param child The ID of the child node.
     */
    void removeParent(int child);

    /**
     * @brief Get the children of the specified node.
     *
     * @param id The ID of the node.
     * @return The set of children nodes.
     */
    std::unordered_set<int> getChildren(int id) const;

    /**
     * @brief Get the adjacent nodes of a node.
     * @param id The source node
     * @return A pairs of integers, where the first means the adjacent node and the second means the weight of the edge.
     */
    const std::unordered_map<int, double> getAdjacents(int id) const override;

    /**
     * @brief Get the number of nodes in the graph.
     */
    size_t size() const override;

    /**
     * @brief Read a graph from a file.
     * @param filePath The name of the file to import the graph from
     * @param extName The extension of the file
     * 
     * @note filePath should be the path to the folder that contains the edges.* and parents.*.
     */
    void readGraph(std::string filePath, FileExtension extName) override;

    /**
     * @brief Write the graph to a file.
     * @param filePath The name of the folder to export the graph to
     * @param extName The extension of the file
     * 
     * @note graph will be exported as the children of the folder.
     */
    void writeGraph(std::string filePath, FileExtension extName) const override;

private:
    /**
     * @brief Read the graph from a file.
     * @param filePath The name of the file to import the graph from
     * @param parser The parser to read the graph
     */
    void readEdgeHelper(std::string filePath, IGraphParser &parser);

    /**
     * @brief Read the hierarchy from a file.
     * @param filePath The name of the file to import the graph from
     * @param parser The parser to read the graph
     */
    void readHierarchyHelper(std::string filePath, IGraphParser &parser);
};

#endif // SUPERGRAPH_HPP