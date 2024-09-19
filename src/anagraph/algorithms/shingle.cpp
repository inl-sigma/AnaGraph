#include "anagraph/algorithms/graph_hashes.hpp"

#include <numeric>
#include <random>
#include <unordered_map>

#include <spdlog/spdlog.h>

namespace {
    const int maxCandidateGroupSize = 500; // 500
    const int recursionLimit = 10; // 10

    void AppendIndicesToCandidates(int left, int right, std::vector<int>& indices, std::vector<std::vector<int>>& candidates) {
        const int size = right - left;
        std::vector<int> temp(size); 
        std::copy(indices.begin() + left, indices.begin() + right, temp.begin());
        candidates.push_back(temp);
    }

    void SortIndicesByShingle(std::vector<int>& indices, std::unordered_map<int, int>& shingles, int left, int right) {
        spdlog::debug("called SortIndicesByShingle at left: {}, right: {}", left, right);
        if (left < right) {
            int l = left;
            int r = right;
            int pivot = shingles[indices[(l+r)/2]];
            spdlog::debug("half: {}, pivot: {}", indices[(l+r)/2], pivot);

            while (l <= r) {
                while ((l < right) && (shingles[indices[l]] < pivot)) {
                    spdlog::debug("left: idx={}, indices={}, hash={}", l, indices[l], shingles[indices[l]]);
                    l++;
                }
                while ((r > left) && (shingles[indices[r]] > pivot)) {
                    spdlog::debug("right: idx={}, indices={}, hash={}", r, indices[r], shingles[indices[r]]);
                    r--;
                } 
                if (l > r) break;
                spdlog::debug("swap: l: {}(indices={}, hash={}), r: {}(indices={}, hash={})", l, indices[l], shingles[indices[l]], 
                    r, indices[r], shingles[indices[r]]);
                std::swap(indices[l], indices[r]);
                l++;
                r--;
            }
            SortIndicesByShingle(indices, shingles, left, r);
            SortIndicesByShingle(indices, shingles, l, right);
        }
    }

    template <typename T>
    void recursiveSplitIndices(
        int left, int right, int depth, 
        std::vector<std::vector<int>> &candidates, 
        std::vector<std::vector<int>> &hashes, 
        const T& graph, 
        std::vector<int> &indices
    ) {
        if (depth == recursionLimit) {
            for (int i = left; i < right; i += maxCandidateGroupSize) {
                if (i + maxCandidateGroupSize - 1 < right) {
                    AppendIndicesToCandidates(i, i + maxCandidateGroupSize, indices, candidates);
                } else {
                    AppendIndicesToCandidates(i, right, indices, candidates);
                }
            }
            return;
        }

        std::unordered_map<int, int> shingles;
        for (int i = left; i < right; i++) {
            int root = indices[i];
            int min_hash = hashes[depth][root];
            for (int adj : graph.getAdjacents(root)) {
                min_hash = std::min(min_hash, hashes[depth][adj]);
            }
            shingles[root] = min_hash;
        }

        SortIndicesByShingle(indices, shingles, left, right - 1);

        int prv = left;
        for (int i = left + 1; i < right; i++) {
            int idx = indices[i];
            int prevIdx = indices[i - 1];
            if (shingles[idx] != shingles[prevIdx]) {
                if (i - prv <= maxCandidateGroupSize) {
                    AppendIndicesToCandidates(prv, i, indices, candidates);
                } else {
                    recursiveSplitIndices(prv, i, ++depth, candidates, hashes, graph, indices);
                }
                prv = i;
            }
        }
        if (right - prv <= maxCandidateGroupSize) {
            AppendIndicesToCandidates(prv, right, indices, candidates);
        } else {
            recursiveSplitIndices(prv, right, ++depth, candidates, hashes, graph, indices);
        }
    }

    template <typename T>
    std::vector<std::vector<int>> minHashTemplate(const T &graph) {
        const int size = graph.size();

        if (size == 0) {
            return std::vector<std::vector<int>>();
        }

        std::vector<int> indices(size);
        std::iota(indices.begin(), indices.end(), 0);

        std::random_device seed_gen;
        std::mt19937 engine(seed_gen());

        std::vector<std::vector<int>> hashes(recursionLimit, std::vector<int>(size));
        for (int i = 0; i < recursionLimit; i++) {
            std::iota(hashes[i].begin(), hashes[i].end(), 0);
            std::shuffle(hashes[i].begin(), hashes[i].end(), engine);
        }

        std::vector<std::vector<int>> candidates;
        int left_init = 0;
        int right_init = size;
        recursiveSplitIndices<T>(left_init, right_init, 0, candidates, hashes, graph, indices);

        for (auto& candidate : candidates) {
            // sort candidate by node id, because SortIndicesByShingle may change the order
            std::sort(candidate.begin(), candidate.end());
        }

        return candidates;
    }
}

namespace anagraph {
namespace hashes {

std::vector<std::vector<int>> minHash(const graph_structure::Graph &graph) {
    return minHashTemplate<graph_structure::Graph>(graph);
}

std::vector<std::vector<int>> minHash(const graph_structure::Digraph &graph) {
    return minHashTemplate<graph_structure::Digraph>(graph);
}

} // namespace hashes
} // namespace anagraph