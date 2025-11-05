#ifndef RUBIK_CUBE_SRC_RUBIK_CUBE_H
#define RUBIK_CUBE_SRC_RUBIK_CUBE_H

#include <cstdint>
#include <string>
#include "helperClasses.h"
#include <map>
#include <deque>
#include <vector>



class RubikCube {
public:

  struct CubeCorner {
    uint8_t* f1;
    uint8_t* f2;
    uint8_t* f3;

    size_t Score() {
      return (*f1) + (2 * (*f2)) + (3 * (*f3));
    }
  };

  struct CubeEdge {
    uint8_t* f1;
    uint8_t* f2;

    size_t Score() {
      return (*f1) + (2 * (*f2));
    }
  };



  unsigned short size;
  unsigned int faceSize;
  unsigned int cubeSize;
  unsigned int edgeCount;
  unsigned int edgeFirstBoundry;

  uint8_t* cubiePlanes;

  uint8_t* front;
  uint8_t* back;

  uint8_t* left;
  uint8_t* right;

  uint8_t* down;
  uint8_t* up;

  CubeCorner corners[8];
  CubeEdge* edges;

  short centerLine;


  size_t CornerIndex() {
    return corners[0].Score() + (2 * corners[1].Score()) + (3 * corners[2].Score()) + (4 * corners[3].Score()) + (5 * corners[4].Score()) + (6 * corners[5].Score()) + (7 * corners[6].Score()) + (8 * corners[7].Score());
  }

  size_t FirstEdgeIndex() {
    size_t ret = 0;
    for (unsigned int i = 0; i < edgeFirstBoundry; i++) {
      ret += edges[i].Score() * (i + 1);
    }
    return ret;
  }
  size_t SecondEdgeIndex() {
    size_t ret = 0;
    for (unsigned int i = edgeFirstBoundry; i < edgeCount; i++) {
      ret += edges[i].Score() * (i + 1);
    }
    return ret;
  }

  void Rotate(unsigned char face, unsigned char dir, unsigned short layer);

  std::vector<RubikActionSingle> Randomize(unsigned int steps, unsigned int seed);

  std::string AsString();

  uint8_t* Copy();

  std::deque<RubikActionSingle> Solve();

  unsigned int MaxScoreOf(unsigned short distance, std::map<size_t, uint8_t>& corners, std::map<size_t, uint8_t>& firstEdges, std::map<size_t, uint8_t>& secondEdges);

  void Reset();


  bool operator==(RubikCube& cube);


  RubikCube(unsigned short Size, uint8_t* planes = nullptr) : size(Size), faceSize(Size * Size), cubeSize(6 * Size * Size),
    edgeCount(0), edgeFirstBoundry(0),
    cubiePlanes(nullptr), front(nullptr), back(nullptr), left(nullptr), right(nullptr), down(nullptr),
    up(nullptr), edges(nullptr), centerLine(-1) {

    if (Size == 0)
      return;

    if (Size % 2)
      centerLine = Size / 2;

    cubiePlanes = new uint8_t[cubeSize];
    if (planes != nullptr)
      memcpy(cubiePlanes, planes, cubeSize);


    back = cubiePlanes;
    left = back + faceSize;
    down = left + faceSize;
    right = down + faceSize;
    front = right + faceSize;
    up = front + faceSize;


    unsigned int re = size - 1;
    unsigned int ce = faceSize - size - 1;
    unsigned int ae = faceSize - 1;


    corners[0] = CubeCorner(left + re, up + ce, front);
    corners[1] = CubeCorner(up + ae, right, front + re);
    corners[2] = CubeCorner(right + ae, front + ce, down);
    corners[3] = CubeCorner(front + ae, right + ae, down + re);
    corners[4] = CubeCorner(up, left, back);
    corners[5] = CubeCorner(right + re, up + re, back + re);
    corners[6] = CubeCorner(back + ce, left + ce, down + ce);
    corners[7] = CubeCorner(right + ae, back + ae, down + ae);


    if (size > 2) {
      edgeCount = 12 * (size - 2) * (size - 2);
      edges = new CubeEdge[edgeCount];
      edgeFirstBoundry = edgeCount / 2;

      unsigned int ei = 0;
      for (unsigned int i = 1; i <= size - 2; i++) {
        unsigned int ri = i * size;

        edges[ei]     = CubeEdge(front + i, up + ce + i);
        edges[ei + 1] = CubeEdge(front + re + ri, right + ri);
        edges[ei + 2] = CubeEdge(front + ce + i, down + i);
        edges[ei + 3] = CubeEdge(front + ri, left + re + ri);

        edges[ei + 4] = CubeEdge(left + i, up + ri);
        edges[ei + 5] = CubeEdge(left + ce + i, down + ce - ri);
        edges[ei + 6] = CubeEdge(left + ri, back + ri);

        edges[ei + 7] = CubeEdge(right + i, up + ae - ri);
        edges[ei + 8] = CubeEdge(right + ce + i, down + re + ri);
        edges[ei + 9] = CubeEdge(right + re + ri, back + ce + ri);

        edges[ei + 10] = CubeEdge(back + i, up + i);
        edges[ei + 11] = CubeEdge(back + ce + i, down + ce + i);

        ei += 12;
      }
    }



    if (planes == nullptr) {
      for (int i = 0; i < faceSize; i++) {
        front[i] = 4;
        back[i] = 0;
        left[i] = 1;
        right[i] = 3;
        down[i] = 2;
        up[i] = 5;
      }
    }
  }

  ~RubikCube() {
    if (cubiePlanes)
      delete[](cubiePlanes);
    if (edges)
      delete[](edges);
  }
};

#endif