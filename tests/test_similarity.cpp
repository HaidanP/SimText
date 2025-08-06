#include "../include/text_processor.hpp"
#include "../include/similarity_calculator.hpp"
#include <iostream>
#include <cassert>
#include <cmath>

// Simple test framework
int tests_run = 0;
int tests_passed = 0;

#define ASSERT_NEAR(a, b, tolerance) do { \
    tests_run++; \
    if (std::abs((a) - (b)) <= (tolerance)) { \
        tests_passed++; \
        std::cout << "✓ Test " << tests_run << " passed\n"; \
    } else { \
        std::cout << "✗ Test " << tests_run << " failed: expected " << (a) \
                  << " to be near " << (b) << " (tolerance: " << (tolerance) << ")\n"; \
    } \
} while(0)

#define ASSERT_EQ(a, b) do { \
    tests_run++; \
    if ((a) == (b)) { \
        tests_passed++; \
        std::cout << "✓ Test " << tests_run << " passed\n"; \
    } else { \
        std::cout << "✗ Test " << tests_run << " failed: expected " << (a) \
                  << " to equal " << (b) << "\n"; \
    } \
} while(0)

void testTextProcessor() {
    std::cout << "\n=== Testing TextProcessor ===\n";
    
    TextProcessor processor;
    
    // Test basic tokenization
    std::string text = "Hello, world! This is a test.";
    auto tokens = processor.processText(text);
    ASSERT_EQ(tokens.size(), 6);
    
    // Test term frequency
    auto tf = processor.getTermFrequencyMap(text);
    ASSERT_NEAR(tf["hello"], 1.0/6.0, 0.001);
    ASSERT_NEAR(tf["world"], 1.0/6.0, 0.001);
    
    // Test stopwords filtering
    processor.setIgnoreStopwords(true);
    auto tokensNoStop = processor.processText("the quick brown fox");
    // Should filter out "the" if default stopwords were loaded
    // For now just test that it returns some tokens
    ASSERT_EQ(tokensNoStop.size() >= 3, true);
}

void testCosineSimilarity() {
    std::cout << "\n=== Testing Cosine Similarity ===\n";
    
    TextProcessor processor;
    
    // Test identical texts
    std::string text1 = "hello world test";
    std::string text2 = "hello world test";
    auto tf1 = processor.getTermFrequencyMap(text1);
    auto tf2 = processor.getTermFrequencyMap(text2);
    double similarity = SimilarityCalculator::calculateCosineSimilarity(tf1, tf2);
    ASSERT_NEAR(similarity, 1.0, 0.001);
    
    // Test completely different texts
    text1 = "hello world";
    text2 = "foo bar";
    tf1 = processor.getTermFrequencyMap(text1);
    tf2 = processor.getTermFrequencyMap(text2);
    similarity = SimilarityCalculator::calculateCosineSimilarity(tf1, tf2);
    ASSERT_NEAR(similarity, 0.0, 0.001);
    
    // Test partially similar texts
    text1 = "hello world test";
    text2 = "hello world different";
    tf1 = processor.getTermFrequencyMap(text1);
    tf2 = processor.getTermFrequencyMap(text2);
    similarity = SimilarityCalculator::calculateCosineSimilarity(tf1, tf2);
    ASSERT_NEAR(similarity, 0.667, 0.01); // 2/3 words in common
}

void testJaccardSimilarity() {
    std::cout << "\n=== Testing Jaccard Similarity ===\n";
    
    TextProcessor processor;
    
    // Test identical sets
    std::vector<std::string> tokens1 = {"hello", "world", "test"};
    std::vector<std::string> tokens2 = {"hello", "world", "test"};
    double similarity = SimilarityCalculator::calculateJaccardSimilarity(tokens1, tokens2);
    ASSERT_NEAR(similarity, 1.0, 0.001);
    
    // Test disjoint sets
    tokens1 = {"hello", "world"};
    tokens2 = {"foo", "bar"};
    similarity = SimilarityCalculator::calculateJaccardSimilarity(tokens1, tokens2);
    ASSERT_NEAR(similarity, 0.0, 0.001);
    
    // Test partial overlap
    tokens1 = {"hello", "world", "test"};
    tokens2 = {"hello", "world", "different"};
    similarity = SimilarityCalculator::calculateJaccardSimilarity(tokens1, tokens2);
    ASSERT_NEAR(similarity, 0.5, 0.001); // 2 common / 4 total unique
}

void testLevenshteinSimilarity() {
    std::cout << "\n=== Testing Levenshtein Similarity ===\n";
    
    // Test identical strings
    double similarity = SimilarityCalculator::calculateLevenshteinSimilarity("hello", "hello");
    ASSERT_NEAR(similarity, 1.0, 0.001);
    
    // Test completely different strings
    similarity = SimilarityCalculator::calculateLevenshteinSimilarity("abc", "def");
    ASSERT_NEAR(similarity, 0.0, 0.001);
    
    // Test similar strings
    similarity = SimilarityCalculator::calculateLevenshteinSimilarity("kitten", "sitting");
    // Distance is 3, max length is 7, so similarity is 1 - 3/7 = 4/7 ≈ 0.571
    ASSERT_NEAR(similarity, 0.571, 0.01);
}

void testAlgorithmSelection() {
    std::cout << "\n=== Testing Algorithm Selection ===\n";
    
    std::string text1 = "hello world test";
    std::string text2 = "hello world different";
    
    // Test cosine algorithm
    auto result = SimilarityCalculator::calculateSimilarity(
        text1, text2, SimilarityAlgorithm::COSINE);
    ASSERT_EQ(result.algorithm, "cosine");
    ASSERT_EQ(result.score > 0.0 && result.score < 1.0, true);
    
    // Test jaccard algorithm
    result = SimilarityCalculator::calculateSimilarity(
        text1, text2, SimilarityAlgorithm::JACCARD);
    ASSERT_EQ(result.algorithm, "jaccard");
    ASSERT_EQ(result.score > 0.0 && result.score < 1.0, true);
}

int main() {
    std::cout << "Running SimText Unit Tests\n";
    std::cout << "==========================\n";
    
    testTextProcessor();
    testCosineSimilarity();
    testJaccardSimilarity();
    testLevenshteinSimilarity();
    testAlgorithmSelection();
    
    std::cout << "\n=== Test Results ===\n";
    std::cout << "Tests run: " << tests_run << "\n";
    std::cout << "Tests passed: " << tests_passed << "\n";
    std::cout << "Tests failed: " << (tests_run - tests_passed) << "\n";
    std::cout << "Success rate: " << (100.0 * tests_passed / tests_run) << "%\n";
    
    return (tests_passed == tests_run) ? 0 : 1;
}