#include "helperClasses.hpp"
#include <fstream>


constexpr size_t nodeUsageMaxSize = 4294967296ui64;
constexpr size_t nodeViewMaxSize = 131072ui64;



constexpr size_t nodeUsageCount = nodeUsageMaxSize / sizeof(RubikNode);
constexpr size_t nodeViewCount = nodeViewMaxSize / sizeof(RubikNode);


constexpr size_t nodeUsageBytes = nodeUsageCount * sizeof(RubikNode);
constexpr size_t nodeViewSizeBytes = nodeViewCount * sizeof(RubikNode);


static_assert(nodeUsageMaxSize >= sizeof(RubikNode));
static_assert(nodeViewMaxSize >= sizeof(RubikNode));


bool fileExists(const char* file) {
  struct _stat buffer;
  if (_stat(file, &buffer) == 0) return true;
  return false;
}



std::ostream& operator<<(std::ostream& str, const RubikActionSingle& action) {
  str << action.face << " " << action.direction << " " << action.layer;
  return str;
}

template<typename T>
concept equal_constraint = requires(T a, T b) {
  a == b;
};



template<typename T> requires equal_constraint<T>
T* searchNearest(T* buf, T val, size_t length) {
  size_t p = -1;
  for (size_t i = 0; i < length; i++) {
    if (buf[i] == val)
      return &buf[i];
    if (buf[i] > val) {
      if (p != -1 && buf[p] < val)
        return &buf[p];
      return nullptr;
    }
    p++;
  }
  if (length > 0)
    return &buf[length - 1];
  return nullptr;
}



RubikNodeList::RubikNodeList(std::string SwapFileDir) : nodeBegin(nullptr), nodeNext(nullptr), nodeEnd(nullptr),
nodeNumberNext(0), nodeBlock(nullptr), loadedNodeBlock(-1), nodeBlockCount(0), swapFileDir(SwapFileDir),
swapFile()
{}




size_t RubikNodeList::IndexOf(uint8_t* pos) const {
  return (pos - (uint8_t*)nodeBegin) / sizeof(RubikNode);
}



size_t RubikNodeList::GetNext(RubikNode set) {
  if (!nodeBegin) {
    nodeBegin = (RubikNode*)calloc(nodeUsageCount, sizeof(RubikNode));

    nodeEnd = nodeBegin + nodeUsageCount;
    nodeNext = nodeBegin;
    nodeNext++;

    *nodeBegin = set;
    return nodeNumberNext++;
  }


  if (nodeNext == nodeEnd) {

    if (!swapFile.is_open()) {
      if (!fileExists(swapFileDir.c_str())) {
        swapFile.open(swapFileDir, std::ios::binary | std::ios::out);
        swapFile.close();
      }

      swapFile.open(swapFileDir, std::ios::binary | std::ios::out | std::ios::in);
    }

    swapFile.seekp(nodeBlockCount * nodeUsageBytes);
    swapFile.write((char*)nodeBegin, nodeUsageBytes);
    swapFile.flush();

    nodeNext = nodeBegin;

    nodeBlockCount++;
  }

  *nodeNext = set;

  nodeNext++;

  return nodeNumberNext++;
}



RubikNode* RubikNodeList::Find(size_t index) {
  //if (!indexBlock)
  //  indexBlock = (size_t*)calloc(indexTreeSize, sizeof(size_t));

  if (!nodeBegin) {
    std::cout << "No node data around\n";
    return nullptr;
  }

  if (index >= nodeBlockCount * nodeUsageCount)
    return nodeBegin + (index - nodeBlockCount * nodeUsageCount);



  if (!nodeBlock)
    nodeBlock = (RubikNode*)calloc(nodeViewCount, sizeof(RubikNode));


  if (index >= nodeNumberNext) {
    std::cout << "Search for " << index << " failed\n";
    return nullptr;
  }

  if (index >= loadedNodeBlock && index < loadedNodeBlock + nodeViewCount)
    return nodeBlock + (index - loadedNodeBlock);

  loadedNodeBlock = index - std::min<size_t>(index, nodeViewCount - 1);

  swapFile.seekg(loadedNodeBlock * sizeof(RubikNode));
  swapFile.read((char*)nodeBlock, nodeViewSizeBytes);
  return nodeBlock + (index - loadedNodeBlock);




  /*if (index >= nodeBlockCount * nodeUsageCount) {
    RubikNode* t = nodeBegin + (index - nodeBlockCount * nodeUsageCount);
    return t;
  }


  size_t* searchIndex = searchNearest(indexBegin, index, indexBlockCount);
  if (searchIndex != nullptr && index <= *searchIndex + nodeUsageCount) {

    if (loadedNodeBlock != *searchIndex) {
      swapFile.seekg((*searchIndex) * sizeof(RubikNode));
      swapFile.read((char*)nodeBlock, nodeUsageBytes);
      swapFile.clear(std::ios::goodbit);

      loadedNodeBlock = *searchIndex;
    }

    return nodeBlock + (index - *searchIndex);
  }

  if (loadedIndexBlock != *indexBlock) {
    indexFile.seekg(0, std::ios::beg);
    indexFile.read((char*)indexBlock, indexTreeSizeBytes);
    indexFile.clear(std::ios::goodbit);
    loadedIndexBlock = *indexBlock;
  }

  if (indexFile.gcount() == 0) {
    return nullptr;
  }

  do {
    searchIndex = searchNearest(indexBlock, index, indexFile.gcount() / sizeof(size_t));

    if (searchIndex == nullptr || index >= *searchIndex + nodeUsageCount) {
      indexFile.read((char*)indexBlock, indexTreeSizeBytes);
      loadedIndexBlock++;

      continue;
    }


    if (loadedNodeBlock != *searchIndex) {
      swapFile.seekg((*searchIndex) * sizeof(RubikNode));
      swapFile.read((char*)nodeBlock, nodeUsageBytes);
      swapFile.clear(std::ios::goodbit);
      loadedNodeBlock = *searchIndex;
    }
    return nodeBlock + (index - (*searchIndex) * nodeUsageCount);
  } while (!swapFile.eof());
  indexFile.clear(std::ios::goodbit);

  return nullptr;*/
}



void RubikNodeList::Clear() {
  loadedNodeBlock = -1;
  nodeNumberNext = 0;
  nodeBlockCount = 0;
  nodeNext = nodeBegin;
}

size_t RubikNodeList::Size() const {
  return nodeNumberNext;
}