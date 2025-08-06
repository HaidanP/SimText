#include "document_analyzer.hpp"
#include "text_processor.hpp"
#include "similarity_calculator.hpp"
#include <algorithm>
#include <sstream>
#include <regex>
#include <iomanip>

DocumentStats DocumentAnalyzer::analyzeDocument(const std::string& content, 
                                               const std::vector<std::string>& tokens) {
    DocumentStats stats;
    
    stats.wordCount = tokens.size();
    stats.characterCount = content.length();
    
    // Count sentences (rough estimate using punctuation)
    std::regex sentenceRegex(R"([.!?]+)");
    std::sregex_iterator iter(content.begin(), content.end(), sentenceRegex);
    std::sregex_iterator end;
    stats.sentenceCount = std::distance(iter, end);
    if (stats.sentenceCount == 0) stats.sentenceCount = 1; // At least one sentence
    
    // Count unique words
    std::set<std::string> uniqueTokens(tokens.begin(), tokens.end());
    stats.uniqueWords = uniqueTokens.size();
    
    // Calculate derived metrics
    stats.averageWordsPerSentence = static_cast<double>(stats.wordCount) / stats.sentenceCount;
    stats.lexicalDiversity = stats.wordCount > 0 ? 
        static_cast<double>(stats.uniqueWords) / stats.wordCount : 0.0;
    
    return stats;
}

SimilarityConfidence DocumentAnalyzer::analyzeSimilarityConfidence(
    double cosine, double tfidf, double jaccardChar, double jaccardWord) {
    
    SimilarityConfidence confidence;
    
    // Calculate weighted average (cosine and jaccard are most reliable)
    double weightedScore = (cosine * 0.4) + (jaccardChar * 0.3) + 
                          (jaccardWord * 0.2) + (tfidf * 0.1);
    confidence.score = weightedScore;
    
    // Determine confidence level and interpretation
    if (weightedScore >= 0.85) {
        confidence.level = "Very High";
        confidence.interpretation = "Extremely high similarity - likely identical or near-identical content";
        confidence.indicators.push_back("Potential copy-paste plagiarism");
        confidence.indicators.push_back("Review immediately");
    }
    else if (weightedScore >= 0.70) {
        confidence.level = "High";
        confidence.interpretation = "High similarity - significant content overlap detected";
        confidence.indicators.push_back("Possible paraphrasing or heavy adaptation");
        confidence.indicators.push_back("Manual review recommended");
    }
    else if (weightedScore >= 0.50) {
        confidence.level = "Medium";
        confidence.interpretation = "Moderate similarity - some shared concepts or phrases";
        confidence.indicators.push_back("May share common sources or ideas");
        confidence.indicators.push_back("Consider context and field norms");
    }
    else if (weightedScore >= 0.30) {
        confidence.level = "Low";
        confidence.interpretation = "Low similarity - minimal content overlap";
        confidence.indicators.push_back("Likely original content");
        confidence.indicators.push_back("Normal similarity for same topic");
    }
    else {
        confidence.level = "Very Low";
        confidence.interpretation = "Very low similarity - distinct content";
        confidence.indicators.push_back("Content appears original");
        confidence.indicators.push_back("No plagiarism concerns");
    }
    
    // Add specific algorithm insights
    if (cosine > 0.8) {
        confidence.indicators.push_back("High word frequency similarity");
    }
    if (jaccardChar > 0.7) {
        confidence.indicators.push_back("Similar character patterns detected");
    }
    if (jaccardWord > 0.6) {
        confidence.indicators.push_back("Similar phrase structures found");
    }
    if (tfidf > 0.5) {
        confidence.indicators.push_back("Shared rare or distinctive terms");
    }
    
    return confidence;
}

