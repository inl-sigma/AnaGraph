#pragma once

#ifndef WEIGHTED_DIRECTED_SUPERGRAPH_HPP
#define WEIGHTED_DIRECTED_SUPERGRAPH_HPP

#include "anagraph/components/graph_parser.hpp"
#include "anagraph/components/graph_writer.hpp"
#include "anagraph/components/weighted_supernode.hpp"
#include "anagraph/interfaces/weighted_digraph_interface.hpp"

#include <vector>

namespace anagraph {
namespace graph_structure {

/**
 * @brief The WeightedSuperDigraph class.
 * 
 * This class is used to represent a superdigraph(directed supergraph).
 * A superdigraph is a digraph that contains a set of nodes and a set of edges.
 * Each node in the superdigraph is a supernode that contains a set of nodes and edges.
 * The superdigraph is used to represent a kind of hierarchy of digraph.
 */
class WeightedSuperDigraph : public interface::IWeightedDigraph {
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
     * @note this function returns the edges of the node without considering the hierarchy.
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

} // namespace graph_structure
} // namespace anagraph

#endif // WEIGHTED_DIRECTED_SUPERGRAPH_HPP