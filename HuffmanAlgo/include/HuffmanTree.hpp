#pragma once

#include <unordered_map>
#include <optional>
#include <cstdint>
#include <vector>
#include <memory>

namespace HuffmanCoder{

class HuffmanTree{

public:
    HuffmanTree();
    ~HuffmanTree();

    bool FormTheTree(const std::vector<std::pair<uint8_t, uint64_t>>& dataToFreqMap);
    bool IsTreeFormed() const;

    std::vector<uint8_t> GenerateFileHeader();

    std::vector<bool> GetEncodeedByte(const uint8_t& aByte) const;

    std::optional<uint8_t> SearchForDecodedByte(bool aBit);
    bool ResetSearch();

private:
    class HuffmanTreeImpl;
    std::unique_ptr<HuffmanTreeImpl> mImpl;
};

}