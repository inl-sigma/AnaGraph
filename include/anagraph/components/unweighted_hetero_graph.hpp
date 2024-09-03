#pragma once

#ifndef UNWEIGHTED_HETERO_GRAPH_HPP
#define UNWEIGHTED_HETERO_GRAPH_HPP

#include "anagraph/components/unweighted_hetero_digraph.hpp"

namespace anagraph {
namespace graph_structure {

/**
 * @class HeteroGraph
 * @brief Represents a graph data structure.
 * 
 * @note This HeteroGraph class is indexed as 0-origin and allowed to have self-loops.
 * The HeteroGraph class provides a way to represent a graph using an adjacency list.
 * It supports adding edges between vertices and accessing the adjacency list of a node.
 */
template <typename T>
class HeteroGraph : public interface::IHeteroDigraph<T> {
private:
    HeteroDigraph<T> digraph;

public:
    /**
     * @brief Constructs a HeteroGraph object.
     */
    HeteroGraph() = default;

    /**
     * @brief Constructs a HeteroGraph object with the file.
     * @param filePath The path to the file containing the graph data.
     */
    HeteroGraph(std::string filePath, FileExtension extName);

    /**
     * @brief Copy constructor for the HeteroGraph object.
     */
    HeteroGraph(const HeteroGraph<T> &graph) = default;

    /**
     * @brief Assignment operator for the HeteroGraph object.
     */
    HeteroGraph<T>& operator=(const HeteroGraph<T>& graph) = default;

    /** 
     * @brief Get the attributes of a node.
     * @param id The node to get the attributes of
     * @return A copy of the attributes of the node
     * 
     * @todo override interface method after implementing the method
     */
    HeteroNode<T>& getNode(int id);

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
    void setNode(HeteroNode<T> &node);

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
    void setEdge(int src, int dst) override;

    /**
     * @brief Remove an edge between two nodes.
     * @param src The source node
     * @param dst The destination node
     */
    void removeEdge(int src, int dst) override;

    /**
     * @brief Get the adjacent nodes of a node.
     * @param id The source node
     * @return A set of integers representing the adjacent nodes.
     */
    const std::unordered_set<int> getAdjacents(int id) const override;

    /**
     * @brief Get the subgraph of the graph.
     * @param indices The indices of the nodes to include in the subgraph
     * @return A subgraph of the graph
     */
    HeteroGraph<T> getSubgraph(std::unordered_set<int> indices) const;

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
     * Converts the heterogeneous graph to a directed graph.
     *
     * @return The converted directed graph.
     */
    HeteroDigraph<T> toDigraph() const;

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
};

} // namespace graph_structure
} // namespace anagraph

#endif // UNWEIGHTED_HETERO_GRAPH_HPP