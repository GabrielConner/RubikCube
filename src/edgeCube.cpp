#include "edgeCube.hpp"
#include "rubikCube.hpp"
#include "helperClasses.hpp"

static std::vector<uint8_t> edgePositionStillList = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};



void EdgeCube::Rotate(unsigned char face, unsigned char dir) {
  uint8_t holdC = 0;

  int i = 0;
  int ring = 0;
  int ringSize = 0;
  int offset = 0;
  int ringFaceSize = 0;


  // Front
  switch (face) {

  case 4:
    // Clockwise
    if (dir == 0) {

      // Face
      holdC = cubieEdges[0].index;
      cubieEdges[0].index = cubieEdges[1].index;
      cubieEdges[1].index = cubieEdges[3].index;
      cubieEdges[3].index = cubieEdges[2].index;
      cubieEdges[2].index = holdC;


      holdC = front[0];
      front[0] = front[1];
      front[1] = front[3];
      front[3] = front[2];
      front[2] = holdC;


      // Slice
      holdC = up[3];
      up[3] = left[2];
      left[2] = down[0];
      down[0] = right[1];
      right[1] = holdC;

      // Anticlockwise
    } else if (dir == 1) {

      // Face
      holdC = cubieEdges[0].index;
      cubieEdges[0].index = cubieEdges[2].index;
      cubieEdges[2].index = cubieEdges[3].index;
      cubieEdges[3].index = cubieEdges[1].index;
      cubieEdges[1].index = holdC;


      holdC = front[0];
      front[0] = front[2];
      front[2] = front[3];
      front[3] = front[1];
      front[1] = holdC;


      // Slice
      holdC = up[3];
      up[3] = right[1];
      right[1] = down[0];
      down[0] = left[2];
      left[2] = holdC;

      // Twist
    } else if (dir == 2) {

      // Face
      holdC = cubieEdges[0].index;
      cubieEdges[0].index = cubieEdges[3].index;
      cubieEdges[3].index = holdC;

      holdC = cubieEdges[1].index;
      cubieEdges[1].index = cubieEdges[2].index;
      cubieEdges[2].index = holdC;


      holdC = front[0];
      front[0] = front[3];
      front[3] = holdC;

      holdC = front[1];
      front[1] = front[2];
      front[2] = holdC;


      // Slice
      holdC = up[3];
      up[3] = down[0];
      down[0] = holdC;

      holdC = left[2];
      left[2] = right[1];
      right[1] = holdC;
    }

    break;

    // Back
  case 0:

    // Clockwise
    if (dir == 0) {

      // Face
      holdC = cubieEdges[8].index;
      cubieEdges[8].index = cubieEdges[9].index;
      cubieEdges[9].index = cubieEdges[11].index;
      cubieEdges[11].index = cubieEdges[10].index;
      cubieEdges[10].index = holdC;


      holdC = back[0];
      back[0] = back[1];
      back[1] = back[3];
      back[3] = back[2];
      back[2] = holdC;


      // Slice
      holdC = up[0];
      up[0] = right[2];
      right[2] = down[3];
      down[3] = left[1];
      left[1] = holdC;

      // Anticlockwise
    } else if (dir == 1) {

      // Face
      holdC = cubieEdges[8].index;
      cubieEdges[8].index = cubieEdges[10].index;
      cubieEdges[10].index = cubieEdges[11].index;
      cubieEdges[11].index = cubieEdges[9].index;
      cubieEdges[9].index = holdC;


      holdC = back[0];
      back[0] = back[2];
      back[2] = back[3];
      back[3] = back[1];
      back[1] = holdC;


      // Slice
      holdC = up[0];
      up[0] = left[1];
      left[1] = down[3];
      down[3] = right[2];
      right[2] = holdC;

      // Twist
    } else if (dir == 2) {

      // Face
      holdC = cubieEdges[8].index;
      cubieEdges[8].index = cubieEdges[11].index;
      cubieEdges[11].index = holdC;

      holdC = cubieEdges[9].index;
      cubieEdges[9].index = cubieEdges[10].index;
      cubieEdges[10].index = holdC;


      holdC = back[0];
      back[0] = back[3];
      back[3] = holdC;

      holdC = back[1];
      back[1] = back[2];
      back[2] = holdC;


      // Slice
      holdC = up[0];
      up[0] = down[3];
      down[3] = holdC;

      holdC = left[1];
      left[1] = right[2];
      right[2] = holdC;
    }

    break;

    // Left
  case 1:

    // Clockwise
    if (dir == 0) {

      // Face
      holdC = cubieEdges[5].index;
      cubieEdges[5].index = cubieEdges[2].index;
      cubieEdges[2].index = cubieEdges[6].index;
      cubieEdges[6].index = cubieEdges[9].index;
      cubieEdges[9].index = holdC;


      holdC = left[0];
      left[0] = left[1];
      left[1] = left[3];
      left[3] = left[2];
      left[2] = holdC;


      // Slice
      holdC = up[2];
      up[2] = front[2];
      front[2] = down[2];
      down[2] = back[1];
      back[1] = holdC;

      // Anticlockwise
    } else if (dir == 1) {

      // Face
      holdC = cubieEdges[5].index;
      cubieEdges[5].index = cubieEdges[9].index;
      cubieEdges[9].index = cubieEdges[6].index;
      cubieEdges[6].index = cubieEdges[2].index;
      cubieEdges[2].index = holdC;


      holdC = left[0];
      left[0] = left[2];
      left[2] = left[3];
      left[3] = left[1];
      left[1] = holdC;


      // Slice
      holdC = up[2];
      up[2] = back[1];
      back[1] = down[2];
      down[2] = front[2];
      front[2] = holdC;

      // Twist
    } else if (dir == 2) {

      // Face
      holdC = cubieEdges[5].index;
      cubieEdges[5].index = cubieEdges[6].index;
      cubieEdges[66].index = holdC;

      holdC = cubieEdges[2].index;
      cubieEdges[2].index = cubieEdges[9].index;
      cubieEdges[9].index = holdC;


      holdC = left[0];
      left[0] = left[3];
      left[3] = holdC;

      holdC = left[1];
      left[1] = left[2];
      left[2] = holdC;


      // Slice
      holdC = up[2];
      up[2] = down[2];
      down[2] = holdC;

      holdC = front[2];
      front[2] = back[2];
      back[2] = holdC;
    }

    break;

    // Right
  case 3:

    // Clockwise
    if (dir == 0) {

      // Face
      holdC = cubieCorners[1].index;
      cubieCorners[1].index = cubieCorners[3].index;
      cubieCorners[3].index = cubieCorners[7].index;
      cubieCorners[7].index = cubieCorners[5].index;
      cubieCorners[5].index = holdC;


      holdC = right[0];
      right[0] = right[2];
      right[2] = right[3];
      right[3] = right[1];
      right[1] = holdC;


      // Slice
      holdC = up[1];
      up[1] = front[1];
      front[1] = down[1];
      down[1] = back[3];
      back[3] = holdC;

      holdC = up[3];
      up[3] = front[3];
      front[3] = down[3];
      down[3] = back[1];
      back[1] = holdC;

      // Anticlockwise
    } else if (dir == 1) {

      // Face
      holdC = cubieCorners[1].index;
      cubieCorners[1].index = cubieCorners[5].index;
      cubieCorners[5].index = cubieCorners[7].index;
      cubieCorners[7].index = cubieCorners[3].index;
      cubieCorners[3].index = holdC;


      holdC = right[0];
      right[0] = right[1];
      right[1] = right[3];
      right[3] = right[2];
      right[2] = holdC;


      // Slice
      holdC = up[1];
      up[1] = back[3];
      back[3] = down[1];
      down[1] = front[1];
      front[1] = holdC;

      holdC = up[3];
      up[3] = back[1];
      back[1] = down[3];
      down[3] = front[3];
      front[3] = holdC;

      // Twist
    } else if (dir == 2) {

      // Face
      holdC = cubieCorners[1].index;
      cubieCorners[1].index = cubieCorners[7].index;
      cubieCorners[7].index = holdC;
      holdC = cubieCorners[5].index;
      cubieCorners[5].index = cubieCorners[3].index;
      cubieCorners[3].index = holdC;


      holdC = right[0];
      right[0] = right[3];
      right[3] = holdC;
      holdC = right[1];
      right[1] = right[2];
      right[2] = holdC;


      // Slice
      holdC = up[1];
      up[1] = down[1];
      down[1] = holdC;

      holdC = up[3];
      up[3] = down[3];
      down[3] = holdC;

      holdC = front[1];
      front[1] = back[3];
      back[3] = holdC;

      holdC = front[3];
      front[3] = back[1];
      back[1] = holdC;
    }

    break;

    // Up
  case 5:

    // Clockwise
    if (dir == 0) {

      // Face
      holdC = cubieCorners[4].index;
      cubieCorners[4].index = cubieCorners[0].index;
      cubieCorners[0].index = cubieCorners[1].index;
      cubieCorners[1].index = cubieCorners[5].index;
      cubieCorners[5].index = holdC;


      holdC = up[0];
      up[0] = up[2];
      up[2] = up[3];
      up[3] = up[1];
      up[1] = holdC;


      // Slice
      holdC = back[0];
      back[0] = left[1];
      left[1] = front[1];
      front[1] = right[1];
      right[1] = holdC;

      holdC = back[1];
      back[1] = left[0];
      left[0] = front[0];
      front[0] = right[0];
      right[0] = holdC;

      // Anticlockwise
    } else if (dir == 1) {

      // Face
      holdC = cubieCorners[4].index;
      cubieCorners[4].index = cubieCorners[5].index;
      cubieCorners[5].index = cubieCorners[1].index;
      cubieCorners[1].index = cubieCorners[0].index;
      cubieCorners[0].index = holdC;


      holdC = up[0];
      up[0] = up[1];
      up[1] = up[3];
      up[3] = up[2];
      up[2] = holdC;


      // Slice
      holdC = back[0];
      back[0] = right[1];
      right[1] = front[1];
      front[1] = left[1];
      left[1] = holdC;

      holdC = back[1];
      back[1] = right[0];
      right[0] = front[0];
      front[0] = left[0];
      left[0] = holdC;

      // Twist
    } else if (dir == 2) {

      // Face
      holdC = cubieCorners[4].index;
      cubieCorners[4].index = cubieCorners[1].index;
      cubieCorners[1].index = holdC;
      holdC = cubieCorners[5].index;
      cubieCorners[5].index = cubieCorners[0].index;
      cubieCorners[0].index = holdC;


      holdC = up[0];
      up[0] = up[3];
      up[3] = holdC;
      holdC = up[1];
      up[1] = up[2];
      up[2] = holdC;


      // Slice
      holdC = back[0];
      back[0] = front[1];
      front[1] = holdC;

      holdC = back[1];
      back[1] = front[0];
      front[0] = holdC;

      holdC = left[0];
      left[0] = right[0];
      right[0] = holdC;

      holdC = left[1];
      left[1] = right[1];
      right[1] = holdC;
    }

    break;

    // Down
  case 2:

    // Clockwise
    if (dir == 0) {

      // Face
      holdC = cubieCorners[2].index;
      cubieCorners[2].index = cubieCorners[6].index;
      cubieCorners[6].index = cubieCorners[7].index;
      cubieCorners[7].index = cubieCorners[3].index;
      cubieCorners[3].index = holdC;


      holdC = down[0];
      down[0] = down[2];
      down[2] = down[3];
      down[3] = down[1];
      down[1] = holdC;


      // Slice
      holdC = back[2];
      back[2] = right[3];
      right[3] = front[3];
      front[3] = left[3];
      left[3] = holdC;

      holdC = back[3];
      back[3] = right[2];
      right[2] = front[2];
      front[2] = left[2];
      left[2] = holdC;

      // Anticlockwise
    } else if (dir == 1) {

      // Face
      holdC = cubieCorners[2].index;
      cubieCorners[2].index = cubieCorners[3].index;
      cubieCorners[3].index = cubieCorners[7].index;
      cubieCorners[7].index = cubieCorners[6].index;
      cubieCorners[6].index = holdC;


      holdC = down[0];
      down[0] = down[1];
      down[1] = down[3];
      down[3] = down[2];
      down[2] = holdC;


      // Slice
      holdC = back[2];
      back[2] = left[3];
      left[3] = front[3];
      front[3] = right[3];
      right[3] = holdC;

      holdC = back[3];
      back[3] = left[2];
      left[2] = front[2];
      front[2] = right[2];
      right[2] = holdC;

      // Twist
    } else if (dir == 2) {

      // Face
      holdC = cubieCorners[2].index;
      cubieCorners[2].index = cubieCorners[7].index;
      cubieCorners[7].index = holdC;
      holdC = cubieCorners[3].index;
      cubieCorners[3].index = cubieCorners[6].index;
      cubieCorners[6].index = holdC;


      holdC = down[0];
      down[0] = down[3];
      down[3] = holdC;
      holdC = down[1];
      down[1] = down[2];
      down[2] = holdC;


      // Slice
      holdC = back[2];
      back[2] = front[3];
      front[3] = holdC;

      holdC = back[3];
      back[3] = front[2];
      front[2] = holdC;

      holdC = left[2];
      left[2] = right[2];
      right[2] = holdC;

      holdC = left[3];
      left[3] = right[3];
      right[3] = holdC;
    }

    break;
  }
}

