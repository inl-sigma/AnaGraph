#pragma once

#ifndef UNWEIGHTED_GRAPH_HPP
#define UNWEIGHTED_GRAPH_HPP

#include "anagraph/components/graph_parser.hpp"
#include "anagraph/components/graph_writer.hpp"
#include "anagraph/components/unweighted_node.hpp"
#include "anagraph/components/unweighted_digraph.hpp"
#include "anagraph/interfaces/unweighted_graph_interface.hpp"
#include "anagraph/utils/graph_utils.hpp"

#include <unordered_set>
#include <vector>

namespace anagraph {
namespace graph_structure {

class Graph : public interface::IGraph {
private:
    Digraph digraph;

public:
    /**
     * @brief Default constructor for the Graph class.
     */
    Graph() = default;

    /**
     * @brief Constructs a new Graph object with the given file path and extension name.
     *
     * @param filePath The path of the file to import the graph from
     * @param extName The extension of the file
     */
    Graph(std::string filePath, FileExtension extName);

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
    Node& getNode(int id);

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
};

} // namespace graph
} // namespace anagraph

#endif // UNWEIGHTED_GRAPH_HPP