#pragma once

#ifndef SIMILARITY_HPP
#define SIMILARITY_HPP

#include <vector>

namespace similarity {
    /**
     * Calculates the cosine similarity between two vectors.
     *
     * @param v1 The first vector.
     * @param v2 The second vector.
     * @return The cosine similarity between v1 and v2.
     */
    double cosineSimilarity(const std::vector<double>& v1, const std::vector<double>& v2);

} // namespace similarity

#endif // SIMILARITY_HPP