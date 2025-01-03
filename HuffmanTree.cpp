#include "HuffmanTree.hpp"
#include <stdlib.h>
#include <stdexcept>
#include <queue>

namespace HuffmanCoder{

class HuffmanTree::HuffmanTreeImpl{
public:
    HuffmanTreeImpl() = default;
    ~HuffmanTreeImpl() = default;

private:
    struct HuffmanNode{
        HuffmanNode(const std::pair<const uint8_t, uint64_t>& mapIt)
            : data{mapIt.first}, freq{mapIt.second}
        {}

        HuffmanNode(std::shared_ptr<HuffmanNode> newLeft, std::shared_ptr<HuffmanNode> newRight)
            : left{newLeft}, right{newRight}
        {
            if(newLeft == nullptr)
                throw std::runtime_error("Missing Left Node for new Huffman Node Construct");
            if(newRight == nullptr)
                throw std::runtime_error("Missing Right Node for new Huffman Node Construct");
            freq = left->freq + right->freq;
        }

        ~HuffmanNode() = default;

        std::optional<uint8_t> data {};
        uint64_t freq {0};
        std::shared_ptr<HuffmanNode> left {nullptr};
        std::shared_ptr<HuffmanNode> right {nullptr};

        bool operator>(const HuffmanNode& rhs) const{
            return freq > rhs.freq;
        }
        bool operator<(const HuffmanNode& rhs) const{
            return freq < rhs.freq;
        }
        bool operator==(const HuffmanNode& rhs) const{
            return freq == rhs.freq;
        }
    };

    using HuffmanNodePtr = std::shared_ptr<HuffmanNode>;

    struct HuffmanNodeComparator {
        bool operator()(const HuffmanNodePtr& lhs, const HuffmanNodePtr& rhs) const {
            return (*lhs) > (*rhs);
        }
    };

    HuffmanNodePtr m_rootNode = nullptr;
    HuffmanNodePtr m_searchNode = nullptr;
    std::unordered_map<uint8_t, std::vector<bool>> m_tranlateTable;
    std::vector<std::pair<uint8_t, uint64_t>> m_dataToFreqVec;

    void FillTable(HuffmanNodePtr currNode, std::vector<bool> currCode){
        if(currNode->data.has_value()){
            m_tranlateTable.emplace(currNode->data.value(), currCode);
            return;
        }
        currCode.emplace_back(false);
        FillTable(currNode->left, currCode);
        currCode.back() = true;
        FillTable(currNode->right, currCode);
    }

public:
    bool FormTheTree(const std::vector<std::pair<uint8_t, uint64_t>>& dataToFreqVec){
        if(IsTreeFormed())
            return false;

        m_dataToFreqVec = dataToFreqVec;

        std::priority_queue<HuffmanNodePtr, std::vector<HuffmanNodePtr>,
            HuffmanNodeComparator> t_sortedByte;
        
        for(const auto& it: m_dataToFreqVec)
            t_sortedByte.emplace(std::make_shared<HuffmanNode>(it));
        
        while(t_sortedByte.size() > 1){
            auto firstNode = t_sortedByte.top();
            t_sortedByte.pop();
            auto secondNode = t_sortedByte.top();
            t_sortedByte.pop();
            t_sortedByte.emplace(std::make_shared<HuffmanNode>(firstNode, secondNode));
        }

        if(t_sortedByte.empty()){
            return false;
        }

        m_rootNode = t_sortedByte.top();

        if(m_rootNode->data.has_value())
            m_tranlateTable.emplace(m_rootNode->data.value(), std::vector{false});
        else
            FillTable(m_rootNode, {});

        ResetSearch();
        return true;
    }

    bool IsTreeFormed() const{
        return m_rootNode != nullptr;
    }

    std::vector<uint8_t> GenerateFileHeader(){
        if(not IsTreeFormed())
            return {};
        std::vector<uint8_t> t_generatedHeader {'R', 'e', 'x'};
        t_generatedHeader.push_back(m_dataToFreqVec.size());
        for(const auto& it: m_dataToFreqVec){
            t_generatedHeader.push_back(it.first);
            t_generatedHeader.push_back(it.second);
            t_generatedHeader.push_back(it.second >> 8);
            t_generatedHeader.push_back(it.second >> 16);
            t_generatedHeader.push_back(it.second >> 24);
            t_generatedHeader.push_back(it.second >> 32);
            t_generatedHeader.push_back(it.second >> 40);
            t_generatedHeader.push_back(it.second >> 48);
            t_generatedHeader.push_back(it.second >> 56);
        }
        return t_generatedHeader;
    }

    std::vector<bool> GetEncodeedByte(const uint8_t& aByte) const{
        if(not IsTreeFormed())
            return {};
        auto tableIt = m_tranlateTable.find(aByte);
        if(tableIt == m_tranlateTable.end())
            return {};
        return tableIt->second;
    }

    std::optional<uint8_t> SearchForDecodedByte(bool aBit){
        if(not IsTreeFormed())
            return {};
        
        // Handle Single Node Tree
        if(m_searchNode->data.has_value())
            return m_searchNode->data;

        auto nextNode = aBit ? m_searchNode->right : m_searchNode->left;
        if(nextNode == nullptr)
            throw std::runtime_error("Bad Search Happends");
        if(nextNode->data.has_value()){
            ResetSearch();
            return nextNode->data;
        }else{
            m_searchNode = nextNode;
            return {};
        }
    }

    bool ResetSearch(){
        if(not IsTreeFormed())
            return false;
        m_searchNode = m_rootNode;
        return true;
    }
};

HuffmanTree::HuffmanTree(){
    mImpl = std::make_unique<HuffmanTreeImpl>();
    if(mImpl == nullptr)
        throw std::runtime_error("Unable to create Huffman Tree Object");
}

HuffmanTree::~HuffmanTree(){
    mImpl.reset();
}

bool HuffmanTree::FormTheTree(const std::vector<std::pair<uint8_t, uint64_t>>& dataToFreqVec){
    return mImpl->FormTheTree(dataToFreqVec);
}
bool HuffmanTree::IsTreeFormed() const{
    return mImpl->IsTreeFormed();
}
std::vector<bool> HuffmanTree::GetEncodeedByte(const uint8_t& aByte) const{
    return mImpl->GetEncodeedByte(aByte);
}
std::vector<uint8_t> HuffmanTree::GenerateFileHeader(){
    return mImpl->GenerateFileHeader();
}
std::optional<uint8_t> HuffmanTree::SearchForDecodedByte(bool aBit){
    return mImpl->SearchForDecodedByte(aBit);
}
bool HuffmanTree::ResetSearch(){
    return mImpl->ResetSearch();
}



}
