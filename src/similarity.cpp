#include "similarity.hpp"

#include <cmath>
#include <stdexcept>

namespace similarity {

double cosineSimilarity(const std::vector<double>& v1, const std::vector<double>& v2) {
    const size_t v1Size = v1.size();
    const size_t v2Size = v2.size();
    if (v1Size != v2Size) {
        throw std::invalid_argument("Vectors must have the same size");
    }
    double dotProduct = 0.0;
    double normA = 0.0;
    double normB = 0.0;
    for (size_t i = 0; i < v1Size; i++) {
        dotProduct += v1[i] * v2[i];
        normA += v1[i] * v1[i];
        normB += v2[i] * v2[i];
    }
    return dotProduct / (std::sqrt(normA) * std::sqrt(normB));
}

} // namespace similarity