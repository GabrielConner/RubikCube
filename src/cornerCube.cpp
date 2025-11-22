#include "cornerCube.hpp"
#include "rubikCube.hpp"
#include "helperClasses.hpp"

static std::vector<uint8_t> stillList = {0, 1, 2, 3, 4, 5, 6, 7};


void CornerCube::Rotate(unsigned char face, unsigned char dir) {
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
      holdC = corners[0].index;
      corners[0].index = corners[2].index;
      corners[2].index = corners[3].index;
      corners[3].index = corners[1].index;
      corners[1].index = holdC;


      holdC = front[0];
      front[0] = front[2];
      front[2] = front[3];
      front[3] = front[1];
      front[1] = holdC;


      // Slice
      holdC = up[2];
      up[2] = left[3];
      left[3] = down[1];
      down[1] = right[0];
      right[0] = holdC;

      holdC = up[3];
      up[3] = left[1];
      left[1] = down[0];
      down[0] = right[2];
      right[2] = holdC;

      // Anticlockwise
    } else if (dir == 1) {

      // Face
      holdC = corners[0].index;
      corners[0].index = corners[1].index;
      corners[1].index = corners[3].index;
      corners[3].index = corners[2].index;
      corners[2].index = holdC;


      holdC = front[0];
      front[0] = front[1];
      front[1] = front[3];
      front[3] = front[2];
      front[2] = holdC;


      // Slice
      holdC = up[2];
      up[2] = right[0];
      right[0] = down[1];
      down[1] = left[3];
      left[3] = holdC;

      holdC = up[3];
      up[3] = right[2];
      right[2] = down[0];
      down[0] = left[1];
      left[1] = holdC;

      // Twist
    } else if (dir == 2) {

      // Face
      holdC = corners[0].index;
      corners[0].index = corners[3].index;
      corners[3].index = holdC;
      holdC = corners[1].index;
      corners[1].index = corners[2].index;
      corners[2].index = holdC;


      holdC = front[0];
      front[0] = front[3];
      front[3] = holdC;
      holdC = front[1];
      front[1] = front[2];
      front[2] = holdC;


      // Slice
      holdC = up[2];
      up[2] = down[1];
      down[1] = holdC;

      holdC = up[3];
      up[3] = down[0];
      down[0] = holdC;

      holdC = right[0];
      right[0] = left[3];
      left[3] = holdC;

      holdC = right[2];
      right[2] = left[1];
      left[1] = holdC;
    }

    break;

    // Back
  case 0:

    // Clockwise
    if (dir == 0) {

      // Face
      holdC = corners[4].index;
      corners[4].index = corners[5].index;
      corners[5].index = corners[7].index;
      corners[7].index = corners[6].index;
      corners[6].index = holdC;


      holdC = back[0];
      back[0] = back[1];
      back[1] = back[3];
      back[3] = back[2];
      back[2] = holdC;


      // Slice
      holdC = up[0];
      up[0] = right[1];
      right[1] = down[3];
      down[3] = left[2];
      left[2] = holdC;

      holdC = up[1];
      up[1] = right[3];
      right[3] = down[2];
      down[2] = left[0];
      left[0] = holdC;

      // Anticlockwise
    } else if (dir == 1) {

      // Face
      holdC = corners[4].index;
      corners[4].index = corners[6].index;
      corners[6].index = corners[7].index;
      corners[7].index = corners[5].index;
      corners[5].index = holdC;

      holdC = back[0];
      back[0] = back[2];
      back[2] = back[3];
      back[3] = back[1];
      back[1] = holdC;


      // Slice
      holdC = up[0];
      up[0] = left[2];
      left[2] = down[3];
      down[3] = right[1];
      right[1] = holdC;

      holdC = up[1];
      up[1] = left[0];
      left[0] = down[2];
      down[2] = right[3];
      right[3] = holdC;

      // Twist
    } else if (dir == 2) {

      // Face
      holdC = corners[4].index;
      corners[4].index = corners[7].index;
      corners[7].index = holdC;
      holdC = corners[5].index;
      corners[5].index = corners[6].index;
      corners[6].index = holdC;


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

      holdC = up[1];
      up[1] = down[2];
      down[2] = holdC;

      holdC = right[1];
      right[1] = left[2];
      left[2] = holdC;

      holdC = right[3];
      right[3] = left[0];
      left[0] = holdC;
    }

    break;

    // Left
  case 1:

    // Clockwise
    if (dir == 0) {

      // Face
      holdC = corners[4].index;
      corners[4].index = corners[6].index;
      corners[6].index = corners[2].index;
      corners[2].index = corners[0].index;
      corners[0].index = holdC;


      holdC = left[0];
      left[0] = left[2];
      left[2] = left[3];
      left[3] = left[1];
      left[1] = holdC;

      // Slice
      holdC = up[0];
      up[0] = back[2];
      back[2] = down[0];
      down[0] = front[0];
      front[0] = holdC;

      holdC = up[2];
      up[2] = back[0];
      back[0] = down[2];
      down[2] = front[2];
      front[2] = holdC;

      // Anticlockwise
    } else if (dir == 1) {

      // Face
      holdC = corners[4].index;
      corners[4].index = corners[0].index;
      corners[0].index = corners[2].index;
      corners[2].index = corners[6].index;
      corners[6].index = holdC;


      holdC = left[0];
      left[0] = left[1];
      left[1] = left[3];
      left[3] = left[2];
      left[2] = holdC;


      // Slice
      holdC = up[0];
      up[0] = front[0];
      front[0] = down[0];
      down[0] = back[2];
      back[2] = holdC;

      holdC = up[2];
      up[2] = front[2];
      front[2] = down[2];
      down[2] = back[0];
      back[0] = holdC;

      // Twist
    } else if (dir == 2) {

      // Face
      holdC = corners[4].index;
      corners[4].index = corners[2].index;
      corners[2].index = holdC;
      holdC = corners[0].index;
      corners[0].index = corners[6].index;
      corners[6].index = holdC;


      holdC = left[0];
      left[0] = left[3];
      left[3] = holdC;
      holdC = left[1];
      left[1] = left[2];
      left[2] = holdC;


      // Slice
      holdC = up[0];
      up[0] = down[0];
      down[0] = holdC;

      holdC = up[2];
      up[2] = down[2];
      down[2] = holdC;

      holdC = front[0];
      front[0] = back[2];
      back[2] = holdC;

      holdC = front[2];
      front[2] = back[0];
      back[0] = holdC;
    }

    break;

    // Right
  case 3:

    // Clockwise
    if (dir == 0) {

      // Face
      holdC = corners[1].index;
      corners[1].index = corners[3].index;
      corners[3].index = corners[7].index;
      corners[7].index = corners[5].index;
      corners[5].index = holdC;


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
      holdC = corners[1].index;
      corners[1].index = corners[5].index;
      corners[5].index = corners[7].index;
      corners[7].index = corners[3].index;
      corners[3].index = holdC;


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
      holdC = corners[1].index;
      corners[1].index = corners[7].index;
      corners[7].index = holdC;
      holdC = corners[5].index;
      corners[5].index = corners[3].index;
      corners[3].index = holdC;


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
      holdC = corners[4].index;
      corners[4].index = corners[0].index;
      corners[0].index = corners[1].index;
      corners[1].index = corners[5].index;
      corners[5].index = holdC;


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
      holdC = corners[4].index;
      corners[4].index = corners[5].index;
      corners[5].index = corners[1].index;
      corners[1].index = corners[0].index;
      corners[0].index = holdC;


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
      holdC = corners[4].index;
      corners[4].index = corners[1].index;
      corners[1].index = holdC;
      holdC = corners[5].index;
      corners[5].index = corners[0].index;
      corners[0].index = holdC;


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
      holdC = corners[2].index;
      corners[2].index = corners[6].index;
      corners[6].index = corners[7].index;
      corners[7].index = corners[3].index;
      corners[3].index = holdC;


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
      holdC = corners[2].index;
      corners[2].index = corners[3].index;
      corners[3].index = corners[7].index;
      corners[7].index = corners[6].index;
      corners[6].index = holdC;


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
      holdC = corners[2].index;
      corners[2].index = corners[7].index;
      corners[7].index = holdC;
      holdC = corners[3].index;
      corners[3].index = corners[6].index;
      corners[6].index = holdC;


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
  size_t orIndex = 1;

  size_t orMul = 1;
  size_t posMul = 1;
  uint8_t i = 0, j = 0, a = 0;

  for (i = 0; i < 7; i++) {
    orIndex *= corners[i].shiftFrom(RubikCube::cornerStillList[corners[i].index]);

    for (j = i, a = 0; ; j++, a++) {
      if (j > 7)
        j = 0;
      if (stillList[j] & 0x80) {
        a--;
        continue;
      }

      if (corners[i].index == stillList[j]) {
        stillList[j] |= 0x80;

        posIndex += a * posMul * orIndex;
        //orIndex = 1;
        break;
      }
    }

    posMul *= 8ull - i;
    //orMul *= 3;
  }
  //posIndex >>= 2;


  stillList[0] &= 0xf;
  stillList[1] &= 0xf;
  stillList[2] &= 0xf;
  stillList[3] &= 0xf;
  stillList[4] &= 0xf;
  stillList[5] &= 0xf;
  stillList[6] &= 0xf;
  stillList[7] &= 0xf;
  
  return posIndex ;





  //for (i = 0; i < 7; i++) {
  //  orIndex += orMul * corners[i].shiftFrom(RubikCube::cornerStillList[corners[i].index]);

  //  for (j = i, a = 0; ; j++, a++) {
  //    if (j > 7)
  //      j = 0;
  //    if (stillList[j] & 0x80) {
  //      a--;
  //      continue;
  //    }

  //    if (corners[i].index == stillList[j]) {
  //      posIndex += posMul * a;
  //      stillList[j] |= 0x80;
  //      break;
  //    }
  //  }

  //  posMul *= 8ull - i;
  //  orMul *= 3;
  //}

  //stillList[0] &= 0xf;
  //stillList[1] &= 0xf;
  //stillList[2] &= 0xf;
  //stillList[3] &= 0xf;
  //stillList[4] &= 0xf;
  //stillList[5] &= 0xf;
  //stillList[6] &= 0xf;
  //stillList[7] &= 0xf;

  //return (posIndex + 1) * (orIndex + 1) - 1;
}