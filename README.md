# 🔍 SimText - Advanced Text Similarity Checker

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![CMake](https://img.shields.io/badge/CMake-3.10+-green.svg)](https://cmake.org/)

SimText is a high-performance command-line tool that compares text files and calculates content similarity using cosine similarity algorithms. Perfect for plagiarism detection, document comparison, and academic integrity checking.

## ✨ Features

- 🧮 **Cosine Similarity Algorithm**: Mathematical precision for accurate text comparison
- 🚫 **Smart Stopwords Filtering**: Ignore common words to focus on meaningful content
- 🔧 **Customizable Stopwords**: Use your own stopwords file for domain-specific filtering
- 🔤 **Intelligent Text Processing**: Case-insensitive with automatic punctuation handling
- ⚡ **Fast Performance**: Optimized C++ implementation for quick results
- 📊 **Clear Output**: Easy-to-understand percentage-based similarity scores

## 🚀 Quick Start

### Prerequisites

- **C++17** compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- **CMake 3.10** or higher

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

## 📖 Usage

### Basic Comparison
```bash
./simtext document1.txt document2.txt
```

### With Stopwords Filtering (Recommended)
```bash
./simtext --ignore-stopwords essay1.txt essay2.txt
```

### Custom Stopwords File
```bash
./simtext --ignore-stopwords --stopwords-file custom_stopwords.txt paper1.txt paper2.txt
```

### Command Line Options

| Option | Description |
|--------|-------------|
| `--ignore-stopwords` | Filter out common words (the, and, is, etc.) |
| `--stopwords-file FILE` | Use custom stopwords from specified file |

## 🔬 How It Works

### 1. Text Preprocessing
- **Normalization**: Converts all text to lowercase
- **Tokenization**: Splits text into individual words
- **Cleaning**: Removes punctuation and special characters
- **Filtering**: Optionally removes stopwords for better accuracy

### 2. Mathematical Analysis
- **Term Frequency Calculation**: Computes frequency of each word
- **Vector Space Model**: Represents documents as high-dimensional vectors
- **Cosine Similarity**: Calculates angle between document vectors
- **Percentage Output**: Converts similarity to intuitive 0-100% scale

### 3. Algorithm Formula
```
similarity = (A · B) / (||A|| × ||B||)
```
Where A and B are term frequency vectors of the two documents.

## 📊 Example Results

```bash
$ ./simtext --ignore-stopwords original.txt suspected_copy.txt
Similarity between files:
original.txt and suspected_copy.txt
Similarity score: 87.3%
```

### Interpretation Guide
- **0-30%**: Minimal similarity - likely original content
- **30-60%**: Moderate similarity - some shared concepts
- **60-80%**: High similarity - significant overlap detected
- **80-100%**: Very high similarity - potential plagiarism

## 🎯 Use Cases

- **Academic Integrity**: Check student submissions for plagiarism
- **Content Review**: Compare drafts and revisions
- **Legal Documents**: Analyze contract similarities
- **Research**: Compare scientific papers and publications
- **Journalism**: Verify originality of articles

## 📁 Project Structure

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

## 🧪 Testing

Create test files and compare them:

```bash
# Create sample files
echo "The quick brown fox jumps over the lazy dog." > test1.txt
echo "A quick brown fox is jumping over a lazy dog." > test2.txt

# Run comparison
./simtext --ignore-stopwords test1.txt test2.txt
```

## 🛠️ Advanced Configuration

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

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request. For major changes, please open an issue first to discuss what you would like to change.

### Development Setup
1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Cosine similarity algorithm implementation
- Modern C++ best practices
- CMake build system integration

## 📞 Support

If you have any questions or need help, please:
- Open an issue on GitHub
- Check existing issues for solutions
- Review the documentation

---

**Made with ❤️ by developers, for developers**