std::vector<std::pair<double, std::string>> DocumentAnalyzer::analyzeSentenceSimilarity(
    const std::string& content1, const std::string& content2) {
    
    std::vector<std::pair<double, std::string>> results;
    
    auto sentences1 = splitIntoSentences(content1);
    auto sentences2 = splitIntoSentences(content2);
    
    TextProcessor processor;
    
    // Compare each sentence from doc1 with all sentences from doc2
    for (const auto& sent1 : sentences1) {
        if (sent1.length() < 10) continue; // Skip very short sentences
        
        double maxSimilarity = 0.0;
        std::string bestMatch;
        
        auto tf1 = processor.getTermFrequencyMap(sent1);
        
        for (const auto& sent2 : sentences2) {
            if (sent2.length() < 10) continue;
            
            auto tf2 = processor.getTermFrequencyMap(sent2);
            double similarity = SimilarityCalculator::calculateCosineSimilarity(tf1, tf2);
            
            if (similarity > maxSimilarity) {
                maxSimilarity = similarity;
                bestMatch = sent2;
            }
        }
        
        if (maxSimilarity > 0.6) { // Only report significant similarities
            results.emplace_back(maxSimilarity, sent1);
        }
    }
    
    // Sort by similarity score (highest first)
    std::sort(results.begin(), results.end(), 
              [](const auto& a, const auto& b) { return a.first > b.first; });
    
    return results;
}

std::string DocumentAnalyzer::generateAnalysisSummary(
    const DocumentStats& stats1, const DocumentStats& stats2,
    const SimilarityConfidence& confidence) {
    
    std::stringstream summary;
    
    summary << "=== ANALYSIS SUMMARY ===\n\n";
    
    summary << "Document Comparison:\n";
    summary << "Document 1: " << stats1.wordCount << " words, " 
            << stats1.sentenceCount << " sentences\n";
    summary << "Document 2: " << stats2.wordCount << " words, " 
            << stats2.sentenceCount << " sentences\n\n";
    
    summary << "Similarity Assessment:\n";
    summary << "Confidence Level: " << confidence.level << "\n";
    summary << "Overall Score: " << std::fixed << std::setprecision(1) 
            << confidence.score * 100 << "%\n";
    summary << "Interpretation: " << confidence.interpretation << "\n\n";
    
    if (!confidence.indicators.empty()) {
        summary << "Key Indicators:\n";
        for (const auto& indicator : confidence.indicators) {
            summary << "• " << indicator << "\n";
        }
        summary << "\n";
    }
    
    // Document comparison insights
    double sizeDiff = std::abs(static_cast<double>(stats1.wordCount) - stats2.wordCount) 
                     / std::max(stats1.wordCount, stats2.wordCount);
    
    if (sizeDiff < 0.1) {
        summary << "• Documents are similar in length\n";
    } else if (sizeDiff > 0.5) {
        summary << "• Significant difference in document length\n";
    }
    
    double diversityDiff = std::abs(stats1.lexicalDiversity - stats2.lexicalDiversity);
    if (diversityDiff < 0.1) {
        summary << "• Similar vocabulary complexity\n";
    }
    
    return summary.str();
}

std::vector<std::string> DocumentAnalyzer::splitIntoSentences(const std::string& text) {
    std::vector<std::string> sentences;
    std::regex sentenceRegex(R"([.!?]+\s*)");
    std::sregex_token_iterator iter(text.begin(), text.end(), sentenceRegex, -1);
    std::sregex_token_iterator end;
    
    for (; iter != end; ++iter) {
        std::string sentence = iter->str();
        // Trim whitespace
        sentence.erase(0, sentence.find_first_not_of(" \t\n\r"));
        sentence.erase(sentence.find_last_not_of(" \t\n\r") + 1);
        
        if (!sentence.empty() && sentence.length() > 5) {
            sentences.push_back(sentence);
        }
    }
    
    return sentences;
}

std::vector<std::string> DocumentAnalyzer::getTopWords(
    const std::unordered_map<std::string, double>& termFreq, size_t count) {
    
    std::vector<std::pair<std::string, double>> wordPairs;
    for (const auto& [word, freq] : termFreq) {
        wordPairs.emplace_back(word, freq);
    }
    
    std::sort(wordPairs.begin(), wordPairs.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    
    std::vector<std::string> topWords;
    for (size_t i = 0; i < std::min(count, wordPairs.size()); ++i) {
        topWords.push_back(wordPairs[i].first);
    }
    
    return topWords;
}