#include "graph_writer.hpp"

#include <iostream>
#include <fstream>

void TextGraphWriter::writeGraph(std::string file_path, const std::vector<EdgeObject>& edges) {
    // Implement the logic to write a graph to a text file.
    std::ofstream file(file_path);
    if (file.is_open()) {
        for (const auto& [src, dst] : edges) {
            file << src << " " << dst << "\n";
        }
        file.close();
    } else {
        throw std::runtime_error("file not found: " + file_path + "\n");
    }
}

void TextGraphWriter::writeWeightedGraph(std::string file_path, const std::vector<WeightedEdgeObject>& weightedEdges) {
    // Implement the logic to write a weighted graph to a text file.
    std::ofstream file(file_path);
    if (file.is_open()) {
        for (const auto& [src, dst, weight] : weightedEdges) {
            file << src << " " << dst << " " << weight << "\n";
        }
        file.close();
    } else {
        throw std::runtime_error("file not found: " + file_path + "\n");
    }
}

void CSVGraphWriter::writeGraph(std::string file_path, const std::vector<EdgeObject>& edges) {
    // Implement the logic to write a graph to a CSV file.
    std::ofstream file(file_path);
    if (file.is_open()) {
        for (const auto& [src, dst] : edges) {
            file << src << "," << dst << "\n";
        }
        file.close();
    } else {
        throw std::runtime_error("file not found: " + file_path + "\n");
    }
}

void CSVGraphWriter::writeWeightedGraph(std::string file_path, const std::vector<WeightedEdgeObject>& weightedEdges) {
    // Implement the logic to write a weighted graph to a CSV file.
    std::ofstream file(file_path);
    if (file.is_open()) {
        for (const auto& [src, dst, weight] : weightedEdges) {
            file << src << "," << dst << "," << weight << "\n";
        }
        file.close();
    } else {
        throw std::runtime_error("file not found: " + file_path + "\n");
    }
}