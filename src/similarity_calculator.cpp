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

double SimilarityCalculator::calculateTfIdfCosineSimilarity(
    const std::unordered_map<std::string, double>& tf1,
    const std::unordered_map<std::string, double>& tf2,
    const std::unordered_map<std::string, double>& idf
) {
    double dotProduct = 0.0;
    
    // Calculate TF-IDF weighted dot product
    for (const auto& [term, freq1] : tf1) {
        auto it2 = tf2.find(term);
        auto idfIt = idf.find(term);
        
        if (it2 != tf2.end() && idfIt != idf.end()) {
            double tfidf1 = freq1 * idfIt->second;
            double tfidf2 = it2->second * idfIt->second;
            dotProduct += tfidf1 * tfidf2;
        }
    }
    
    // Calculate TF-IDF weighted magnitudes
    double magnitude1 = calculateTfIdfMagnitude(tf1, idf);
    double magnitude2 = calculateTfIdfMagnitude(tf2, idf);
    
    // Avoid division by zero
    if (magnitude1 == 0.0 || magnitude2 == 0.0) {
        return 0.0;
    }
    
    return dotProduct / (magnitude1 * magnitude2);
}

std::unordered_map<std::string, double> SimilarityCalculator::calculateIdf(
    const std::vector<std::unordered_map<std::string, double>>& documents
) {
    std::unordered_map<std::string, double> idf;
    std::unordered_map<std::string, int> documentFreq;
    
    // Count document frequency for each term
    for (const auto& doc : documents) {
        for (const auto& [term, freq] : doc) {
            documentFreq[term]++;
        }
    }
    
    // Calculate IDF: log(N / df)
    double totalDocs = static_cast<double>(documents.size());
    for (const auto& [term, df] : documentFreq) {
        idf[term] = std::log(totalDocs / df);
    }
    
    return idf;
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

double SimilarityCalculator::calculateTfIdfMagnitude(
    const std::unordered_map<std::string, double>& tf,
    const std::unordered_map<std::string, double>& idf
) {
    double sumSquares = 0.0;
    
    for (const auto& [term, freq] : tf) {
        auto idfIt = idf.find(term);
        if (idfIt != idf.end()) {
            double tfidf = freq * idfIt->second;
            sumSquares += tfidf * tfidf;
        }
    }
    
    return std::sqrt(sumSquares);
}