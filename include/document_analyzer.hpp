#pragma once

#include <string>
#include <vector>
#include <unordered_map>

struct DocumentStats {
    size_t wordCount = 0;
    size_t characterCount = 0;
    size_t sentenceCount = 0;
    size_t uniqueWords = 0;
    double averageWordsPerSentence = 0.0;
    double lexicalDiversity = 0.0; // unique words / total words
    std::vector<std::string> topWords;
};

struct SimilarityConfidence {
    double score = 0.0;
    std::string level; // "Low", "Medium", "High", "Very High"
    std::string interpretation;
    std::vector<std::string> indicators;
};

class DocumentAnalyzer {
public:
    // Analyze document statistics
    static DocumentStats analyzeDocument(const std::string& content, 
                                       const std::vector<std::string>& tokens);
    
    // Determine confidence level and interpretation
    static SimilarityConfidence analyzeSimilarityConfidence(
        double cosine, double tfidf, double jaccardChar, double jaccardWord);
    
    // Sentence-level similarity analysis
    static std::vector<std::pair<double, std::string>> analyzeSentenceSimilarity(
        const std::string& content1, const std::string& content2);
    
    // Generate analysis summary
    static std::string generateAnalysisSummary(
        const DocumentStats& stats1, const DocumentStats& stats2,
        const SimilarityConfidence& confidence);

private:
    static std::vector<std::string> splitIntoSentences(const std::string& text);
    static std::vector<std::string> getTopWords(
        const std::unordered_map<std::string, double>& termFreq, size_t count = 5);
};