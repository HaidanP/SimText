#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class SimilarityCalculator {
public:
    // Calculate cosine similarity between two texts
    static double calculateCosineSimilarity(
        const std::unordered_map<std::string, double>& tf1,
        const std::unordered_map<std::string, double>& tf2
    );

private:
    // Helper function to calculate vector magnitude
    static double calculateMagnitude(const std::unordered_map<std::string, double>& tf);
};