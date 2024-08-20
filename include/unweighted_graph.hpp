#pragma once

#ifndef UNWEIGHTED_GRAPH_HPP
#define UNWEIGHTED_GRAPH_HPP

#include "graph_interface.hpp"
#include "graph_parser.hpp"
#include "graph_writer.hpp"

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

class Digraph : public IDigraph {
private:
    std::vector<Node> nodes;
    std::unordered_set<int> usedNodes;

public:
    /**
     * @brief Default constructor for the Digraph class.
     */
    Digraph() = default;

    /**
     * @brief Copy constructor for the Digraph class.
     */
    Digraph(const Digraph& digraph) = default;

        /** 
     * @brief Get the attributes of a node.
     * @param id The node to get the attributes of
     * @return A copy of the attributes of the node
     * 
     * @todo override interface method after implementing the method
     */
    Node getNode(int id) const;

    /**
     * @brief Set a node to the graph.
     * @param id The node to add
     */
    void setNode(int id);

    /**
     * @brief Set a node to the graph.
     * @param node The node to add
     * 
     * @todo override interface method after implementing the method
     */
    void setNode(Node &node);

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
     */
    void addEdge(int src, int dst) override;

    /**
     * @brief Remove an edge between two nodes.
     * @param src The source node
     * @param dst The destination node
     */
    void removeEdge(int src, int dst) override;

    /**
     * @brief Get the adjacent nodes of a node.
     * @param id The source node
     */
    const std::unordered_set<int> getAdjacents(int id) const override;

    /**
     * @brief Get the subgraph of the graph.
     * @param indices The indices of the nodes to include in the subgraph
     * @return A subgraph of the graph
     */
    Digraph getSubgraph(std::unordered_set<int> indices) const;

    /**
     * @brief Reorganize the graph.
     * 
     * This method is used to reorganize the graph.
     * It can be used to delete unnecessary nodes.
     */
    void reorganize();

    /**
     * @brief Get the number of nodes in the graph.
     */
    size_t size() const override;

    /**
     * @brief Read a graph from a file.
     * @param filePath The path of the file to import the graph from
     * @param extName The extension of the file
     */
    void readGraph(std::string filePath, FileExtension extName) override;

    /**
     * @brief Write the graph to a file.
     * @param filePath The path of the file to export the graph to
     * @param extName The extension of the file
     */
    void writeGraph(std::string filePath, FileExtension extName) const override;

private:
    /**
     * @brief Read a graph from a file.
     * @param filePath The path of the file to import the graph from
     * @param parser The parser to use to read the graph
     * 
     * This method is used to read a graph from a file.
     * It uses the specified parser to read the graph.
     */
    void readGraphHelper(std::string filePath, IGraphParser &parser);

    /**
     * @brief Write the graph to a file.
     * @param filePath The path of the file to export the graph to
     * @param writer The writer to use to write the graph
     * 
     * This method is used to write the graph to a file.
     * It uses the specified writer to write the graph.
     */
    void writeGraphHelper(std::string filePath, IGraphWriter &writer, std::vector<EdgeObject> &edges) const;
};

class Graph : public IGraph {
private:
    Digraph digraph;

public:
    /**
     * @brief Default constructor for the Graph class.
     */
    Graph() = default;

    /**
     * @brief Copy constructor for the Graph class.
     */
    Graph(const Graph& graph) = default;

        /** 
     * @brief Get the attributes of a node.
     * @param id The node to get the attributes of
     * @return A copy of the attributes of the node
     * 
     * @todo override interface method after implementing the method
     */
    Node getNode(int id) const;

    /**
     * @brief Set a node to the graph.
     * @param id The node to add
     */
    void setNode(int id);

    /**
     * @brief Set a node to the graph.
     * @param node The node to add
     * 
     * @todo override interface method after implementing the method
     */
    void setNode(Node &node);

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
     */
    void addEdge(int src, int dst) override;

    /**
     * @brief Remove an edge between two nodes.
     * @param src The source node
     * @param dst The destination node
     */
    void removeEdge(int src, int dst) override;

    /**
     * @brief Get the adjacent nodes of a node.
     * @param id The source node
     */
    const std::unordered_set<int> getAdjacents(int id) const override;

    /**
     * @brief Get the subgraph of the graph.
     * @param indices The indices of the nodes to include in the subgraph
     * @return A subgraph of the graph
     */
    Graph getSubgraph(std::unordered_set<int> indices) const;

    /**
     * @brief Reorganize the graph.
     * 
     * This method is used to reorganize the graph.
     * It can be used to delete unnecessary nodes.
     */
    void reorganize();

    /**
     * @brief Convert the graph to a digraph.
     * @return A digraph representation of the graph
     */
    Digraph toDigraph() const;

    /**
     * @brief Get the number of nodes in the graph.
     */
    size_t size() const override;

    /**
     * @brief Read a graph from a file.
     * @param filePath The path of the file to import the graph from
     * @param extName The extension of the file
     */
    void readGraph(std::string filePath, FileExtension extName) override;

    /**
     * @brief Write the graph to a file.
     * @param filePath The path of the file to export the graph to
     * @param extName The extension of the file
     */
    void writeGraph(std::string filePath, FileExtension extName) const override;

private:
    /**
     * @brief Read a graph from a file.
     * @param filePath The path of the file to import the graph from
     * @param parser The parser to use to read the graph
     * 
     * This method is used to read a graph from a file.
     * It uses the specified parser to read the graph.
     */
    void readGraphHelper(std::string filePath, IGraphParser &parser);

    /**
     * @brief Write the graph to a file.
     * @param filePath The path of the file to export the graph to
     * @param writer The writer to use to write the graph
     * 
     * This method is used to write the graph to a file.
     * It uses the specified writer to write the graph.
     */
    void writeGraphHelper(std::string filePath, IGraphWriter &writer, std::vector<EdgeObject> &edges) const;
};

#endif // UNWEIGHTED_GRAPH_HPP