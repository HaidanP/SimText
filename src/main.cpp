#include "text_processor.hpp"
#include "similarity_calculator.hpp"
#include "shingling.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <chrono>

enum class Algorithm {
    COSINE,
    TFIDF,
    JACCARD_CHAR,
    JACCARD_WORD,
    ALL
};

enum class OutputFormat {
    SIMPLE,
    DETAILED,
    JSON
};

struct Config {
    bool ignoreStopwords = false;
    std::string stopwordsFile;
    Algorithm algorithm = Algorithm::COSINE;
    OutputFormat outputFormat = OutputFormat::SIMPLE;
    int shingleSize = 3;
    bool showTimings = false;
    double threshold = 0.0;
    std::vector<std::string> files;
};

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    return content;
}

void printUsage() {
    std::cout << "SimText - Advanced Text Similarity Checker\n\n"
              << "Usage: simtext [options] <file1> <file2> [file3...]\n\n"
              << "Options:\n"
              << "  --algorithm ALGO        Algorithm to use: cosine, tfidf, jaccard-char, jaccard-word, all (default: cosine)\n"
              << "  --ignore-stopwords      Ignore common stopwords\n"
              << "  --stopwords-file FILE   Use custom stopwords file\n"
              << "  --output FORMAT         Output format: simple, detailed, json (default: simple)\n"
              << "  --shingle-size N        Size of shingles for Jaccard similarity (default: 3)\n"
              << "  --threshold N           Only show results above threshold (0.0-1.0)\n"
              << "  --timing                Show execution times\n"
              << "  --help, -h              Show this help message\n\n"
              << "Examples:\n"
              << "  simtext doc1.txt doc2.txt\n"
              << "  simtext --algorithm all --output detailed doc1.txt doc2.txt\n"
              << "  simtext --algorithm jaccard-word --shingle-size 4 --ignore-stopwords *.txt\n";
}

Config parseArguments(const std::vector<std::string>& args) {
    Config config;
    
    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "--help" || args[i] == "-h") {
            printUsage();
            exit(0);
        }
        else if (args[i] == "--ignore-stopwords") {
            config.ignoreStopwords = true;
        }
        else if (args[i] == "--stopwords-file" && i + 1 < args.size()) {
            config.stopwordsFile = args[++i];
        }
        else if (args[i] == "--algorithm" && i + 1 < args.size()) {
            std::string algo = args[++i];
            if (algo == "cosine") config.algorithm = Algorithm::COSINE;
            else if (algo == "tfidf") config.algorithm = Algorithm::TFIDF;
            else if (algo == "jaccard-char") config.algorithm = Algorithm::JACCARD_CHAR;
            else if (algo == "jaccard-word") config.algorithm = Algorithm::JACCARD_WORD;
            else if (algo == "all") config.algorithm = Algorithm::ALL;
            else {
                std::cerr << "Unknown algorithm: " << algo << "\n";
                exit(1);
            }
        }
        else if (args[i] == "--output" && i + 1 < args.size()) {
            std::string format = args[++i];
            if (format == "simple") config.outputFormat = OutputFormat::SIMPLE;
            else if (format == "detailed") config.outputFormat = OutputFormat::DETAILED;
            else if (format == "json") config.outputFormat = OutputFormat::JSON;
            else {
                std::cerr << "Unknown output format: " << format << "\n";
                exit(1);
            }
        }
        else if (args[i] == "--shingle-size" && i + 1 < args.size()) {
            config.shingleSize = std::stoi(args[++i]);
        }
        else if (args[i] == "--threshold" && i + 1 < args.size()) {
            config.threshold = std::stod(args[++i]);
        }
        else if (args[i] == "--timing") {
            config.showTimings = true;
        }
        else if (args[i][0] != '-') {
            config.files.push_back(args[i]);
        }
    }
    
    return config;
}

struct SimilarityResult {
    double cosine = 0.0;
    double tfidf = 0.0;
    double jaccardChar = 0.0;
    double jaccardWord = 0.0;
    double duration = 0.0;
};

