#include "similarity.hpp"

#include <cmath>
#include <stdexcept>
#include <unordered_map>

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

double nDCG(const std::vector<double>& expected, const std::vector<double>& answer, int k) {
    const size_t v1Size = expected.size();
    const size_t v2Size = answer.size();
    if (v1Size != v2Size) {
        throw std::invalid_argument("Vectors must have the same size");
    }
    if (static_cast<size_t>(k) > v1Size) {
        throw std::invalid_argument("k must be less than or equal to the size of the vectors");
    }
    if (k <= 0) {
        throw std::invalid_argument("k must be greater than 0");
    }

    std::vector<std::pair<double, int>> expectedMap;
    std::vector<std::pair<double, int>> answerMap;
    for (size_t i = 0; i < expected.size(); i++) {
        expectedMap.emplace_back(std::make_pair(expected[i], i));
        answerMap.emplace_back(std::make_pair(answer[i], i));
    }
    std::sort(expectedMap.begin(), expectedMap.end(), [](const std::pair<double, int>& a, const std::pair<double, int>& b) {
        return a.first > b.first;
    });
    std::sort(answerMap.begin(), answerMap.end(), [](const std::pair<double, int>& a, const std::pair<double, int>& b) {
        return a.first > b.first;
    });
    
    double dcg = 0.0;
    double idcg = 0.0;
    for (int i = 0; i < k; i++) {
        int expectedIdx = expectedMap[i].second;
        int answerIdx = answerMap[i].second;

        if (expectedIdx == answerIdx) {
            dcg += (1 / std::log2(i + 2));
        }
        idcg += (1 / std::log2(i + 2));
    }
    return dcg / idcg;
}

double nDCG(const std::vector<double>& expected, const std::vector<double>& answer) {
    return nDCG(expected, answer, expected.size());
}


} // namespace similarity