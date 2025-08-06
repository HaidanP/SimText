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
    
    // Calculate TF-IDF weighted cosine similarity
    static double calculateTfIdfCosineSimilarity(
        const std::unordered_map<std::string, double>& tf1,
        const std::unordered_map<std::string, double>& tf2,
        const std::unordered_map<std::string, double>& idf
    );
    
    // Calculate IDF values from multiple documents
    static std::unordered_map<std::string, double> calculateIdf(
        const std::vector<std::unordered_map<std::string, double>>& documents
    );

private:
    // Helper function to calculate vector magnitude
    static double calculateMagnitude(const std::unordered_map<std::string, double>& tf);
    
    // Helper function to calculate TF-IDF weighted magnitude
    static double calculateTfIdfMagnitude(
        const std::unordered_map<std::string, double>& tf,
        const std::unordered_map<std::string, double>& idf
    );
};