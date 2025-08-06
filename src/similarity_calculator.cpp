#include "similarity_calculator.hpp"
#include <cmath>

double SimilarityCalculator::calculateCosineSimilarity(
    const std::unordered_map<std::string, double>& tf1,
    const std::unordered_map<std::string, double>& tf2
) {
    double dotProduct = 0.0;
    
    // Calculate dot product
    for (const auto& [term, freq] : tf1) {
        auto it = tf2.find(term);
        if (it != tf2.end()) {
            dotProduct += freq * it->second;
        }
    }
    
    // Calculate magnitudes
    double magnitude1 = calculateMagnitude(tf1);
    double magnitude2 = calculateMagnitude(tf2);
    
    // Avoid division by zero
    if (magnitude1 == 0.0 || magnitude2 == 0.0) {
        return 0.0;
    }
    
    return dotProduct / (magnitude1 * magnitude2);
}

double SimilarityCalculator::calculateMagnitude(
    const std::unordered_map<std::string, double>& tf
) {
    double sumSquares = 0.0;
    
    for (const auto& [term, freq] : tf) {
        sumSquares += freq * freq;
    }
    
    return std::sqrt(sumSquares);
}