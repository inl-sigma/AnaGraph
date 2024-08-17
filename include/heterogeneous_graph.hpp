#pragma once

#ifndef HETEROGENEOUS_GRAPH_HPP
#define HETEROGENEOUS_GRAPH_HPP

#include "heterogeneous_graph_if.hpp"
#include "weighted_graph.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

/**
 * @class weightedNode
 * @brief Represents a weighted node data structure with attributes.
 * 
 * The weightedNode class provides a way to represent a weighted node using an adjacency list.
 * It supports adding adjacent nodes and accessing the attributes of the node.
 */
template <typename T>
class WeightedHeteroNode : public IWeightedHeteroNode<T> {
private:
    static const int UNUSED_ID; /**< The default value for an unused node */
    WeightedNode weightedNode;
    T attributes;
    bool isAttrEnabled; // todo : implement this for checking if the attributes are initialized

public:
    /**
     * @brief Constructs a weightedNode object.
     */
    WeightedHeteroNode() : weightedNode(WeightedNode()), attributes(T()), isAttrEnabled(false) {
    };

    /**
     * @brief Constructs a weightedNode object with the specified id.
     * @param id The id of the node.
     */
    WeightedHeteroNode(int id) : weightedNode(WeightedNode(id)), attributes(T()), isAttrEnabled(false) {
    };

    /**
     * @brief Constructs a weightedNode object with the specified id and attributes.
     * @param id The id of the node.
     * @param attributes The attributes of the node.
     */
    WeightedHeteroNode(int id, T attributes) : weightedNode(WeightedNode(id)), attributes(attributes), isAttrEnabled(true) {
    };

    /**
     * @brief Copy constructor for the weightedNode object.
     */
    WeightedHeteroNode(const WeightedHeteroNode<T> &node) 
        : weightedNode(node.weightedNode), attributes(node.attributes), isAttrEnabled(node.isAttrEnabled) {
    }

    /**
     * @brief Move constructor for the weightedNode object.
     */
    WeightedHeteroNode(WeightedHeteroNode<T> &&node) noexcept
        : weightedNode(std::move(node.weightedNode)), attributes(std::move(node.attributes)), 
        isAttrEnabled(std::move(node.isAttrEnabled)) 
    {};

    /**
     * @brief Assignment operator for the weightedNode object.
     */
    WeightedHeteroNode<T>& operator=(const WeightedHeteroNode<T>& node);

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

/**
 * @class WeightedHeteroDigraph
 * @brief Represents a weighted graph data structure.
 * 
 * @note This WeightedHeteroDigraph class is indexed as 0-origin and allowed to have self-loops.
 * The WeightedHeteroDigraph class provides a way to represent a weighted graph using an adjacency list.
 * It supports adding edges between vertices and accessing the adjacency list of a node.
 */
template <typename T>
class WeightedHeteroDigraph : public IWeightedHeteroDigraph<T> {
private:
    std::vector<WeightedHeteroNode<T>> nodes; /**< The nodes of the graph */
    std::unordered_set<int> usedNodes; /**< The set of used nodes */

public:
    /**
     * @brief Constructs a WeightedHeteroDigraph object.
     */
    WeightedHeteroDigraph();

    /**
     * @brief Constructs a WeightedHeteroDigraph object with the file.
     * @param filePath The path to the file containing the graph data.
     */
    WeightedHeteroDigraph(std::string filePath, FileExtension extName);

    /**
     * @brief Copy constructor for the WeightedHeteroDigraph object.
     */
    WeightedHeteroDigraph(const WeightedHeteroDigraph<T> &graph) 
        : nodes(graph.nodes), usedNodes(graph.usedNodes) {
    }

    /**
     * @brief Assignment operator for the WeightedHeteroDigraph object.
     */
    WeightedHeteroDigraph<T>& operator=(const WeightedHeteroDigraph<T>& graph);

    /** 
     * @brief Get the attributes of a node.
     * @param id The node to get the attributes of
     * @return A copy of the attributes of the node
     * 
     * @todo override interface method after implementing the method
     * 
     * This method is used to access the attributes of a node.
     */
    WeightedHeteroNode<T> getNode(int id) const;

