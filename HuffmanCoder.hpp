#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <optional>

namespace HuffmanCoder{
    std::optional<std::string> HuffmanEncoder(const std::vector<uint8_t>& srcData, std::vector<uint8_t> *resultData);
    std::optional<std::string> HuffmanDecoder(const std::vector<uint8_t>& srcData, std::vector<uint8_t> *resultData);
}