#include <iostream>
#include <fstream>
#include <cstring>
#include <chrono>
#include "HuffmanCoder.hpp"

int main(int argc, char** argv){

    // Check if the correct number of arguments is provided
    if(argc < 3){
        printf("Error Command Formart\n");
        printf("Example: rzip <input source inFile name> <output compassed inFile name>\n");
        printf("Example: rzip -d <input compassed inFile name> <output decompassed inFile name>\n");
        return -1;
    }

    bool isCompress = true;
    char** inFileName{&argv[1]}, **outFileName{&argv[2]};
    // Check if the operation is decompression
    if(strncmp(argv[1], "-d", 2) == 0){
        inFileName++;
        outFileName++;
        isCompress = false;
    }
    // Open input and output files
    std::ifstream inFile(*inFileName, std::ios::binary | std::ios::ate);
    std::ofstream outFile(*outFileName, std::ios::out);
    if (!inFile.is_open()) {
        printf("Unable to Open Input File %s\n", *inFileName);
        return -1;
    }
    if (!outFile.is_open()) {
        printf("Unable to Open Output File %s\n", *outFileName);
        return -1;
    }

    // Read the input file into a buffer
    uint64_t inFileSize = inFile.tellg();
    std::vector<uint8_t> inBuf(inFileSize), outBuf;
    inFile.seekg(0);
    inFile.read(reinterpret_cast<char*>(inBuf.data()), inFileSize);
    inFile.close();

    // Measure the time taken for compression/decompression
    auto start = std::chrono::high_resolution_clock::now();
    std::optional<std::string> res;
    if(isCompress)
        res = HuffmanCoder::HuffmanEncoder(inBuf, &outBuf);
    else
        res = HuffmanCoder::HuffmanDecoder(inBuf, &outBuf);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Check for errors in the compression/decompression process
    if(res.has_value()){
        std::cout << res.value() << std::endl;
        return 1;
    }
    // Write the output buffer to the output file
    outFile.write(reinterpret_cast<const char*>(outBuf.data()), outBuf.size());
    outFile.close();

    uint64_t outFileSize = outBuf.size();

    // Print success messages and file sizes
    if(isCompress){
        printf("File Compress Success\n");
        printf("Original Size:   %ld Bytes\n", inFileSize);
        printf("Compressed Size: %ld Bytes\n", outFileSize);
        printf("Compressed Rate: %.2f%%\n", double(outFileSize)/double(inFileSize)*100);
    }else{
        printf("File Decompress Success\n");
        printf("Original Size:   %ld Bytes\n", inFileSize);
        printf("Decompressed Size: %ld Bytes\n", outFileSize);
    }

    // Calculate and print the elapsed time in minutes, seconds, and milliseconds
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(elapsed);
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed) % 60;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed) % 1000;

    printf("Time Elapsed: %02ldm:%02lds.%03ldms\n", minutes.count(), seconds.count(), milliseconds.count());

    return 0;
}