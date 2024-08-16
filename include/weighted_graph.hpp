#pragma once

#ifndef WEIGHTED_GRAPH_HPP
#define WEIGHTED_GRAPH_HPP

#include "graph_interface.hpp"
#include "graph_parser.hpp"
#include "graph_writer.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @class weightedNode
 * @brief Represents a weighted node data structure.
 * 
 * The weightedNode class provides a way to represent a weighted node using an adjacency list.
 * It supports adding adjacent nodes and accessing the attributes of the node.
 */
class WeightedNode : public IWeightedNode {
private:
    int id; /**< The id of the node */
    std::unordered_map<int, double> adjacents; /**< The adjacent nodes of the node */

public:
    static const int UNUSED_ID; /**< The default value for an unused node */
    /**
     * @brief Constructs a weightedNode object.
     */
    WeightedNode();

    /**
     * @brief Constructs a weightedNode object with the specified id.
     * @param id The id of the node.
     */
    WeightedNode(int id);

    /**
     * @brief Copy constructor for the weightedNode object.
     */
    WeightedNode(const WeightedNode &node) 
        : id(node.id), adjacents(node.adjacents) {
    }

    /**
     * @brief Assignment operator for the weightedNode object.
     */
    WeightedNode& operator=(const WeightedNode& node);

    /**
     * @brief Move constructor for the weightedNode object.
     */
    WeightedNode(WeightedNode &&node) noexcept
        : id(node.id), adjacents(std::move(node.adjacents)) {
        node.clear();
    }

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
     * @brief Clear the attributes of the node.
     */
    void clear() override;
};

/**
 * @class WeightedDigraph
 * @brief Represents a weighted graph data structure.
 * 
 * @note This WeightedGraph class is indexed as 0-origin and allowed to have self-loops.
 * The WeightedGraph class provides a way to represent a weighted graph using an adjacency list.
 * It supports adding edges between vertices and accessing the adjacency list of a node.
 */
class WeightedDigraph : public IWeightedDigraph {
private:
    std::vector<WeightedNode> nodes; /**< The adjacency list representing the graph. */
    std::unordered_set<int> usedNodes; /**< The set of used nodes. */

public:
    /**
     * @brief Constructs a WeightedGraph object.
     */
    WeightedDigraph();

    /**
     * @brief Constructs a WeightedGraph object with the file.
     * @param filePath The path to the file containing the graph data.
     */
    WeightedDigraph(std::string filePath, FileExtension extName);

    /**
     * @brief Copy constructor for the WeightedGraph object.
     */
    WeightedDigraph(const WeightedDigraph &graph) 
        : nodes(graph.nodes), usedNodes(graph.usedNodes) {
    }

    /**
     * @brief Assignment operator for the WeightedGraph object.
     */
    WeightedDigraph& operator=(const WeightedDigraph& graph);

    /**
     * @brief Get the id of the graph.
     */
    std::unordered_set<int> getIds() const override;

    /** 
     * @brief Get the attributes of a node.
     * @param id The node to get the attributes of
     * @return A copy of the attributes of the node
     * 
     * @todo override interface method after implementing the method
     * 
     * This method is used to access the attributes of a node.
     */
    WeightedNode getNode(int id) const;

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
    void setNode(WeightedNode &node);

    /**
     * @brief Remove a node from the graph.
     * @param id The node to remove
     */
    void removeNode(int id) override;

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
    WeightedDigraph getSubgraph(std::unordered_set<int> indices) const;

    /**
     * @brief Organize the graph.
     * 
     * This method is used to organize the graph.
     * It can be used to delete unnecessary nodes.
     */
    void organize();

    /**
     * @brief Get the number of nodes in the graph.
     */
    size_t size() const override;

    /**
     * @brief Read a graph from a file.
     * @param filename The name of the file to import the graph from
     * @param extName The extension of the file
     */
    void readGraph(std::string filename, FileExtension extName) override;

    /**
     * @brief Write the graph to a file.
     * @param filename The name of the file to export the graph to
     * @param extName The extension of the file
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
 * @class WeightedGraph
 * @brief Represents a weighted graph data structure.
 * 
 * @note This WeightedGraph class is indexed as 0-origin and allowed to have self-loops.
 * The WeightedGraph class provides a way to represent a weighted graph using an adjacency list.
 * It supports adding edges between vertices and accessing the adjacency list of a node.
 */
class WeightedGraph : public IWeightedGraph {
private:
    WeightedDigraph digraph; /**< The directed weighted graph, treated as undirected one. */

public:
    /**
     * @brief Constructs a WeightedGraph object.
     */
    WeightedGraph();

    /**
     * @brief Constructs a WeightedGraph object with the file.
     * @param filePath The path to the file containing the graph data.
     */
    WeightedGraph(std::string filePath, FileExtension extName);

    /**
     * @brief Copy constructor for the WeightedGraph object.
     */
    WeightedGraph(const WeightedGraph &graph) 
        : digraph(graph.digraph) {
    }

    /**
     * @brief Assignment operator for the WeightedGraph object.
     */
    WeightedGraph& operator=(const WeightedGraph& digraph);

    /** 
     * @brief Get the attributes of a node.
     * @param id The node to get the attributes of
     * @return A copy of the attributes of the node
     * 
     * @todo override interface method after implementing the method
     * 
     * This method is used to access the attributes of a node.
     */
    WeightedNode getNode(int id) const;

    /**
     * @brief Set a node to the graph.
     * @param id The node to add
     */
    void setNode(int id) override;

    /**
     * @brief Get the id of the graph.
     */
    std::unordered_set<int> getIds() const override;

    /**
     * @brief Set a node to the graph.
     * @param node The node to add
     * 
     * @todo override interface method after implementing the method
     */
    void setNode(WeightedNode &node);

    /**
     * @brief Remove a node from the graph.
     * @param id The node to remove
     */
    void removeNode(int id) override;

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
    WeightedGraph getSubgraph(std::unordered_set<int> indices) const;

    /**
     * @brief Organize the graph.
     * 
     * This method is used to organize the graph.
     * It can be used to delete unnecessary nodes.
     */
    void organize();

    /**
     * @brief Get the number of nodes in the graph.
     */
    size_t size() const override;

    /**
     * @brief Read a graph from a file.
     * @param filename The name of the file to import the graph from
     * @param extName The extension of the file
     */
    void readGraph(std::string filename, FileExtension extName) override;

    /**
     * @brief Write the graph to a file.
     * @param filename The name of the file to export the graph to
     * @param extName The extension of the file
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

#endif // WEIGHTED_GRAPH_HPP