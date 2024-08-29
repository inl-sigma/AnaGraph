#include "anagraph/algorithms/similarity.hpp"

#include <spdlog/spdlog.h>

#include <cmath>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

namespace {
    using namespace anagraph;
    std::tuple<int, int, int, int> calcConfusionMatrix(const graph_structure::Digraph &expected, const graph_structure::Digraph &answer) {
        int tp = 0;
        int fp = 0;
        int fn = 0;
        int tn = 0;
        std::unordered_set<int> nodes = expected.getIds();

        for (int node : nodes) {
            const std::unordered_set<int> expectedAdjacents = expected.getAdjacents(node);
            const std::unordered_set<int> answerAdjacents = answer.getAdjacents(node);
            for (int adj : nodes) {
                if (node == adj) {
                    continue; // exclude self loop
                }
                const bool isPositive = expectedAdjacents.contains(adj);
                const bool isTrue = answerAdjacents.contains(adj);
                if (isPositive && isTrue) {
                    tp++;
                } else if (isPositive && !isTrue) {
                    fp++;
                } else if (!isPositive && isTrue) {
                    fn++;
                } else { // !isPositive && !isTrue
                    tn++;
                }
            }
        }
        return std::make_tuple(tp, fp, fn, tn);
    }

    std::tuple<int, int, int, int> calcConfusionMatrix(const graph_structure::Graph &expected, const graph_structure::Graph &answer) {
        int tp = 0;
        int fp = 0;
        int fn = 0;
        int tn = 0;
        std::unordered_set<int> nodes = expected.getIds();

        for (int node : nodes) {
            const std::unordered_set<int> expectedAdjacents = expected.getAdjacents(node);
            const std::unordered_set<int> answerAdjacents = answer.getAdjacents(node);
            for (int adj : nodes) {
                if (node >= adj) {
                    continue; // exclude self loop
                }
                const bool isPositive = expectedAdjacents.contains(adj);
                const bool isTrue = answerAdjacents.contains(adj);
                if (isPositive && isTrue) {
                    tp++;
                } else if (isPositive && !isTrue) {
                    fp++;
                } else if (!isPositive && isTrue) {
                    fn++;
                } else { // !isPositive && !isTrue
                    tn++;
                }
            }
        }
        return std::make_tuple(tp, fp, fn, tn);
    }
}

namespace anagraph {
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

double KLdivergence(const std::vector<double>& p, const std::vector<double>& q) {
    const size_t pSize = p.size();
    const size_t qSize = q.size();
    if (pSize != qSize) {
        throw std::invalid_argument("Vectors must have the same size");
    }

    double kl = 0.0;
    for (size_t i = 0; i < pSize; i++) {
        if (p[i] == 0.0) {
            continue;
        }
        if (q[i] == 0.0) {
            throw std::invalid_argument("q must not contain 0.0 when p contains non-zero value");
        }
        kl += p[i] * std::log2(p[i] / q[i]);
    }
    return kl;
}

double JSdivergence(const std::vector<double>& p, const std::vector<double>& q) {
    const size_t pSize = p.size();
    const size_t qSize = q.size();
    if (pSize != qSize) {
        throw std::invalid_argument("Vectors must have the same size");
    }

    std::vector<double> m(pSize);
    for (size_t i = 0; i < pSize; i++) {
        m[i] = (p[i] + q[i]) / 2;
    }
    return (KLdivergence(p, m) + KLdivergence(q, m)) / 2;
}

double accuracy(const graph_structure::Digraph &expected, const graph_structure::Digraph &answer) {
    auto [tp, fp, fn, tn] = calcConfusionMatrix(expected, answer);
    spdlog::debug("tp/fp/fn/tn = {}/{}/{}/{}", tp, fp, fn, tn);
    if (tp + fp + fn + tn == 0) {
        return 0.0;
    } else {
        return static_cast<double>(tp + tn) / (tp + fp + fn + tn);
    }
}

double accuracy(const graph_structure::Graph &expected, const graph_structure::Graph &answer) {
    auto [tp, fp, fn, tn] = calcConfusionMatrix(expected, answer);
    spdlog::debug("tp/fp/fn/tn = {}/{}/{}/{}", tp, fp, fn, tn);
    if (tp + fp + fn + tn == 0) {
        return 0.0;
    } else {
        return static_cast<double>(tp + tn) / (tp + fp + fn + tn);
    }
}

double precision(const graph_structure::Digraph &expected, const graph_structure::Digraph &answer) {
    auto [tp, fp, fn, tn] = calcConfusionMatrix(expected, answer);
    spdlog::debug("tp/fp/fn/tn = {}/{}/{}/{}", tp, fp, fn, tn);
    if (tp + fp == 0) {
        return 0.0;
    } else {
        return static_cast<double>(tp) / (tp + fp);
    }
}

double precision(const graph_structure::Graph &expected, const graph_structure::Graph &answer) {
    auto [tp, fp, fn, tn] = calcConfusionMatrix(expected, answer);
    spdlog::debug("tp/fp/fn/tn = {}/{}/{}/{}", tp, fp, fn, tn);
    if (tp + fp == 0) {
        return 0.0;
    } else {
        return static_cast<double>(tp) / (tp + fp);
    }
}

double recall(const graph_structure::Digraph &expected, const graph_structure::Digraph &answer) {
    auto [tp, fp, fn, tn] = calcConfusionMatrix(expected, answer);
    spdlog::debug("tp/fp/fn/tn = {}/{}/{}/{}", tp, fp, fn, tn);
    if (tp + fn == 0) {
        return 0.0;
    } else {
        return static_cast<double>(tp) / (tp + fn);
    }
}

double recall(const graph_structure::Graph &expected, const graph_structure::Graph &answer) {
    auto [tp, fp, fn, tn] = calcConfusionMatrix(expected, answer);
    spdlog::debug("tp/fp/fn/tn = {}/{}/{}/{}", tp, fp, fn, tn);
    if (tp + fn == 0) {
        return 0.0;
    } else {
        return static_cast<double>(tp) / (tp + fn);
    }
}

double fMeasure(const graph_structure::Digraph &expected, const graph_structure::Digraph &answer) {
    auto [tp, fp, fn, tn] = calcConfusionMatrix(expected, answer);
    spdlog::debug("tp/fp/fn/tn = {}/{}/{}/{}", tp, fp, fn, tn);
    const double precisionValue = ((tp + fp)) != 0 ? static_cast<double>(tp) / (tp + fp) : 0.0;
    const double recallValue = ((tp + fn) != 0) ? static_cast<double>(tp) / (tp + fn) : 0.0;
    return 2 * (precisionValue * recallValue) / (precisionValue + recallValue);
}

double fMeasure(const graph_structure::Graph &expected, const graph_structure::Graph &answer) {
    auto [tp, fp, fn, tn] = calcConfusionMatrix(expected, answer);
    spdlog::debug("tp/fp/fn/tn = {}/{}/{}/{}", tp, fp, fn, tn);
    const double precisionValue = ((tp + fp)) != 0 ? static_cast<double>(tp) / (tp + fp) : 0.0;
    const double recallValue = ((tp + fn) != 0) ? static_cast<double>(tp) / (tp + fn) : 0.0;
    return 2 * (precisionValue * recallValue) / (precisionValue + recallValue);
}

} // namespace similarity
} // namespace anagraph