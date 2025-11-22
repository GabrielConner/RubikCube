#ifndef RUBIK_CUBE_SRC_CORNER_CUBE_H
#define RUBIK_CUBE_SRC_CORNER_CUBE_H

#include <cstdint>
#include "helperClasses.hpp"



class CornerCube {
public:

  uint8_t* cubiePlanes;

  CubeCorner* corners;

  uint8_t* front;
  uint8_t* back;

  uint8_t* left;
  uint8_t* right;

  uint8_t* down;
  uint8_t* up;


  size_t CornerIndex();


  void Rotate(unsigned char face, unsigned char dir);


  CornerCube() : cubiePlanes(nullptr), corners(nullptr), front(nullptr), back(nullptr), left(nullptr), right(nullptr), down(nullptr), up(nullptr) {
    cubiePlanes = new uint8_t[24];

    back = cubiePlanes;
    left = back + 4;
    down = left + 4;
    right = down + 4;
    front = right + 4;
    up = front + 4;

    for (int i = 0; i < 4; i++) {
      back[i] = 0;
      left[i] = 1;
      down[i] = 2;
      right[i] = 3;
      front[i] = 4;
      up[i] = 5;
    }


    corners = new CubeCorner[8]{
    CubeCorner(left + 1, up + 2, front, 0),
    CubeCorner(up + 3, right, front + 1, 1),
    CubeCorner(left + 3, front + 2, down, 2),
    CubeCorner(front + 3, right + 2, down + 1, 3),
    CubeCorner(up, left, back, 4),
    CubeCorner(right + 1, up + 1, back + 1, 5),
    CubeCorner(back + 2, left + 2, down + 2, 6),
    CubeCorner(right + 3, back + 3, down + 3, 7)
    };
  }

  ~CornerCube() {
    delete[](cubiePlanes);
    delete[](corners);
  }
};

#endif