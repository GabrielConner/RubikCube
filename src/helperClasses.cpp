#include "helperClasses.h"
#include <fstream>


// TODO when this is at 1GiB it misses solution and continues to depth 7
// At 5MiB file is only about 2.45GiB
constexpr size_t nodeWindowSize = 5242880ui64;
constexpr size_t indexWindowSize = 5242880ui64;



constexpr size_t nodeViewSize = nodeWindowSize / sizeof(RubikNode);
constexpr size_t indexTreeSize = indexWindowSize / sizeof(size_t);

constexpr size_t nodeViewSizeBytes = nodeViewSize * sizeof(RubikNode);
constexpr size_t indexTreeSizeBytes = indexTreeSize * sizeof(size_t);

constexpr const char* rubikNodeSwapFileDir = "./rubikNodeView.sfrn";
constexpr const char* indexTreeSwapFileDir = "./rubikIndexView.sfi";


static_assert(nodeWindowSize >= sizeof(RubikNode));
static_assert(indexWindowSize >= sizeof(size_t));




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



RubikNodeList::RubikNodeList() : nodeBegin(nullptr), nodeNext(nullptr), nodeEnd(nullptr), indexBegin(nullptr), indexNext(nullptr),
indexEnd(nullptr), nodeNumberNext(0), nodeBlock(nullptr), indexBlock(nullptr), loadedNodeBlock(0), loadedIndexBlock(0), indexBlockCount(0), nodeBlockCount(0),
nodeFile(rubikNodeSwapFileDir, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc),
indexFile(indexTreeSwapFileDir, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc)
{}




size_t RubikNodeList::IndexOf(uint8_t* pos) const {
  return (pos - (uint8_t*)nodeBegin) / sizeof(RubikNode);
}


size_t RubikNodeList::GetNext(RubikNode set) {
  if (!indexBegin) {
    indexBegin = (size_t*)calloc(indexTreeSize, sizeof(size_t));
    indexEnd = indexBegin + indexTreeSize;
    indexNext = indexBegin;

    if (!indexFile.is_open())
      indexFile.open(indexTreeSwapFileDir, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
  }

  if (!nodeBegin) {
    nodeBegin = (RubikNode*)calloc(nodeViewSize, sizeof(RubikNode));

    nodeEnd = nodeBegin + nodeViewSize;
    nodeNext = nodeBegin;
    nodeNext++;

    if (!nodeFile.is_open())
      nodeFile.open(rubikNodeSwapFileDir, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);

    *nodeBegin = set;
    return nodeNumberNext++;
  }


  if (nodeNext == nodeEnd) {
    nodeFile.seekp(0, std::ios::end);
    nodeFile.write((char*)nodeBegin, nodeViewSizeBytes);
    nodeFile.flush();
    nodeFile.clear(std::ios::goodbit);

    if (indexNext == indexEnd) {
      indexFile.seekp(0, std::ios::end);
      indexFile.write((char*)indexBegin, indexTreeSizeBytes);
      indexFile.flush();
      indexFile.clear(std::ios::goodbit);

      indexBlockCount = 0;
      indexNext = indexBegin;
    }

    nodeNext = nodeBegin;

    *indexNext = nodeBlockCount * nodeViewSize;

    nodeBlockCount++;
    indexBlockCount++;
    indexNext++;
  }

  *nodeNext = set;

  nodeNext++;

  return nodeNumberNext++;
}


RubikNode* RubikNodeList::Find(size_t index) {
  if (!indexBlock)
    indexBlock = (size_t*)calloc(indexTreeSize, sizeof(size_t));
  if (!nodeBlock)
    nodeBlock = (RubikNode*)calloc(nodeViewSize, sizeof(RubikNode));

  if (index >= nodeBlockCount * nodeViewSize) {
    RubikNode* t = nodeBegin + (index - nodeBlockCount * nodeViewSize);
    return t;
  }


  size_t* searchIndex = searchNearest(indexBegin, index, indexBlockCount);
  if (searchIndex != nullptr && index <= *searchIndex + nodeViewSize) {

    if (loadedNodeBlock != *searchIndex) {
      nodeFile.seekg((*searchIndex) * sizeof(RubikNode));
      nodeFile.read((char*)nodeBlock, nodeViewSizeBytes);
      nodeFile.clear(std::ios::goodbit);

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

    if (searchIndex == nullptr || index >= *searchIndex + nodeViewSize) {
      indexFile.read((char*)indexBlock, indexTreeSizeBytes);
      loadedIndexBlock++;

      continue;
    }


    if (loadedNodeBlock != *searchIndex) {
      nodeFile.seekg((*searchIndex) * sizeof(RubikNode));
      nodeFile.read((char*)nodeBlock, nodeViewSizeBytes);
      nodeFile.clear(std::ios::goodbit);
      loadedNodeBlock = *searchIndex;
    }
    return nodeBlock + (index - (*searchIndex) * nodeViewSize);
  } while (!nodeFile.eof());
  indexFile.clear(std::ios::goodbit);

  return nullptr;
}



void RubikNodeList::Clear() {
  free(nodeBegin);
  free(indexBegin);
  free(nodeBlock);
  free(indexBlock);
  nodeNumberNext = 0;
  nodeBlockCount = 0;
  indexBlockCount = 0;
  nodeBegin = nullptr;
  indexBegin = nullptr;
  nodeBlock = nullptr;
  indexBlock = nullptr;

  nodeFile.flush();
  nodeFile.close();
  indexFile.flush();
  indexFile.close();
}

size_t RubikNodeList::Size() const {
  return nodeNumberNext;
}