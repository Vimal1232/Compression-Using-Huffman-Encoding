# Compression in C++ Using Huffman Coding

This repository contains a C++ implementation of Huffman coding, a popular algorithm for lossless data compression. The code is designed to read Text Files, compress its contents using Huffman coding.

## Features

- **Compression**: Efficiently compress Text Files using Huffman coding.
- **Binary Representation**: Store compressed data in a binary format for efficiency.
- **Shannon entropy**: As Close a Shannon entropy Limit as possible for the compressed data.
- **Decompression**: Implement functionality to decompress Text Files back to their original form.
- **File Handling**:
  - Read input from a file.
  - Write compressed data to a binary file.
  - Read compressed data from a binary file and decompress it.

## Upcoming Features

- **Images Support**: Extend the algorithm to handle image files for compression.

### 📦 Huffman Compression Results (Large Text File)

- **Refer to the Test.txt file for Data That Was Used**
- **Original File Size**: 3.3 MB
- **Compressed File Size**: 1.8 MB
- **Compressed By**: 43.06%
- **Shannon Entropy**: 4.302 bits per character
- **Average Bits Per Character**: 4.55 bits per character

### 📈 Summary Comparison

| Metric                     | Huffman       | Gzip          |
|---------------------------|---------------|---------------|
| File Size (Compressed)    | 1.8 MB        | 1.2 MB        |
| Compression Ratio         | 1.76          | 2.75          |
| Compressed By             | 43.06%        | 64%           |
| Decompression Accuracy    | ✅ Yes        | ✅ Yes        |

---




