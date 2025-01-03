#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <optional>

namespace HuffmanCoder{

/**
 * @brief Encodes the given source data using Huffman coding.
 * @param srcData The source data to be encoded.
 * @param resultData Pointer to the vector where the encoded data will be stored.
 * @return An optional string containing an error message if the encoding fails, or std::nullopt on success.
 */
std::optional<std::string> HuffmanEncoder(const std::vector<uint8_t>& srcData, std::vector<uint8_t> *resultData);

/**
 * @brief Decodes the given source data using Huffman coding.
 * @param srcData The source data to be decoded.
 * @param resultData Pointer to the vector where the decoded data will be stored.
 * @return An optional string containing an error message if the decoding fails, or std::nullopt on success.
 */
std::optional<std::string> HuffmanDecoder(const std::vector<uint8_t>& srcData, std::vector<uint8_t> *resultData);

}