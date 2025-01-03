#include <iostream>
#include <fstream>
#include <cstring>
#include "HuffmanCoder.hpp"

int main(int argc, char** argv){

    if(argc < 3){
        printf("Error Command Formart\n");
        printf("Example: rzip <input source inFile name> <output compassed inFile name>\n");
        printf("Example: rzip -d <input compassed inFile name> <output decompassed inFile name>\n");
        return -1;
    }

    bool isCompress = true;
    char** inFileName{&argv[1]}, **outFileName{&argv[2]};
    if(strncmp(argv[1], "-d", 2) == 0){
        inFileName++;
        outFileName++;
        isCompress = false;
    }
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

    uint64_t inFileSize = inFile.tellg();
    std::vector<uint8_t> inBuf(inFileSize), outBuf;
    inFile.seekg(0);
    inFile.read(reinterpret_cast<char*>(inBuf.data()), inFileSize);
    inFile.close();

    std::optional<std::string> res;
    if(isCompress)
        res = HuffmanCoder::HuffmanEncoder(inBuf, &outBuf);
    else
        res = HuffmanCoder::HuffmanDecoder(inBuf, &outBuf);

    if(res.has_value()){
        std::cout << res.value() << std::endl;
        return 1;
    }
    outFile.write(reinterpret_cast<const char*>(outBuf.data()), outBuf.size());
    outFile.close();

    uint64_t outFileSize = outBuf.size();

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

    return 0;
}