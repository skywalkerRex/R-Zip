#include "HuffmanCoder.hpp"
#include "HuffmanTree.hpp"

#include <cstring>

namespace HuffmanCoder{
    std::optional<std::string> HuffmanEncoder(const std::vector<uint8_t>& srcData, std::vector<uint8_t> *resultData){
        if(resultData == nullptr)
            return {"No result data pointer provided"};
        resultData->clear();

        std::unordered_map<uint8_t, uint64_t> t_byteToFreqMap;
        for(const auto& srcIt: srcData){
            if(t_byteToFreqMap.find(srcIt) == t_byteToFreqMap.end())
                t_byteToFreqMap.emplace(srcIt, 0);
            t_byteToFreqMap[srcIt]++;
        }
        std::vector<std::pair<uint8_t, uint64_t>> t_byteToFreqVec;
        for(auto it: t_byteToFreqMap) t_byteToFreqVec.push_back(it);
        HuffmanTree treeForEncode;
        if(not treeForEncode.FormTheTree(t_byteToFreqVec)){
            return {"Unknow to build the Huffman Tree"};
        }

        auto fileHeader = treeForEncode.GenerateFileHeader();
        resultData->swap(fileHeader);

        std::vector<bool> tEncodedBits;
        for(const auto& srcIt: srcData){
            auto encodeBits = treeForEncode.GetEncodeedByte(srcIt);
            tEncodedBits.insert(tEncodedBits.end(), encodeBits.begin(), encodeBits.end());
        }
        int debugSize = tEncodedBits.size();
        resultData->push_back(tEncodedBits.size()%8);
        auto preInsertSize = resultData->size();
        resultData->resize(preInsertSize + (tEncodedBits.size()+7)/8);
        for (size_t i = 0; i < tEncodedBits.size(); ++i){
            int modByteLoc = preInsertSize + (i / 8);
            auto debugByte = resultData->at(modByteLoc) ;
            if (tEncodedBits[i])
                resultData->at(modByteLoc) |= (1 << (i % 8));
            else
                resultData->at(modByteLoc) &= ~(1 << (i % 8));
        }
        return {};
    }

    std::optional<std::string> HuffmanDecoder(const std::vector<uint8_t>& srcData, std::vector<uint8_t> *resultData){
        if(resultData == nullptr)
            return {"No result data pointer provided"};
        auto inFileSize = srcData.size();
        if(inFileSize < 4)
            return "File too small to be rzip file";

        std::vector<std::pair<uint8_t, uint64_t>> t_byteToFreqVec;
        const std::vector<uint8_t> fixHead{'R', 'e', 'x'};
        for(int i=0; i<3; i++){
            if(fixHead[i] != srcData[i])
                return "Header mismatch, Not a rzip file";
        }

        uint8_t mapSize = srcData.at(3);

        if(inFileSize < (4 + (9*mapSize) + 1))
            return "Incorrect File Format, Huffman Tree Map Unable To Recover";

        for(int i=0; i<mapSize; i++){
            uint64_t newEntrySize = 0;
            std::memcpy(&newEntrySize, srcData.data()+(5+9*i), sizeof(uint64_t));
            t_byteToFreqVec.emplace_back(srcData.at(4+9*i), newEntrySize);
        }

        HuffmanTree treeToDecode;
        if(not treeToDecode.FormTheTree(t_byteToFreqVec)){
            printf("Unknow to rebuild the Huffman Tree\n");
            return {"Unknow to rebuild the Huffman Tree"};
        }

        auto inputIt = srcData.begin() + 4 + (9*mapSize) + 1;

        for(; inputIt != srcData.end(); inputIt++){
            for(int i=0; i<8; i++){
                auto currBit = (*inputIt) & (1 << i);
                auto searchData = treeToDecode.SearchForDecodedByte(currBit);
                if(searchData.has_value())
                    resultData->push_back(searchData.value());
            }
        }

        return {};
    }

}