SimilarityResult calculateSimilarity(const std::string& file1, const std::string& file2, 
                                   const Config& config, TextProcessor& processor) {
    auto start = std::chrono::high_resolution_clock::now();
    
    SimilarityResult result;
    
    // Read files
    std::string content1 = readFile(file1);
    std::string content2 = readFile(file2);
    
    // Calculate TF maps
    auto tf1 = processor.getTermFrequencyMap(content1);
    auto tf2 = processor.getTermFrequencyMap(content2);
    
    // Cosine similarity
    if (config.algorithm == Algorithm::COSINE || config.algorithm == Algorithm::ALL) {
        result.cosine = SimilarityCalculator::calculateCosineSimilarity(tf1, tf2);
    }
    
    // TF-IDF similarity
    if (config.algorithm == Algorithm::TFIDF || config.algorithm == Algorithm::ALL) {
        std::vector<std::unordered_map<std::string, double>> docs = {tf1, tf2};
        auto idf = SimilarityCalculator::calculateIdf(docs);
        result.tfidf = SimilarityCalculator::calculateTfIdfCosineSimilarity(tf1, tf2, idf);
    }
    
    // Jaccard similarities
    if (config.algorithm == Algorithm::JACCARD_CHAR || config.algorithm == Algorithm::ALL) {
        auto shingles1 = ShinglingCalculator::generateCharacterShingles(content1, config.shingleSize);
        auto shingles2 = ShinglingCalculator::generateCharacterShingles(content2, config.shingleSize);
        result.jaccardChar = ShinglingCalculator::calculateJaccardSimilarity(shingles1, shingles2);
    }
    
    if (config.algorithm == Algorithm::JACCARD_WORD || config.algorithm == Algorithm::ALL) {
        auto tokens1 = processor.processText(content1);
        auto tokens2 = processor.processText(content2);
        auto shingles1 = ShinglingCalculator::generateWordShingles(tokens1, config.shingleSize);
        auto shingles2 = ShinglingCalculator::generateWordShingles(tokens2, config.shingleSize);
        result.jaccardWord = ShinglingCalculator::calculateJaccardSimilarity(shingles1, shingles2);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    result.duration = std::chrono::duration<double, std::milli>(end - start).count();
    
    return result;
}

void outputResults(const std::string& file1, const std::string& file2, 
                  const SimilarityResult& result, const Config& config) {
    
    // Check threshold
    double maxSimilarity = std::max({result.cosine, result.tfidf, result.jaccardChar, result.jaccardWord});
    if (maxSimilarity < config.threshold) {
        return;
    }
    
    if (config.outputFormat == OutputFormat::JSON) {
        std::cout << "{\n"
                  << "  \"file1\": \"" << file1 << "\",\n"
                  << "  \"file2\": \"" << file2 << "\",\n"
                  << "  \"similarity\": {\n";
        
        if (config.algorithm == Algorithm::COSINE || config.algorithm == Algorithm::ALL) {
            std::cout << "    \"cosine\": " << std::fixed << std::setprecision(4) << result.cosine << ",\n";
        }
        if (config.algorithm == Algorithm::TFIDF || config.algorithm == Algorithm::ALL) {
            std::cout << "    \"tfidf\": " << std::fixed << std::setprecision(4) << result.tfidf << ",\n";
        }
        if (config.algorithm == Algorithm::JACCARD_CHAR || config.algorithm == Algorithm::ALL) {
            std::cout << "    \"jaccard_char\": " << std::fixed << std::setprecision(4) << result.jaccardChar << ",\n";
        }
        if (config.algorithm == Algorithm::JACCARD_WORD || config.algorithm == Algorithm::ALL) {
            std::cout << "    \"jaccard_word\": " << std::fixed << std::setprecision(4) << result.jaccardWord << ",\n";
        }
        
        std::cout << "  },\n";
        if (config.showTimings) {
            std::cout << "  \"duration_ms\": " << std::fixed << std::setprecision(2) << result.duration << "\n";
        }
        std::cout << "}\n";
        
    } else if (config.outputFormat == OutputFormat::DETAILED) {
        std::cout << "=== Similarity Analysis ===\n"
                  << "File 1: " << file1 << "\n"
                  << "File 2: " << file2 << "\n\n";
        
        if (config.algorithm == Algorithm::COSINE || config.algorithm == Algorithm::ALL) {
            std::cout << "Cosine Similarity:      " << std::fixed << std::setprecision(2) << result.cosine * 100 << "%\n";
        }
        if (config.algorithm == Algorithm::TFIDF || config.algorithm == Algorithm::ALL) {
            std::cout << "TF-IDF Similarity:      " << std::fixed << std::setprecision(2) << result.tfidf * 100 << "%\n";
        }
        if (config.algorithm == Algorithm::JACCARD_CHAR || config.algorithm == Algorithm::ALL) {
            std::cout << "Jaccard (Character):    " << std::fixed << std::setprecision(2) << result.jaccardChar * 100 << "%\n";
        }
        if (config.algorithm == Algorithm::JACCARD_WORD || config.algorithm == Algorithm::ALL) {
            std::cout << "Jaccard (Word):         " << std::fixed << std::setprecision(2) << result.jaccardWord * 100 << "%\n";
        }
        
        if (config.showTimings) {
            std::cout << "Processing time:        " << std::fixed << std::setprecision(2) << result.duration << " ms\n";
        }
        std::cout << "\n";
        
    } else { // SIMPLE
        double similarity = 0.0;
        std::string algorithmName;
        
        switch (config.algorithm) {
            case Algorithm::COSINE:
                similarity = result.cosine;
                algorithmName = "Cosine";
                break;
            case Algorithm::TFIDF:
                similarity = result.tfidf;
                algorithmName = "TF-IDF";
                break;
            case Algorithm::JACCARD_CHAR:
                similarity = result.jaccardChar;
                algorithmName = "Jaccard (Character)";
                break;
            case Algorithm::JACCARD_WORD:
                similarity = result.jaccardWord;
                algorithmName = "Jaccard (Word)";
                break;
            default:
                similarity = result.cosine;
                algorithmName = "Cosine";
        }
        
        std::cout << file1 << " vs " << file2 << ": " 
                  << std::fixed << std::setprecision(1) << similarity * 100 << "%";
        if (config.showTimings) {
            std::cout << " (" << std::fixed << std::setprecision(1) << result.duration << "ms)";
        }
        std::cout << "\n";
    }
}

int main(int argc, char* argv[]) {
    std::vector<std::string> args(argv + 1, argv + argc);
    
    if (args.empty()) {
        printUsage();
        return 1;
    }
    
    Config config = parseArguments(args);
    
    if (config.files.size() < 2) {
        std::cerr << "Error: Please provide at least two files to compare\n";
        printUsage();
        return 1;
    }
    
    try {
        // Configure text processor
        TextProcessor processor;
        processor.setIgnoreStopwords(config.ignoreStopwords);
        if (!config.stopwordsFile.empty()) {
            processor.loadStopwords(config.stopwordsFile);
        }
        
        // Compare all pairs of files
        for (size_t i = 0; i < config.files.size(); ++i) {
            for (size_t j = i + 1; j < config.files.size(); ++j) {
                auto result = calculateSimilarity(config.files[i], config.files[j], config, processor);
                outputResults(config.files[i], config.files[j], result, config);
            }
        }
        
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}