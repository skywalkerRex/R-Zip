#pragma once

#include <unordered_map>
#include <optional>
#include <cstdint>
#include <vector>
#include <memory>

namespace HuffmanCoder{

/**
 * @class HuffmanTree
 * @brief A class representing a Huffman Tree used for encoding and decoding data.
 */
class HuffmanTree{

public:
    /**
     * @brief Constructor for HuffmanTree.
     */
    HuffmanTree();

    /**
     * @brief Destructor for HuffmanTree.
     */
    ~HuffmanTree();

    /**
     * @brief Forms the Huffman Tree based on the provided data-to-frequency map.
     * @param dataToFreqMap A vector of pairs containing data bytes and their frequencies.
     * @return True if the tree was successfully formed, false otherwise.
     */
    bool FormTheTree(const std::vector<std::pair<uint8_t, uint64_t>>& dataToFreqMap);

    /**
     * @brief Checks if the Huffman Tree has been formed.
     * @return True if the tree is formed, false otherwise.
     */
    bool IsTreeFormed() const;

    /**
     * @brief Generates the file header for the encoded data.
     * @return A vector of bytes representing the file header.
     */
    std::vector<uint8_t> GenerateFileHeader();

    /**
     * @brief Gets the encoded bits for a given byte.
     * @param aByte The byte to be encoded.
     * @return A vector of booleans representing the encoded bits.
     */
    std::vector<bool> GetEncodeedByte(const uint8_t& aByte) const;

    /**
     * @brief Searches for the decoded byte based on the provided bit.
     * @param aBit The bit to be used for searching.
     * @return An optional byte if the search is successful, std::nullopt otherwise.
     */
    std::optional<uint8_t> SearchForDecodedByte(bool aBit);

    /**
     * @brief Resets the search state of the Huffman Tree.
     * @return True if the reset was successful, false otherwise.
     */
    bool ResetSearch();

private:
    class HuffmanTreeImpl;
    std::unique_ptr<HuffmanTreeImpl> mImpl;
};

}