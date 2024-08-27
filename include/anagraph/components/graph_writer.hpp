#pragma once

#ifndef GRAPH_WRITER_HPP
#define GRAPH_WRITER_HPP

#include "anagraph/utils/graph_utils.hpp"

#include <string>
#include <vector>

class IGraphWriter {
public:
    virtual void writeGraph(std::string file_path, const std::vector<EdgeObject> &edges) = 0;
    virtual void writeWeightedGraph(std::string file_path, const std::vector<WeightedEdgeObject> &weightedEdges) = 0;
};

class TextGraphWriter : public IGraphWriter {
public:
    void writeGraph(std::string file_path, const std::vector<EdgeObject> &edges) override;

    void writeWeightedGraph(std::string file_path, const std::vector<WeightedEdgeObject> &weightedEdges) override;
};

class CSVGraphWriter : public IGraphWriter {
public:
    void writeGraph(std::string file_path, const std::vector<EdgeObject> &edges) override;

    void writeWeightedGraph(std::string file_path, const std::vector<WeightedEdgeObject> &weightedEdges) override;
};

class GMLGraphWriter : public IGraphWriter {
public:
    void writeGraph(std::string file_path, const std::vector<EdgeObject> &edges) override;

    void writeWeightedGraph(std::string file_path, const std::vector<WeightedEdgeObject> &weightedEdges) override;
};


#endif // GRAPH_WRITER_HPP