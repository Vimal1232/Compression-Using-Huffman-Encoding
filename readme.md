# Compression in C++ Using Huffman Coding

This repository contains a C++ implementation of Huffman coding, a popular algorithm for lossless data compression. The code is designed to read a String, compress its contents using Huffman coding.

## Features

- **Compression**: Efficiently compress Strings using Huffman coding.
- **Binary Representation**: Store compressed data in a binary format for efficiency.
- **Shannon entropy**: As Close a Shannon entropy Limit as possible for the compressed data.
- **Decompression**: Implement functionality to decompress String back to their original form.

## Upcoming Features

- **File Handling**: Support for reading and writing files.
- **Images Support**: Extend the algorithm to handle image files for compression.

## Example

String = "One day, a mighty lion was sleeping in the jungle when a little mouse accidentally ran across his paw. The lion woke up and caught the mouse, ready to eat it. The mouse pleaded, “Please, let me go, and someday I’ll repay your kindness.” The lion laughed but decided to let the mouse go. A few days later, the lion got caught in a hunter’s net. The mouse heard his roar, ran to him, and chewed through the ropes to free him. The lion was grateful and realized even small creatures can make a big difference."

Results:

- **The String Size in bytes**: 514
- **The Compressed Size in Bytes**: 281
- **The Compression Ratio**: 1.82918
- **Compressed By**: 45.3307%
- **Decompressed String**:One day, a mighty lion was sleeping in the jungle when a little mouse accidentally ran across his paw. The lion woke up and caught the mouse, ready to eat it. The mouse pleaded, “Please, let me go, and someday I’ll repay your kindness.” The lion laughed but decided to let the mouse go. A few days later, the lion got caught in a hunter’s net. The mouse heard his roar, ran to him, and chewed through the ropes to free him. The lion was grateful and realized even small creatures can make a big difference.
