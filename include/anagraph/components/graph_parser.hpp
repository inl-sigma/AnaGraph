#pragma once

#ifndef GRAPH_PARSER_HPP
#define GRAPH_PARSER_HPP

#include "anagraph/utils/graph_utils.hpp"

#include <string>
#include <vector>

namespace anagraph {

class IGraphParser {
protected:
    const double DEFAULT_WEIGHT = 1.0;
public:
    virtual std::vector<EdgeObject> parseGraph(std::string file_path) = 0;
    virtual std::vector<WeightedEdgeObject> parseWeightedGraph(std::string file_path) = 0;
};

class TextGraphParser : public IGraphParser {
public:
    std::vector<EdgeObject> parseGraph(std::string file_path) override;

    std::vector<WeightedEdgeObject> parseWeightedGraph(std::string file_path) override;
};

class CSVGraphParser : public IGraphParser {
public:
    std::vector<EdgeObject> parseGraph(std::string file_path) override;

    std::vector<WeightedEdgeObject> parseWeightedGraph(std::string file_path) override;
};

class GMLGraphParser : public IGraphParser {
public:
    std::vector<EdgeObject> parseGraph(std::string file_path) override;

    std::vector<WeightedEdgeObject> parseWeightedGraph(std::string file_path) override;
};

} // namespace anagraph

#endif // GRAPH_PARSER_HPP