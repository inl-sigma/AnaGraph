#include "anagraph/components/graph_parser.hpp"

#include <fstream>
#include <regex>
#include <sstream>
#include <vector>

namespace anagraph {

std::vector<EdgeObject> TextGraphParser::parseGraph(std::string file_path) {
	// Implement the logic to parse a graph from a text file
	// and return a vector of EdgeObjects.
    std::vector<EdgeObject> edges;
    std::ifstream file(file_path);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            int src, dst; 
            if (std::istringstream(line) >> src >> dst) {
                edges.push_back(EdgeObject(src, dst));
            }
        }
        file.close();
        return edges;
    } else {
        throw std::runtime_error("file not found: " + file_path + "\n");
    }
}

std::vector<WeightedEdgeObject> TextGraphParser::parseWeightedGraph(std::string file_path) {
	// Implement the logic to parse a weighted graph from a text file
	// and return a vector of WeightedEdgeObjects.
    std::vector<WeightedEdgeObject> weightedEdges;
    std::ifstream file(file_path);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            int src, dst;
            double weight;
            if (std::istringstream(line) >> src >> dst >> weight) {
                weightedEdges.push_back(WeightedEdgeObject(src, dst, weight));
            } else if (std::istringstream(line) >> src >> dst) {
                weightedEdges.push_back(WeightedEdgeObject(src, dst, DEFAULT_WEIGHT));

            }
        }
        file.close();
        return weightedEdges;
    } else {
        throw std::runtime_error("file not found: " + file_path + "\n");
    }
}

std::vector<EdgeObject> CSVGraphParser::parseGraph(std::string file_path) {
	// Implement the logic to parse a graph from a CSV file
	// and return a vector of EdgeObjects.
    std::vector<EdgeObject> edges;
    std::ifstream file(file_path);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            // TODO: Implement the logic to parse a graph from a CSV file
            std::regex re(R"(\d+), *(\d+)");
            std::smatch match;
            if (std::regex_match(line, match, re)) {
                int src = std::stoi(match[1]);
                int dst = std::stoi(match[2]);
                edges.push_back(EdgeObject(src, dst));
            }
        }
        file.close();
        return edges;
    } else {
        throw std::runtime_error("file not found: " + file_path + "\n");
    }
}

std::vector<WeightedEdgeObject> CSVGraphParser::parseWeightedGraph(std::string file_path) {
	// Implement the logic to parse a weighted graph from a CSV file
	// and return a vector of WeightedEdgeObjects.
    std::vector<WeightedEdgeObject> weightedEdges;
    std::ifstream file(file_path);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            // TODO: Implement the logic to parse a graph from a CSV file
            std::regex re(R"(\d+), *(\d+), *(\d+.\d+)");
            std::smatch match;
            if (std::regex_match(line, match, re)) {
                int src = std::stoi(match[1]);
                int dst = std::stoi(match[2]);
                double weight = std::stod(match[3]);
                weightedEdges.push_back(WeightedEdgeObject(src, dst, weight));
            }
        }
        file.close();
        return weightedEdges;
    } else {
        throw std::runtime_error("file not found: " + file_path + "\n");
    }
}

std::vector<EdgeObject> GMLGraphParser::parseGraph(std::string file_path) {
    // Implement the logic to parse a graph from a GML file
    // and return a vector of EdgeObjects.

    //TODO: Implement the logic to parse a graph from a GML file
}

std::vector<WeightedEdgeObject> GMLGraphParser::parseWeightedGraph(std::string file_path) {
    // Implement the logic to parse a weighted graph from a GML file
    // and return a vector of WeightedEdgeObjects.

    //TODO: Implement the logic to parse a weighted graph from a GML file
}

} // namespace anagraph