#pragma once

#ifndef WEIGHTED_HETERO_DIGRAPH_HPP
#define WEIGHTED_HETERO_DIGRAPH_HPP

#include "anagraph/components/graph_parser.hpp"
#include "anagraph/components/graph_writer.hpp"
#include "anagraph/components/weighted_hetero_node.hpp"
#include "anagraph/interfaces/weighted_hetero_digraph_interface.hpp"

#include <map>

namespace anagraph {
namespace graph_structure {

/**
 * @class WeightedHeteroDigraph
 * @brief Represents a weighted graph data structure.
 * 
 * @note This WeightedHeteroDigraph class is indexed as 0-origin and allowed to have self-loops.
 * The WeightedHeteroDigraph class provides a way to represent a weighted graph using an adjacency list.
 * It supports adding edges between vertices and accessing the adjacency list of a node.
 */
template <typename T>
class WeightedHeteroDigraph : public interface::IWeightedHeteroDigraph<T> {
private:
    std::map<int, WeightedHeteroNode<T>> nodes; /**< The nodes of the graph */

public:
    /**
     * @brief Constructs a WeightedHeteroDigraph object.
     */
    WeightedHeteroDigraph() : nodes(std::map<int, WeightedHeteroNode<T>>()) {}

    /**
     * @brief Constructs a WeightedHeteroDigraph object with the file.
     * @param filePath The path to the file containing the graph data.
     */
    WeightedHeteroDigraph(std::string filePath, FileExtension extName);

    /**
     * @brief Copy constructor for the WeightedHeteroDigraph object.
     */
    WeightedHeteroDigraph(const WeightedHeteroDigraph<T> &graph) = default;

    /**
     * @brief Assignment operator for the WeightedHeteroDigraph object.
     */
    WeightedHeteroDigraph<T>& operator=(const WeightedHeteroDigraph<T>& graph) = default;

    /** 
     * @brief Get the attributes of a node.
     * @param id The node to get the attributes of
     * @return A copy of the attributes of the node
     * 
     * @todo override interface method after implementing the method
     */
    WeightedHeteroNode<T>& getNode(int id);

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
    const std::unordered_map<int, double> getAdjacents(int id) const override;

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
    void writeGraphHelper(std::string filename, IGraphWriter &writer, std::vector<WeightedEdgeObject> &edges) const;
};

} // namespace graph_structure
} // namespace anagraph

#endif // WEIGHTED_HETERO_DIGRAPH_HPP