    /**
     * @brief Set a node to the graph.
     * @param id The node to add
     */
    void setNode(int id) override;

    /**
     * @brief Set a node to the graph.
     * @param node The node to add
     * 
     * @todo override interface method after implementing the method
     */
    void setNode(WeightedHeteroNode<T> &node);

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
     */
    double getWeight(int src, int dst) const override;

    /**
     * @brief Set the weight of an edge between two nodes.
     * @param src The source node
     * @param dst The destination node
     * @param weight The weight of the edge
     * 
     * @note If the edge does not exist, throw an exception.
     */
    void setWeight(int src, int dst, double weight) override;

    /**
     * @brief Add the weight of an edge between two nodes.
     * @param src The source node
     * @param dst The destination node
     * @param weight The weight of the edge
     * 
     * This method is used to modify the weight of an edge between two nodes.
     */
    void addWeight(int src, int dst, double weight) override;

    /**
     * @brief Get the adjacent nodes of a node.
     * @param id The source node
     * @return A pairs of integers, where the first means the adjacent node and the second means the weight of the edge.
     */
    const std::unordered_map<int, double> &getAdjacents(int id) const override;

    /**
     * @brief Get the subgraph of the graph.
     * @param indices The indices of the nodes to include in the subgraph
     * @return A subgraph of the graph
     */
    WeightedHeteroDigraph<T> getSubgraph(std::unordered_set<int> indices) const;

    /**
     * @brief Reorganize the graph.
     * 
     * This method is used to reorganize the graph.
     * It can be used to delete unnecessary nodes.
     */
    void reorganize();

    /**
     * @brief Get the attributes of the nodes. 
     * @param id The id of the node to get the attributes of
     * @return A vector of the attributes of the nodes
     */
    T getAttributes(int id) const override;

    /**
     * @brief Set the attributes of the nodes.
     * @param id The id of the node to set the attributes of
     * @param attribute The attributes of the nodes
     */
    void setAttributes(int id, T attributes) override;

    /**
     * @brief Get the number of nodes in the graph.
     */
    size_t size() const override;

    /**
     * @brief Read a graph from a file.
     * @param filename The name of the file to import the graph from
     * @param extName The extension of the file
     * 
     * @todo read graph as a heterogeneous graph, not a weighted graph
     */
    void readGraph(std::string filename, FileExtension extName) override;

    /**
     * @brief Write the graph to a file.
     * @param filename The name of the file to export the graph to
     * @param extName The extension of the file
     * 
     * @todo write graph as a heterogeneous graph, not a weighted graph
     */
    void writeGraph(std::string filename, FileExtension extName) const override;

private:
    /**
     * @brief Read a graph from a file.
     * @param filename The name of the file to import the graph from
     * @param parser The parser to use to read the graph
     * 
     * This method is used to read a graph from a file.
     * It uses the specified parser to read the graph.
     */
    void readGraphHelper(std::string filename, IGraphParser &parser);

    /**
     * @brief Write the graph to a file.
     * @param filename The name of the file to export the graph to
     * @param writer The writer to use to write the graph
     * 
     * This method is used to write the graph to a file.
     * It uses the specified writer to write the graph.
     */
    void writeGraphHelper(std::string filename, IGraphWriter &writer, std::vector<WeightedEdgeObject>) const;
};

/**
 * @class WeightedHeteroGraph
 * @brief Represents a weighted graph data structure.
 * 
 * @note This WeightedHeteroGraph class is indexed as 0-origin and allowed to have self-loops.
 * The WeightedHeteroGraph class provides a way to represent a weighted graph using an adjacency list.
 * It supports adding edges between vertices and accessing the adjacency list of a node.
 */
template <typename T>
class WeightedHeteroGraph : public IWeightedHeteroGraph<T> {
private:
    WeightedHeteroDigraph<T> digraph; /**< directed graph object, treated as undirected */

public:
    /**
     * @brief Constructs a WeightedHeteroGraph object.
     */
    WeightedHeteroGraph();

    /**
     * @brief Constructs a WeightedHeteroGraph object with the file.
     * @param filePath The path to the file containing the graph data.
     */
    WeightedHeteroGraph(std::string filePath, FileExtension extName);

