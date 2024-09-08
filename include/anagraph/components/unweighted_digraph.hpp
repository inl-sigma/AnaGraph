#pragma once

#ifndef UNWEIGHTED_DIGRAPH_HPP
#define UNWEIGHTED_DIGRAPH_HPP

#include "anagraph/components/unweighted_node.hpp"
#include "anagraph/components/graph_parser.hpp"
#include "anagraph/components/graph_writer.hpp"
#include "anagraph/interfaces/unweighted_digraph_interface.hpp"
#include "anagraph/utils/graph_utils.hpp"

#include <unordered_set>

namespace anagraph {
namespace graph_structure {

class Digraph : public interface::IDigraph {
private:
    std::map<int, Node> nodes;

public:
    /**
     * @brief Default constructor for the Digraph class.
     */
    Digraph() = default;

    /**
     * @brief Constructs a new Digraph object with the given file path and extension name.
     *
     * @param filePath The path of the file to import the graph from
     * @param extName The extension of the file
     */
    Digraph(std::string filePath, FileExtension extName);

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
     */
    const std::unordered_set<int>& getAdjacents(int id) const override;

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

    class Iterator {
    private:
        std::map<int, Node>::iterator it;

    public:
        Iterator(std::map<int, Node>::iterator it) : it(it) {}

        Node& operator*() {
            return it->second;
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
        return Iterator(nodes.begin());
    }

    Iterator end() {
        return Iterator(nodes.end());
    }

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

} // namespace graph
} // namespace anagraph

#endif // UNWEIGHTED_DIGRAPH_HPP