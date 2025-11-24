#ifndef RUBIK_CUBE_SRC_EDGE_CUBE_H
#define RUBIK_CUBE_SRC_EDGE_CUBE_H

#include <cstdint>
#include "helperClasses.hpp"



class EdgeCube {
public:
  uint8_t* cubiePlanes;

  CubeEdge* cubieEdges;


  uint8_t* front;
  uint8_t* back;

  uint8_t* left;
  uint8_t* right;

  uint8_t* down;
  uint8_t* up;


  size_t CornerIndex();


  void Rotate(unsigned char face, unsigned char dir);

  void SetTo(uint8_t* planes, CubeEdge* edges);

  void MakeCopy(uint8_t* planes, CubeEdge* edges) const;


  EdgeCube() : cubiePlanes(nullptr), cubieEdges(nullptr), front(nullptr), back(nullptr), left(nullptr), right(nullptr), down(nullptr), up(nullptr) {
    cubiePlanes = new uint8_t[24];

    back = cubiePlanes;
    left = back + 8;
    down = left + 4;
    right = down + 4;
    front = right + 4;
    up = front + 4;

    //for (int i = 0; i < 3; i++)
    //  cubiePlanes[i] = i % 6;

    for (int i = 0; i < 4; i++) {
      back[i] = 0;
      left[i] = 1;
      down[i] = 2;
      right[i] = 3;
      front[i] = 4;
      up[i] = 5;
    }


    cubieEdges = new CubeEdge[12]{
      CubeEdge(front, up + 3, 0),
      CubeEdge(front + 2, right + 1, 1),
      CubeEdge(front + 3, down, 2),
      CubeEdge(front + 1, left + 2, 3),

      CubeEdge(up + 1, left, 4),
      CubeEdge(up + 2, right, 5),
      CubeEdge(right + 3, down + 2, 6),
      CubeEdge(left + 3, down + 1, 7),

      CubeEdge(back, front, 8),
      CubeEdge(back + 2, left + 1, 9),
      CubeEdge(back + 3, down + 3, 10),
      CubeEdge(back + 1, right + 2, 11),
    };
  }

  ~EdgeCube() {
    delete[](cubiePlanes);
    delete[](cubieEdges);
  }
};


#endif