size_t CornerCube::CornerIndex() {
  size_t posIndex = 0;
  size_t orIndex = 0;

  size_t orMul = 1;
  size_t posMul = 1;
  uint8_t i = 0, j = 0, a = 0;

  for (i = 0; i < 7; i++) {
    orIndex += orMul * cubieCorners[i].shiftFrom(RubikCube::cornerRotationStillList[cubieCorners[i].index]);

    for (j = i, a = 0; ; j++, a++) {
      if (j > 7)
        j = 0;
      if (cornerPositionStillList[j] & 0x80) {
        a--;
        continue;
      }

      if (cubieCorners[i].index == cornerPositionStillList[j]) {
        cornerPositionStillList[j] |= 0x80;

        posIndex += a * posMul;
        break;
      }
    }

    posMul *= 8ull - i;
    orMul *= 3;
  }


  cornerPositionStillList[0] &= 0xf;
  cornerPositionStillList[1] &= 0xf;
  cornerPositionStillList[2] &= 0xf;
  cornerPositionStillList[3] &= 0xf;
  cornerPositionStillList[4] &= 0xf;
  cornerPositionStillList[5] &= 0xf;
  cornerPositionStillList[6] &= 0xf;
  cornerPositionStillList[7] &= 0xf;

  return posIndex + (40320ull * orIndex);





  //for (i = 0; i < 7; i++) {
  //  orIndex += orMul * cubieCorners[i].shiftFrom(RubikCube::cornerRotationStillList[cubieCorners[i].index]);

  //  for (j = i, a = 0; ; j++, a++) {
  //    if (j > 7)
  //      j = 0;
  //    if (cornerPositionStillList[j] & 0x80) {
  //      a--;
  //      continue;
  //    }

  //    if (cubieCorners[i].index == cornerPositionStillList[j]) {
  //      posIndex += posMul * a;
  //      cornerPositionStillList[j] |= 0x80;
  //      break;
  //    }
  //  }

  //  posMul *= 8ull - i;
  //  orMul *= 3;
  //}

  //cornerPositionStillList[0] &= 0xf;
  //cornerPositionStillList[1] &= 0xf;
  //cornerPositionStillList[2] &= 0xf;
  //cornerPositionStillList[3] &= 0xf;
  //cornerPositionStillList[4] &= 0xf;
  //cornerPositionStillList[5] &= 0xf;
  //cornerPositionStillList[6] &= 0xf;
  //cornerPositionStillList[7] &= 0xf;

  //return (posIndex + 1) * (orIndex + 1) - 1;
}




void CornerCube::SetTo(uint8_t* planes, CubeCorner* corners) {
  memcpy(cubiePlanes, planes, 24);
  memcpy(cubieCorners, corners, 8 * sizeof(CubeCorner));
}


void CornerCube::MakeCopy(uint8_t* planes, CubeCorner* corners) const {
  memcpy(planes, cubiePlanes, 24);
  memcpy(corners, cubieCorners, 8 * sizeof(CubeCorner));
}