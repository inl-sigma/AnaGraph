#pragma once

#ifndef UNWEIGHTED_HETERO_DIGRAPH_HPP
#define UNWEIGHTED_HETERO_DIGRAPH_HPP

#include "anagraph/components/graph_parser.hpp"
#include "anagraph/components/graph_writer.hpp"
#include "anagraph/components/unweighted_hetero_node.hpp"
#include "anagraph/interfaces/unweighted_hetero_digraph_interface.hpp"
#include "anagraph/utils/graph_utils.hpp"

#include <map>

namespace anagraph {
namespace graph_structure {

/**
 * @class HeteroDigraph
 * @brief Represents a weighted graph data structure.
 * 
 * @note This HeteroDigraph class is indexed as 0-origin and allowed to have self-loops.
 * The HeteroDigraph class provides a way to represent a graph using an adjacency list.
 * It supports adding edges between vertices and accessing the adjacency list of a node.
 */
template <typename T>
class HeteroDigraph : public interface::IHeteroDigraph<T> {
private:
    std::map<int, HeteroNode<T>> nodes; /**< The nodes of the graph */

public:
    /**
     * @brief Constructs a UnweightedHeteroDigraph object.
     */
    HeteroDigraph() : nodes(std::map<int, HeteroNode<T>>()) {};

    /**
     * @brief Constructs a UnweightedHeteroDigraph object with the file.
     * @param filePath The path to the file containing the graph data.
     */
    HeteroDigraph(std::string filePath, FileExtension extName);

    /**
     * @brief Copy constructor for the UnweightedHeteroDigraph object.
     */
    HeteroDigraph(const HeteroDigraph<T> &graph) = default;

    /**
     * @brief Assignment operator for the UnweightedHeteroDigraph object.
     */
    HeteroDigraph<T>& operator=(const HeteroDigraph<T>& graph) = default;

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
    HeteroDigraph<T> getSubgraph(std::unordered_set<int> indices) const;

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
    void writeGraphHelper(std::string filename, IGraphWriter &writer, std::vector<EdgeObject> &edges) const;
};

} // namespace graph_structure
} // namespace anagraph

#endif // UNWEIGHTED_HETERO_DIGRAPH_HPP