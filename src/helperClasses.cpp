#include "helperClasses.h"
#include <fstream>


// TODO when this is at 1GiB it misses solution and continues to depth 7
// At 5MiB file is only about 2.45GiB
constexpr size_t nodeUsageMaxSize = 1073741824ui64;
constexpr size_t nodeViewMaxSize = 131072ui64;



constexpr size_t nodeUsageCount = nodeUsageMaxSize / sizeof(RubikNode);
constexpr size_t nodeViewCount = nodeViewMaxSize / sizeof(RubikNode);


constexpr size_t nodeUsageBytes = nodeUsageCount * sizeof(RubikNode);
constexpr size_t nodeViewSizeBytes = nodeViewCount * sizeof(RubikNode);



constexpr const char* rubikNodeSwapFileDir = "./rubikNodeView.sfrn";
constexpr const char* indexTreeSwapFileDir = "./rubikIndexView.sfi";


static_assert(nodeUsageMaxSize >= sizeof(RubikNode));
static_assert(nodeViewMaxSize >= sizeof(RubikNode));




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



RubikNodeList::RubikNodeList() : nodeBegin(nullptr), nodeNext(nullptr), nodeEnd(nullptr),
nodeNumberNext(0), nodeBlock(nullptr), loadedNodeBlock(-1), nodeBlockCount(0),
nodeFile(rubikNodeSwapFileDir, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc)
{}




size_t RubikNodeList::IndexOf(uint8_t* pos) const {
  return (pos - (uint8_t*)nodeBegin) / sizeof(RubikNode);
}


size_t RubikNodeList::GetNext(RubikNode set) {
  //if (!indexBegin) {
  //  indexBegin = (size_t*)calloc(indexTreeSize, sizeof(size_t));
  //  indexEnd = indexBegin + indexTreeSize;
  //  indexNext = indexBegin;

  //  if (!indexFile.is_open())
  //    indexFile.open(indexTreeSwapFileDir, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
  //}

  if (!nodeBegin) {
    nodeBegin = (RubikNode*)calloc(nodeUsageCount, sizeof(RubikNode));

    nodeEnd = nodeBegin + nodeUsageCount;
    nodeNext = nodeBegin;
    nodeNext++;

    *nodeBegin = set;
    return nodeNumberNext++;
  }


  if (nodeNext == nodeEnd) {
    nodeFile.seekp(nodeBlockCount * nodeUsageBytes);
    nodeFile.write((char*)nodeBegin, nodeUsageBytes);
    nodeFile.flush();
    nodeFile.clear(std::ios::goodbit);

    //if (indexNext == indexEnd) {
    //  indexFile.seekp(0, std::ios::end);
    //  indexFile.write((char*)indexBegin, indexTreeSizeBytes);
    //  indexFile.flush();
    //  indexFile.clear(std::ios::goodbit);

    //  indexBlockCount = 0;
    //  indexNext = indexBegin;
    //}

    nodeNext = nodeBegin;

    //*indexNext = nodeBlockCount * nodeUsageCount;

    nodeBlockCount++;
    //indexBlockCount++;
    //indexNext++;
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
  nodeFile.seekg(loadedNodeBlock * sizeof(RubikNode));
  nodeFile.read((char*)nodeBlock, nodeViewSizeBytes);
  return nodeBlock + (index - loadedNodeBlock);




  /*if (index >= nodeBlockCount * nodeUsageCount) {
    RubikNode* t = nodeBegin + (index - nodeBlockCount * nodeUsageCount);
    return t;
  }


  size_t* searchIndex = searchNearest(indexBegin, index, indexBlockCount);
  if (searchIndex != nullptr && index <= *searchIndex + nodeUsageCount) {

    if (loadedNodeBlock != *searchIndex) {
      nodeFile.seekg((*searchIndex) * sizeof(RubikNode));
      nodeFile.read((char*)nodeBlock, nodeUsageBytes);
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

    if (searchIndex == nullptr || index >= *searchIndex + nodeUsageCount) {
      indexFile.read((char*)indexBlock, indexTreeSizeBytes);
      loadedIndexBlock++;

      continue;
    }


    if (loadedNodeBlock != *searchIndex) {
      nodeFile.seekg((*searchIndex) * sizeof(RubikNode));
      nodeFile.read((char*)nodeBlock, nodeUsageBytes);
      nodeFile.clear(std::ios::goodbit);
      loadedNodeBlock = *searchIndex;
    }
    return nodeBlock + (index - (*searchIndex) * nodeUsageCount);
  } while (!nodeFile.eof());
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