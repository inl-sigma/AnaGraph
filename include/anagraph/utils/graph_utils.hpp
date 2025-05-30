#pragma once

#ifndef GRAPH_UTILS_HPP
#define GRAPH_UTILS_HPP

#include <tuple>

namespace anagraph {

enum class FileExtension {
    TXT,
    CSV,
    GML,
};

// Todo : implement as data class, with structed binding
using EdgeObject = std::tuple<int, int>;

// Todo : implement as data class, with structed binding
using WeightedEdgeObject = std::tuple<int, int, double>;

enum class GmlObjectType {
    GRAPH,
    NODE,
    EDGE,
    WEIGHT
};

} // namespace anagraph

#endif // GRAPH_UTILS_HPP