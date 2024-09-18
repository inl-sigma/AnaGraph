#include "anagraph/algorithms/graph_hashes.hpp"

#include <numeric>
#include <random>
#include <unordered_map>

#include <spdlog/spdlog.h>

namespace {
    const int SEPARATE_THR = 500;
    const int MAX_DEPTH = 10;

    void append_indices_to_candidates(int left, int right, std::vector<int>& indices, std::vector<std::vector<int>>& candidates) {
        const int size = right - left;
        std::vector<int> temp(size); 
        std::copy(indices.begin() + left, indices.begin() + right, temp.begin());
        candidates.push_back(temp);
    }

    void sort_indices_by_shingle(std::vector<int>& indices, std::unordered_map<int, int>& shingles, int left, int right) {
        spdlog::debug("called sort_indices_by_shingle at left: {}, right: {}", left, right);
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
            sort_indices_by_shingle(indices, shingles, left, r);
            sort_indices_by_shingle(indices, shingles, l, right);
        }
    }

    void recursive_shingle(
        int left, int right, int depth, 
        std::vector<std::vector<int>> &candidates, 
        std::vector<std::vector<int>> &hashes, 
        const anagraph::graph_structure::Graph &graph, 
        std::vector<int> &indices
    ) {
        if (depth == MAX_DEPTH) {
            for (int i = left; i < right; i += SEPARATE_THR) {
                if (i + SEPARATE_THR - 1 < right) {
                    append_indices_to_candidates(i, i + SEPARATE_THR, indices, candidates);
                } else {
                    append_indices_to_candidates(i, right, indices, candidates);
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

        sort_indices_by_shingle(indices, shingles, left, right - 1);

        int prv = left;
        for (int i = left + 1; i < right; i++) {
            int idx = indices[i];
            int prevIdx = indices[i - 1];
            if (shingles[idx] != shingles[prevIdx]) {
                if (i - prv <= SEPARATE_THR) {
                    append_indices_to_candidates(prv, i, indices, candidates);
                } else {
                    recursive_shingle(prv, i, ++depth, candidates, hashes, graph, indices);
                }
                prv = i;
            }
        }
        if (right - prv <= SEPARATE_THR) {
            append_indices_to_candidates(prv, right, indices, candidates);
        } else {
            recursive_shingle(prv, right, ++depth, candidates, hashes, graph, indices);
        }
    }
}

namespace anagraph {
namespace hashes {

std::vector<std::vector<int>> minHash(const graph_structure::Graph &graph) {
    const int size = graph.size();

    if (size == 0) {
        return std::vector<std::vector<int>>();
    }

    std::vector<int> indices(size);
    std::iota(indices.begin(), indices.end(), 0);

    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());

    std::vector<std::vector<int>> hashes(MAX_DEPTH, std::vector<int>(size));
    for (int i = 0; i < MAX_DEPTH; i++) {
        std::iota(hashes[i].begin(), hashes[i].end(), 0);
        std::shuffle(hashes[i].begin(), hashes[i].end(), engine);
    }

    std::vector<std::vector<int>> candidates;
    int left_init = 0;
    int right_init = size;
    recursive_shingle(left_init, right_init, 0, candidates, hashes, graph, indices);

    for (auto& candidate : candidates) {
        // sort candidate by node id
        std::sort(candidate.begin(), candidate.end());
    }

    return candidates;
}

} // namespace hashes
} // namespace anagraph