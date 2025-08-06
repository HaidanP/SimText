#include "shingling.hpp"
#include <algorithm>
#include <sstream>
#include <cctype>

std::string ShinglingCalculator::normalizeText(const std::string& text) {
    std::string normalized;
    for (char c : text) {
        if (std::isalnum(c)) {
            normalized += std::tolower(c);
        } else if (std::isspace(c)) {
            normalized += ' ';
        }
    }
    return normalized;
}

std::set<std::string> ShinglingCalculator::generateShingles(const std::string& text, int w) {
    return generateCharacterShingles(text, w);
}

std::set<std::string> ShinglingCalculator::generateCharacterShingles(const std::string& text, int w) {
    std::set<std::string> shingles;
    std::string normalized = normalizeText(text);
    
    if (normalized.length() < static_cast<size_t>(w)) {
        shingles.insert(normalized);
        return shingles;
    }
    
    for (size_t i = 0; i <= normalized.length() - w; ++i) {
        std::string shingle = normalized.substr(i, w);
        // Skip shingles that are all spaces
        if (shingle.find_first_not_of(' ') != std::string::npos) {
            shingles.insert(shingle);
        }
    }
    
    return shingles;
}

std::set<std::string> ShinglingCalculator::generateWordShingles(
    const std::vector<std::string>& tokens, int w) {
    std::set<std::string> shingles;
    
    if (tokens.size() < static_cast<size_t>(w)) {
        std::string shingle;
        for (size_t i = 0; i < tokens.size(); ++i) {
            if (i > 0) shingle += " ";
            shingle += tokens[i];
        }
        shingles.insert(shingle);
        return shingles;
    }
    
    for (size_t i = 0; i <= tokens.size() - w; ++i) {
        std::string shingle;
        for (int j = 0; j < w; ++j) {
            if (j > 0) shingle += " ";
            shingle += tokens[i + j];
        }
        shingles.insert(shingle);
    }
    
    return shingles;
}

double ShinglingCalculator::calculateJaccardSimilarity(
    const std::set<std::string>& shingles1,
    const std::set<std::string>& shingles2) {
    
    if (shingles1.empty() && shingles2.empty()) {
        return 1.0;
    }
    
    if (shingles1.empty() || shingles2.empty()) {
        return 0.0;
    }
    
    // Calculate intersection
    std::set<std::string> intersection;
    std::set_intersection(
        shingles1.begin(), shingles1.end(),
        shingles2.begin(), shingles2.end(),
        std::inserter(intersection, intersection.begin())
    );
    
    // Calculate union
    std::set<std::string> unionSet;
    std::set_union(
        shingles1.begin(), shingles1.end(),
        shingles2.begin(), shingles2.end(),
        std::inserter(unionSet, unionSet.begin())
    );
    
    return static_cast<double>(intersection.size()) / unionSet.size();
}