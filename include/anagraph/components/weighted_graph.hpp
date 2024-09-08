#pragma once

#ifndef WEIGHTED_GRAPH_HPP
#define WEIGHTED_GRAPH_HPP

#include "anagraph/components/graph_parser.hpp"
#include "anagraph/components/graph_writer.hpp"
#include "anagraph/components/weighted_digraph.hpp"
#include "anagraph/interfaces/weighted_graph_interface.hpp"
#include "anagraph/utils/graph_utils.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace anagraph {
namespace graph_structure {

/**
 * @class WeightedGraph
 * @brief Represents a weighted graph data structure.
 * 
 * @note This WeightedGraph class is indexed as 0-origin and allowed to have self-loops.
 * The WeightedGraph class provides a way to represent a weighted graph using an adjacency list.
 * It supports adding edges between vertices and accessing the adjacency list of a node.
 */
class WeightedGraph : public interface::IWeightedGraph {
private:
    WeightedDigraph digraph; /**< The directed weighted graph, treated as undirected one. */

public:
    /**
     * @brief Constructs a WeightedGraph object.
     */
    WeightedGraph() : digraph(WeightedDigraph()) {}

    /**
     * @brief Constructs a WeightedGraph object with the file.
     * @param filePath The path to the file containing the graph data.
     */
    WeightedGraph(std::string filePath, FileExtension extName);

    /**
     * @brief Copy constructor for the WeightedGraph object.
     */
    WeightedGraph(const WeightedGraph &graph) = default;
    /**
     * @brief Assignment operator for the WeightedGraph object.
     */
    WeightedGraph& operator=(const WeightedGraph& digraph) = default;

    /** 
     * @brief Get the attributes of a node.
     * @param id The node to get the attributes of
     * @return A copy of the attributes of the node
     * 
     * @todo override interface method after implementing the method
     */
    WeightedNode& getNode(int id);

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
    void setEdge(int src, int dst, double weight) override;

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
    const std::unordered_map<int, double>& getAdjacents(int id) const override;

    /**
     * @brief Get the subgraph of the graph.
     * @param indices The indices of the nodes to include in the subgraph
     * @return A subgraph of the graph
     */
    WeightedGraph getSubgraph(std::unordered_set<int> indices) const;

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
     * Converts the graph to a weighted directed graph.
     * 
     * @return The weighted directed graph representation of the graph.
     */
    virtual WeightedDigraph toDigraph() const;

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

    class Iterator {
    private:
        WeightedDigraph::Iterator it;

    public:
        Iterator(WeightedDigraph::Iterator it) : it(it) {}

        WeightedNode& operator*() {
            return *it;
        }

        Iterator& operator++() {
            ++it;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return it != other.it;
        }
    };

    Iterator begin() {
        return Iterator(digraph.begin());
    };

    Iterator end() {
        return Iterator(digraph.end());
    };
};
    
} // namespace graph
} // namespace anagraph

#endif // WEIGHTED_GRAPH_HPP