    /**
     * @brief Copy constructor for the WeightedHeteroGraph object.
     */
    WeightedHeteroGraph(const WeightedHeteroGraph<T> &graph) 
        : digraph(graph.digraph) {
    }

    /**
     * @brief Assignment operator for the WeightedHeteroGraph object.
     */
    WeightedHeteroGraph<T>& operator=(const WeightedHeteroGraph<T>& graph);

    /** 
     * @brief Get the attributes of a node.
     * @param id The node to get the attributes of
     * @return A copy of the attributes of the node
     * 
     * @todo override interface method after implementing the method
     * 
     * This method is used to access the attributes of a node.
     */
    WeightedHeteroNode<T> getNode(int id) const;

    /**
     * @brief Set a node to the graph.
     * @param id The node to add
     */
    void setNode(int id) override;

    /**
     * @brief Set a node to the graph.
     * @param node The node to add
     * 
     * @todo override interface method after implementing the method
     */
    void setNode(WeightedHeteroNode<T> &node);

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
     */
    double getWeight(int src, int dst) const override;

    /**
     * @brief Set the weight of an edge between two nodes.
     * @param src The source node
     * @param dst The destination node
     * @param weight The weight of the edge
     * 
     * @note If the edge does not exist, throw an exception.
     */
    void setWeight(int src, int dst, double weight) override;

    /**
     * @brief Add the weight of an edge between two nodes.
     * @param src The source node
     * @param dst The destination node
     * @param weight The weight of the edge
     * 
     * This method is used to modify the weight of an edge between two nodes.
     */
    void addWeight(int src, int dst, double weight) override;

    /**
     * @brief Get the adjacent nodes of a node.
     * @param id The source node
     * @return A pairs of integers, where the first means the adjacent node and the second means the weight of the edge.
     */
    const std::unordered_map<int, double> &getAdjacents(int id) const override;

    /**
     * @brief Get the subgraph of the graph.
     * @param indices The indices of the nodes to include in the subgraph
     * @return A subgraph of the graph
     */
    WeightedHeteroGraph<T> getSubgraph(std::unordered_set<int> indices) const;

    /**
     * @brief Reorganize the graph.
     * 
     * This method is used to reorganize the graph.
     * It can be used to delete unnecessary nodes.
     */
    void reorganize();

    /**
     * @brief Get the attributes of the nodes. 
     * @param id The id of the node to get the attributes of
     * @return A vector of the attributes of the nodes
     */
    T getAttributes(int id) const override;

    /**
     * @brief Set the attributes of the nodes.
     * @param id The id of the node to set the attributes of
     * @param attribute The attributes of the nodes
     */
    void setAttributes(int id, T attributes) override;

    /**
     * @brief Get the number of nodes in the graph.
     */
    size_t size() const override;

    /**
     * @brief Read a graph from a file.
     * @param filename The name of the file to import the graph from
     * @param extName The extension of the file
     * 
     * @todo read graph as a heterogeneous graph, not a weighted graph
     */
    void readGraph(std::string filename, FileExtension extName) override;

    /**
     * @brief Write the graph to a file.
     * @param filename The name of the file to export the graph to
     * @param extName The extension of the file
     * 
     * @todo write graph as a heterogeneous graph, not a weighted graph
     */
    void writeGraph(std::string filename, FileExtension extName) const override;

private:
    /**
     * @brief Read a graph from a file.
     * @param filename The name of the file to import the graph from
     * @param parser The parser to use to read the graph
     * 
     * This method is used to read a graph from a file.
     * It uses the specified parser to read the graph.
     */
    void readGraphHelper(std::string filename, IGraphParser &parser);

    /**
     * @brief Write the graph to a file.
     * @param filename The name of the file to export the graph to
     * @param writer The writer to use to write the graph
     * 
     * This method is used to write the graph to a file.
     * It uses the specified writer to write the graph.
     */
    void writeGraphHelper(std::string filename, IGraphWriter &writer, std::vector<WeightedEdgeObject>) const;
};

#endif // HETEROGENEOUS_GRAPH_HPP