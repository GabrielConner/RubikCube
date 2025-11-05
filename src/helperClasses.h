#pragma once
#include <ostream>
#include <fstream>
#include <pPack/vector.h>

struct Vertex {
  pPack::vector::Vector3 position;
  unsigned int faceIndex;
  uint8_t dir;
};



struct RubikActionSingle {
  unsigned char face;
  unsigned char direction;
  unsigned short layer;

  RubikActionSingle Reversed() {
    unsigned char dir = direction;
    if (dir == 'c')
      dir = 'a';
    else if (dir == 'a')
      dir = 'c';
    return {face, dir, layer};
  }
};





struct RubikNodeCreate {
  unsigned short distance;
  unsigned short layer;
  uint8_t data;

  RubikNodeCreate(unsigned short Distance, uint8_t Face, uint8_t Direction, bool Valid, unsigned short Layer) : distance(Distance), layer(Layer),
    data(Face | (Direction << 3) | ((Valid ? 1 : 0) << 5)) {}
};




struct RubikNode {
  size_t next;
  size_t prevAction;

  unsigned short layer;
  uint8_t data;
  unsigned short distance;

  RubikActionSingle DecomposeAction() {
    return RubikActionSingle(NormFace(), NormDirection(), layer);
  }

  uint8_t Face() const {
    return data & 7ui8;
  }

  uint8_t Direction() const {
    return (data & 24ui8) >> 3;
  }

  bool ValidAction() const {
    return (data & 32ui8);
  }


  uint8_t NormFace() const {
    uint8_t face = Face();
    switch (face) {
    case 0:
      return 'b';
    case 1:
      return 'l';
    case 2:
      return 'd';
    case 3:
      return 'r';
    case 4:
      return 'f';
    default:
      return 'u';
    }
  }

  uint8_t NormDirection() const {
    uint8_t dir = Direction();
    if (dir == 0)
      return 'c';
    if (dir == 1)
      return 'a';
    if (dir == 2)
      return 't';
  }


  void operator =(const RubikNodeCreate& node) {
    distance = node.distance;
    layer = node.layer;
    data = node.data;
  }

  RubikNode() : next(0), prevAction(0), layer(0), data(0), distance(0) {}
  RubikNode(size_t Next, size_t PrevAction, const RubikNodeCreate& node) : next(Next), prevAction(PrevAction), layer(node.layer), data(node.data), distance(node.distance) {};
};


struct RubikNodeBlock {
  RubikNode* curBlock;
  RubikNode* end;
  RubikNode* node;
  RubikNodeBlock* next;
  RubikNodeBlock* prevAction;

  RubikNode* GetNext() {
    if (node != end) {
      RubikNode* temp = node;
      node++;
      return temp;
    }
    return nullptr;
  }

  void Reset() {
    node = curBlock;
  }

  RubikNodeBlock(RubikNode* Cur, size_t size) : curBlock(Cur), end(Cur + size), node(Cur), next(nullptr), prevAction(nullptr) {}
  ~RubikNodeBlock() {
    free(curBlock);
  }
};

struct RubikNodeList {
  RubikNode* nodeBegin;
  RubikNode* nodeNext;
  RubikNode* nodeEnd;

  size_t* indexBegin;
  size_t* indexNext;
  size_t* indexEnd;

  size_t nodeNumberNext;
  size_t nodeBlockCount;
  size_t indexBlockCount;

  RubikNode* nodeBlock;
  size_t* indexBlock;

  size_t loadedNodeBlock;
  size_t loadedIndexBlock;


  std::fstream nodeFile;
  std::fstream indexFile;


  size_t IndexOf(uint8_t* pos) const;

  RubikNode* Find(size_t pos);

  size_t GetNext(RubikNode set);

  void Clear();

  size_t Size() const;

  RubikNodeList();

  ~RubikNodeList() {
    nodeFile.flush();
    indexFile.flush();
    nodeFile.close();
    indexFile.close();
    free(nodeBegin);
    free(indexBegin);
    free(nodeBlock);
    free(indexBlock);
  }
};




struct RubikStack {
  size_t topNode;
  bool empty;

  size_t Push(RubikNodeCreate node, size_t PrevAction, RubikNodeList& list) {
    if (empty) {
      topNode = list.GetNext({size_t(-1), PrevAction, node});
      empty = false;
      return topNode;
    }


    topNode = list.GetNext({topNode, PrevAction, node});
    return topNode;
  }


  RubikNode* Pop(RubikNodeList& list) {
    if (empty)
      return nullptr;

    RubikNode* node = list.Find(topNode);

    topNode = node->next;
    if (topNode == -1)
      empty = true;

    return node;
  }

  RubikNode* Peek(RubikNodeList& list) const {
    return list.Find(topNode);
  }


  RubikStack() : topNode(0), empty(true) {}
};



std::ostream& operator<<(std::ostream& str, const RubikActionSingle& action);