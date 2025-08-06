#pragma once

#include <string>
#include <vector>
#include <set>
#include <unordered_map>

class TextProcessor {
public:
    TextProcessor();
    
    // Load stopwords from file
    void loadStopwords(const std::string& filename);
    
    // Process text and return vector of tokens
    std::vector<std::string> processText(const std::string& text) const;
    
    // Get term frequency map for a text
    std::unordered_map<std::string, double> getTermFrequencyMap(const std::string& text) const;
    
    // Set whether to ignore stopwords
    void setIgnoreStopwords(bool ignore) { ignoreStopwords = ignore; }

private:
    std::set<std::string> stopwords;
    bool ignoreStopwords;
    
    // Helper functions
    std::string toLowerCase(const std::string& text) const;
    std::vector<std::string> tokenize(const std::string& text) const;
};