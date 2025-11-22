#ifndef RUBIK_CUBE_SRC_HELPER_CLASSES_H
#define RUBIK_CUBE_SRC_HELPER_CLASSES_H
#include <ostream>
#include <fstream>
#include <pPack/vector.h>

#define ReverseDirection(i) i == 2 ? 2 : (i == 0 ? 1 : 0)


struct Vertex {
  pPack::vector::Vector3 position;
  unsigned int faceIndex;
  uint8_t dir;
};


struct StillCubeCorner {
  uint8_t f1;
  uint8_t f2;
  uint8_t f3;



  StillCubeCorner(uint8_t F1, uint8_t F2, uint8_t F3) : f1(F1), f2(F2), f3(F3) {}
};

struct CubeCorner {
  uint8_t* f1;
  uint8_t* f2;
  uint8_t* f3;

  uint8_t index;


  uint8_t shiftFrom(const StillCubeCorner& corn) {
    if (*f2 == corn.f2)
      return 1;
    if (*f1 == corn.f2)
      return 3;
    return 2;
  }

  CubeCorner(uint8_t* F1, uint8_t* F2, uint8_t* F3, uint8_t Index) : f1(F1), f2(F2), f3(F3), index(Index) {}
};




struct HeuristicPair {
  uint8_t data;

  uint8_t First() {
    return data & 0x0F;
  }

  uint8_t Second() {
    return (data & 0xF0) >> 4;
  }

  uint8_t Get(size_t index) {
    return (index & 1) == 0 ? First() : Second();
  }

  void Set(size_t index, uint8_t val) {
    if ((index & 1) == 0)
      data = (data & 0xF0) | val;
    else
      data = (data & 0x0F) | (val << 4);
  }

  void SetIfLess(size_t index, uint8_t val) {
    if ((index & 1) == 0) {
      if (First() > val)
        data = (data & 0xF0) | val;
    } else if (Second() > val) {
      data = (data & 0x0F) | (val << 4);
    }
  }



  HeuristicPair() : data(0xff) {}
  HeuristicPair(uint8_t Data) : data(Data) {}
};





struct RubikActionSingle {
  uint8_t face;
  uint8_t direction;
  unsigned short layer;

  bool operator==(const RubikActionSingle& other) const {
    return face == other.face && direction == other.direction && layer == other.layer;
  }

  uint8_t ReversedDirection() const {
    uint8_t dir = direction;
    if (dir == 0)
      dir = 1;
    else if (dir == 1)
      dir = 0;
    return dir;
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
  size_t prevNode;

  unsigned short layer;
  uint8_t data;
  unsigned short distance;


  bool operator ==(const RubikActionSingle& other) const {
    return DecomposeAction() == other;
  }


  RubikActionSingle DecomposeAction() const {
    return RubikActionSingle(Face(), Direction(), layer);
  }

  uint8_t Face() const {
    return data & 7ui8;
  }

  uint8_t Direction() const {
    return (data & 24ui8) >> 3;
  }

  uint8_t ReversedDirection() const {
    uint8_t dir = Direction();
    dir = ReverseDirection(dir);
    return dir;
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
    return 't';
  }


  void operator =(const RubikNodeCreate& node) {
    distance = node.distance;
    layer = node.layer;
    data = node.data;
  }

  RubikNode() : next(0), prevNode(0), layer(0), data(0), distance(0) {}
  RubikNode(size_t Next, size_t PrevAction, const RubikNodeCreate& node) : next(Next), prevNode(PrevAction), layer(node.layer), data(node.data), distance(node.distance) {};
};

struct RubikNodeBlock {
  RubikNode* curBlock;
  RubikNode* end;
  RubikNode* node;
  RubikNodeBlock* next;
  RubikNodeBlock* prevNode;

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

  RubikNodeBlock(RubikNode* Cur, size_t size) : curBlock(Cur), end(Cur + size), node(Cur), next(nullptr), prevNode(nullptr) {}
  ~RubikNodeBlock() {
    free(curBlock);
  }
};

struct RubikNodeList {
  RubikNode* nodeBegin;
  RubikNode* nodeNext;
  RubikNode* nodeEnd;

  size_t nodeNumberNext;
  size_t nodeBlockCount;

  RubikNode* nodeBlock;

  size_t loadedNodeBlock;

  std::string swapFileDir;

  std::fstream swapFile;





  size_t IndexOf(uint8_t* pos) const;

  RubikNode* Find(size_t pos);

  size_t GetNext(RubikNode set);

  RubikNode* GetNextNode(RubikNode set);

  void Clear();

  size_t Size() const;

  RubikNodeList(std::string SwapFileDir);

  ~RubikNodeList() {
    swapFile.flush();
    swapFile.close();
    free(nodeBegin);
    free(nodeBlock);
  }
};




struct RubikStack {
  size_t topNode;
  bool empty;

public:
  RubikNodeList list;



  size_t Push(RubikNodeCreate node, size_t PrevAction) {
    if (empty) {
      topNode = list.GetNext({size_t(-1), PrevAction, node});
      empty = false;
      return topNode;
    }


    topNode = list.GetNext({topNode, PrevAction, node});
    return topNode;
  }


  RubikNode* Pop() {
    if (empty)
      return nullptr;

    RubikNode* node = list.Find(topNode);

    topNode = node->next;
    if (topNode == -1)
      empty = true;

    return node;
  }

  RubikNode* Peek() {
    return list.Find(topNode);
  }

  bool Empty() {
    return empty;
  }


  RubikStack(std::string SwapFileDir) : list(SwapFileDir), topNode(0), empty(true) {}
};


class RubikQueue {
  RubikNode* end;
  size_t front;

public: 
  RubikNodeList list;


  void Push(RubikNodeCreate node, size_t PrevAction) {
    if (end == nullptr) {
      front = list.GetNext(RubikNode(size_t(-1), PrevAction, node));
      end = list.Find(front);
      return;
    }

    end->next = list.nodeNumberNext;
    end = list.Find(list.GetNext(RubikNode(size_t(-1), PrevAction, node)));
  }


  std::pair<RubikNode*, size_t> Pop() {
    if (end == nullptr)
      return {nullptr, -1};

    RubikNode* node = list.Find(front);

    size_t t = front;
    front = node->next;
    if (front == -1)
      end = nullptr;

    return {node, t};
  }

  RubikNode* Peek() {
    return list.Find(front);
  }

  bool Empty() {
    return end == nullptr;
  }


  RubikQueue(std::string SwapFileDir) : list(SwapFileDir), front(-1), end(nullptr) {}
};



std::ostream& operator<<(std::ostream& str, const RubikActionSingle& action);

#endif