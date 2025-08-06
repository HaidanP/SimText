#include "../include/text_processor.hpp"
#include "../include/similarity_calculator.hpp"
#include <cassert>
#include <iostream>
#include <cmath>
#include <fstream>
#include <algorithm>

void test_text_processing() {
    TextProcessor processor;
    
    // Test basic tokenization
    std::string text = "The Quick Brown Fox! Jumps over the lazy dog.";
    auto tokens = processor.processText(text);
    
    assert(!tokens.empty());
    assert(tokens[0] == "the");
    assert(tokens[1] == "quick");
    
    std::cout << "✓ Text processing test passed\n";
}

void test_stopwords_filtering() {
    TextProcessor processor;
    processor.setIgnoreStopwords(true);
    
    // Create temporary stopwords
    std::ofstream stopwords_file("temp_stopwords.txt");
    stopwords_file << "the\nand\nis\n";
    stopwords_file.close();
    
    processor.loadStopwords("temp_stopwords.txt");
    
    std::string text = "the cat and dog is running";
    auto tokens = processor.processText(text);
    
    // Should only contain "cat", "dog", "running"
    assert(tokens.size() == 3);
    assert(std::find(tokens.begin(), tokens.end(), "the") == tokens.end());
    
    // Cleanup
    std::remove("temp_stopwords.txt");
    
    std::cout << "✓ Stopwords filtering test passed\n";
}

void test_cosine_similarity() {
    std::unordered_map<std::string, double> tf1 = {{"cat", 0.5}, {"dog", 0.5}};
    std::unordered_map<std::string, double> tf2 = {{"cat", 0.5}, {"dog", 0.5}};
    
    double similarity = SimilarityCalculator::calculateCosineSimilarity(tf1, tf2);
    assert(std::abs(similarity - 1.0) < 0.001); // Should be 1.0 (identical)
    
    std::unordered_map<std::string, double> tf3 = {{"bird", 1.0}};
    similarity = SimilarityCalculator::calculateCosineSimilarity(tf1, tf3);
    assert(std::abs(similarity - 0.0) < 0.001); // Should be 0.0 (no overlap)
    
    std::cout << "✓ Cosine similarity test passed\n";
}

void test_term_frequency() {
    TextProcessor processor;
    std::string text = "cat cat dog";
    auto tf = processor.getTermFrequencyMap(text);
    
    assert(std::abs(tf["cat"] - 2.0/3.0) < 0.001);
    assert(std::abs(tf["dog"] - 1.0/3.0) < 0.001);
    
    std::cout << "✓ Term frequency test passed\n";
}

int main() {
    std::cout << "Running SimText tests...\n\n";
    
    try {
        test_text_processing();
        test_stopwords_filtering();
        test_cosine_similarity();
        test_term_frequency();
        
        std::cout << "\n✅ All tests passed!\n";
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "❌ Test failed: " << e.what() << "\n";
        return 1;
    }
}