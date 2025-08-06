#include "text_processor.hpp"
#include "similarity_calculator.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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
    std::cout << "Usage: simtext [options] <file1> <file2>\n"
              << "Options:\n"
              << "  --ignore-stopwords    Ignore common stopwords\n"
              << "  --stopwords-file FILE Use custom stopwords file\n"
              << "Example:\n"
              << "  simtext --ignore-stopwords text1.txt text2.txt\n";
}

int main(int argc, char* argv[]) {
    std::vector<std::string> args(argv + 1, argv + argc);
    
    if (args.size() < 2) {
        printUsage();
        return 1;
    }
    
    TextProcessor processor;
    bool ignoreStopwords = false;
    std::string stopwordsFile;
    std::vector<std::string> files;
    
    // Parse command line arguments
    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "--ignore-stopwords") {
            ignoreStopwords = true;
        }
        else if (args[i] == "--stopwords-file" && i + 1 < args.size()) {
            stopwordsFile = args[++i];
        }
        else {
            files.push_back(args[i]);
        }
    }
    
    if (files.size() < 2) {
        std::cerr << "Error: Please provide at least two files to compare\n";
        printUsage();
        return 1;
    }
    
    try {
        // Configure text processor
        processor.setIgnoreStopwords(ignoreStopwords);
        if (!stopwordsFile.empty()) {
            processor.loadStopwords(stopwordsFile);
        }
        
        // Read and process files
        std::string content1 = readFile(files[0]);
        std::string content2 = readFile(files[1]);
        
        // Calculate term frequency maps
        auto tf1 = processor.getTermFrequencyMap(content1);
        auto tf2 = processor.getTermFrequencyMap(content2);
        
        // Calculate similarity
        double similarity = SimilarityCalculator::calculateCosineSimilarity(tf1, tf2);
        
        // Output results
        std::cout << "Similarity between files:\n"
                  << files[0] << " and " << files[1] << "\n"
                  << "Similarity score: " << similarity * 100 << "%\n";
        
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}