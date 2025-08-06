#pragma once

#include <string>
#include <vector>
#include <set>
#include <unordered_set>

class ShinglingCalculator {
public:
    // Generate w-shingles from text
    static std::set<std::string> generateShingles(const std::string& text, int w = 3);
    
    // Calculate Jaccard similarity between two sets of shingles
    static double calculateJaccardSimilarity(
        const std::set<std::string>& shingles1,
        const std::set<std::string>& shingles2
    );
    
    // Generate character-level shingles
    static std::set<std::string> generateCharacterShingles(const std::string& text, int w = 5);
    
    // Generate word-level shingles
    static std::set<std::string> generateWordShingles(const std::vector<std::string>& tokens, int w = 3);

private:
    static std::string normalizeText(const std::string& text);
};