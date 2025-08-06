#include "text_processor.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cctype>

TextProcessor::TextProcessor() : ignoreStopwords(false) {}

void TextProcessor::loadStopwords(const std::string& filename) {
    std::ifstream file(filename);
    std::string word;
    
    while (std::getline(file, word)) {
        // Remove any trailing whitespace
        word.erase(std::find_if(word.rbegin(), word.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), word.end());
        
        if (!word.empty()) {
            stopwords.insert(toLowerCase(word));
        }
    }
}

std::string TextProcessor::toLowerCase(const std::string& text) const {
    std::string result = text;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::vector<std::string> TextProcessor::tokenize(const std::string& text) const {
    std::vector<std::string> tokens;
    std::istringstream iss(text);
    std::string token;
    
    while (iss >> token) {
        // Remove punctuation from beginning and end
        token.erase(0, token.find_first_not_of(".,!?\"'();:"));
        token.erase(token.find_last_not_of(".,!?\"'();:") + 1);
        
        if (!token.empty()) {
            tokens.push_back(toLowerCase(token));
        }
    }
    
    return tokens;
}

std::vector<std::string> TextProcessor::processText(const std::string& text) const {
    std::vector<std::string> tokens = tokenize(text);
    
    if (ignoreStopwords) {
        tokens.erase(
            std::remove_if(tokens.begin(), tokens.end(),
                [this](const std::string& token) {
                    return stopwords.find(token) != stopwords.end();
                }
            ),
            tokens.end()
        );
    }
    
    return tokens;
}

std::unordered_map<std::string, double> TextProcessor::getTermFrequencyMap(
    const std::string& text) const {
    std::vector<std::string> tokens = processText(text);
    std::unordered_map<std::string, double> tfMap;
    
    // Count occurrences of each token
    for (const auto& token : tokens) {
        tfMap[token]++;
    }
    
    // Convert counts to frequencies
    double totalTokens = tokens.size();
    for (auto& pair : tfMap) {
        pair.second /= totalTokens;
    }
    
    return tfMap;
}