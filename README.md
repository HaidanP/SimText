# SimText - Text Similarity Checker

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![CMake](https://img.shields.io/badge/CMake-3.10+-green.svg)](https://cmake.org/)

An advanced command-line tool that compares text files and detects plagiarism using multiple similarity algorithms. Features intelligent analysis, confidence scoring, and sentence-level detection for comprehensive academic integrity checking.

## Features

### Algorithms
- **Cosine Similarity**: Traditional vector-space model comparison
- **TF-IDF Cosine Similarity**: Weighted comparison emphasizing rare terms
- **Jaccard Similarity (Character)**: Character-level n-gram comparison
- **Jaccard Similarity (Word)**: Word-level n-gram comparison
- **W-Shingling**: Advanced plagiarism detection technique

### Text Processing
- Stopwords filtering to focus on meaningful content
- Customizable stopwords files for domain-specific filtering
- Case-insensitive text processing with automatic punctuation handling
- Configurable shingle sizes for n-gram analysis

### Analysis & Intelligence
- **Confidence Level Assessment**: Automatic categorization with interpretation guidelines
- **Document Statistics**: Word count, sentence analysis, lexical diversity
- **Sentence-Level Detection**: Identifies paraphrasing and structural similarities
- **Plagiarism Indicators**: Specific flags and recommendations for educators
- **Weighted Scoring**: Intelligent combination of multiple algorithms

### Output & Performance
- Multiple output formats: simple, detailed, JSON
- Performance timing measurements
- Similarity threshold filtering
- Batch processing of multiple files
- Comprehensive analysis summaries
- Optimized C++17 implementation with compiler optimizations

## Quick Start

### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.10 or higher

### Building from Source

```bash
# Clone the repository
git clone https://github.com/HaidanP/SimText.git
cd SimText

# Build the project
mkdir build && cd build
cmake ..
make

# Run with test files
./simtext --ignore-stopwords ../test1.txt ../test2.txt
```

## Usage

### Basic Comparison
```bash
./simtext document1.txt document2.txt
```

### Advanced Algorithm Selection
```bash
# Use TF-IDF for better rare term emphasis
./simtext --algorithm tfidf essay1.txt essay2.txt

# Use Jaccard similarity for structural comparison
./simtext --algorithm jaccard-word --shingle-size 4 doc1.txt doc2.txt

# Compare using all algorithms
./simtext --algorithm all --output detailed paper1.txt paper2.txt
```

### Plagiarism Analysis
```bash
# Comprehensive plagiarism detection with confidence levels
./simtext --algorithm all --analysis essay1.txt essay2.txt

# Detect paraphrasing with sentence-level analysis
./simtext --analysis --sentence-check document1.txt document2.txt

# Professional analysis for educators
./simtext --algorithm all --output detailed --analysis --sentence-check paper1.txt paper2.txt
```

### Output Formats
```bash
# Simple output (default)
./simtext doc1.txt doc2.txt

# Detailed analysis
./simtext --algorithm all --output detailed --timing doc1.txt doc2.txt

# JSON output for integration
./simtext --algorithm all --output json doc1.txt doc2.txt
```

### Batch Processing
```bash
# Compare multiple files (all pairs)
./simtext --threshold 0.5 *.txt

# Filter results above threshold
./simtext --algorithm jaccard-char --threshold 0.8 --output simple essay*.txt
```

### Command Line Options

| Option | Description | Default |
|--------|-------------|---------|
| `--algorithm ALGO` | Algorithm: cosine, tfidf, jaccard-char, jaccard-word, all | cosine |
| `--ignore-stopwords` | Filter out common words | false |
| `--stopwords-file FILE` | Use custom stopwords file | none |
| `--output FORMAT` | Output format: simple, detailed, json | simple |
| `--shingle-size N` | N-gram size for Jaccard similarity | 3 |
| `--threshold N` | Only show results above threshold (0.0-1.0) | 0.0 |
| `--timing` | Show execution times | false |
| `--analysis` | Show detailed plagiarism analysis and confidence levels | false |
| `--sentence-check` | Show sentence-level similarity analysis | false |

## How It Works

### Algorithms Explained

#### 1. Cosine Similarity
Traditional vector-space model approach:
```
similarity = (A · B) / (||A|| × ||B||)
```
Where A and B are term frequency vectors.

#### 2. TF-IDF Cosine Similarity
Emphasizes rare terms by weighting with inverse document frequency:
```
TF-IDF(t,d) = TF(t,d) × log(N/DF(t))
```
More effective for larger document collections.

#### 3. Jaccard Similarity (W-Shingling)
Compares n-gram sets using Jaccard coefficient:
```
Jaccard(A,B) = |A ∩ B| / |A ∪ B|
```
- **Character-level**: Detects character-level plagiarism
- **Word-level**: Identifies structural similarities

### Text Preprocessing Pipeline
1. **Normalization**: Converts to lowercase
2. **Tokenization**: Splits into words/characters
3. **Cleaning**: Removes punctuation and special characters
4. **Filtering**: Optionally removes stopwords
5. **N-gram Generation**: Creates shingles for Jaccard analysis

## Example Output

```bash
$ ./simtext --ignore-stopwords original.txt suspected_copy.txt
Similarity between files:
original.txt and suspected_copy.txt
Similarity score: 87.3%
```

### Interpretation
- 0-30%: Minimal similarity - likely original content
- 30-60%: Moderate similarity - some shared concepts
- 60-80%: High similarity - significant overlap detected
- 80-100%: Very high similarity - potential plagiarism

## Use Cases

- Academic integrity checking
- Document comparison and revision tracking
- Legal document analysis
- Research paper comparison
- Content originality verification

## Project Structure

```
SimText/
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── stopwords.txt          # Default stopwords list
├── include/               # Header files
│   ├── text_processor.hpp
│   └── similarity_calculator.hpp
├── src/                   # Source code
│   ├── main.cpp
│   ├── text_processor.cpp
│   └── similarity_calculator.cpp
└── build/                 # Build directory (generated)
```

## Testing

### Running Unit Tests
```bash
cd build
./test_simtext
```

### Manual Testing
Create test files and compare them:

```bash
# Create sample files
echo "The quick brown fox jumps over the lazy dog." > test1.txt
echo "A quick brown fox is jumping over a lazy dog." > test2.txt

# Basic comparison
./simtext test1.txt test2.txt

# Advanced comparison with all algorithms
./simtext --algorithm all --output detailed --timing test1.txt test2.txt
```

### Expected Output
```
=== Similarity Analysis ===
File 1: test1.txt
File 2: test2.txt

Cosine Similarity:      68.35%
TF-IDF Similarity:      0.00%
Jaccard (Character):    50.00%
Jaccard (Word):         7.50%

=== ANALYSIS SUMMARY ===

Document Comparison:
Document 1: 25 words, 3 sentences
Document 2: 22 words, 3 sentences

Similarity Assessment:
Confidence Level: Low
Overall Score: 43.8%
Interpretation: Low similarity - minimal content overlap

Key Indicators:
• Likely original content
• Normal similarity for same topic

=== HIGH SIMILARITY SENTENCES ===
Similarity: 78.3%
Sentence: "This is a test document for plagiarism detection"
```

## Advanced Configuration

### Custom Stopwords File Format
Create a text file with one stopword per line:
```
the
and
is
are
was
were
```

### Integration with Scripts
```bash
#!/bin/bash
SIMILARITY=$(./simtext --ignore-stopwords "$1" "$2" | grep -o '[0-9.]*%')
echo "Documents are $SIMILARITY similar"
```

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request. For major changes, please open an issue first to discuss what you would like to change.

### Development Setup
1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Support

For questions or issues:
- Open an issue on GitHub
- Check existing issues for solutions
- Review the documentation