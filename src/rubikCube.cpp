#include "rubikCube.hpp"
#include "cornerCube.hpp"
#include "helperClasses.hpp"

#include <random>
#include <ctime>
#include <string>

#include <iostream>
#include <deque>
#include <vector>
#include <array>
#include <filesystem>
#include <chrono>



constexpr const uint8_t b = 0;
constexpr const uint8_t l = 1;
constexpr const uint8_t d = 2;
constexpr const uint8_t r = 3;
constexpr const uint8_t f = 4;
constexpr const uint8_t u = 5;

constexpr const uint8_t c = 0;
constexpr const uint8_t a = 1;
constexpr const uint8_t t = 2;




constexpr const char* patternHeuristicLocation = "./patternHeuristic.sdb";

static std::vector<uint8_t> cornerPositionStillList = {0, 1, 2, 3, 4, 5, 6, 7};

StillCubeCorner RubikCube::cornerRotationStillList[8] = {
    StillCubeCorner(1, 5, 4),
    StillCubeCorner(5, 3, 4),
    StillCubeCorner(1, 4, 2),
    StillCubeCorner(4, 3, 2),
    StillCubeCorner(5, 1, 0),
    StillCubeCorner(3, 5, 0),
    StillCubeCorner(0, 1, 2),
    StillCubeCorner(3, 0, 2)
};





bool ValidRubikAction(RubikActionSingle prevNode, RubikActionSingle cur);


//----------------------------------------------------------------------------------------------------
std::deque<RubikActionSingle> RubikCube::Solve() {
  RubikCube* rubik = new RubikCube(size);


  std::map<size_t, uint8_t> cornerDB = std::map<size_t, uint8_t>();
  std::map<size_t, uint8_t> firstEdgeDB = std::map<size_t, uint8_t>();
  std::map<size_t, uint8_t> secondEdgeDB = std::map<size_t, uint8_t>();

  std::deque<RubikActionSingle> ret;

  if (*rubik == *this)
    return ret;


  int maxDistance = 1;
  RubikStack stack("./stackSwapFile");

  RubikNode pNode = {};
  RubikNode pPrevNode = {};

  bool validPNode = false;
  bool validPPrevNode = false;

  bool validPNodeAction = false;

  unsigned short curDistance = 1;


  size_t node = 0;
  size_t prevNode = -1;


  size_t score = 0;

  bool found = false;

  size_t minAbove = 1;


  std::vector<RubikActionSingle> actionList;
  std::vector<unsigned short> depthActions;
  unsigned short actionCount = 0;
  unsigned short current = 0;
  unsigned short minDown = -1;

  actionList.push_back({'b', 'c', 0});
  actionList.push_back({'b', 'a', 0});
  actionList.push_back({'b', 't', 0});

  actionList.push_back({'d', 'c', 0});
  actionList.push_back({'d', 'a', 0});
  actionList.push_back({'d', 't', 0});

  actionList.push_back({'r', 'c', 0});
  actionList.push_back({'r', 'a', 0});
  actionList.push_back({'r', 't', 0});

  for (unsigned short i = 0; i < size - 1; i++) {
    actionList.push_back({'f', 'c', i});
    actionList.push_back({'f', 'a', i});
    actionList.push_back({'f', 't', i});

    actionList.push_back({'l', 'c', i});
    actionList.push_back({'l', 'a', i});
    actionList.push_back({'l', 't', i});

    actionList.push_back({'u', 'c', i});
    actionList.push_back({'u', 'a', i});
    actionList.push_back({'u', 't', i});
  }
  actionCount = actionList.size();



  while (!found) {
    if (current >= maxDistance) {
      if (current < minDown)
        minDown = current;

    }

    RubikActionSingle curAction = actionList[depthActions[current]];
    RubikActionSingle* prevNode = current > 0 ? &actionList[depthActions[current - 1]] : nullptr;

    // unsigned short score = MaxScoreOf(*whatever*);
    // if (score < minDown)
    //  minDown = score;
    if (current == 0 || ValidRubikAction(*prevNode, curAction)) {
      rubik->Rotate(curAction.face, curAction.direction, curAction.layer);
      if (*rubik == *this) {
        found = true;
      }
    }

    if (depthActions[current] < actionCount - 1) {
      depthActions[current]++;
      continue;
    }

    while (depthActions[current] >= actionCount - 1) {
      depthActions[current] = 0;

      if (current < maxDistance) {
        current++;
        depthActions.push_back(0);
      } else {
        if (current == 0) {
          maxDistance = minDown;
          minDown = -1;
          break;
        }
        current--;
      }
    }
  }








  while (!found && maxDistance < 10) {

    if (stack.empty) {
      rubik->Reset();
      stack.list.Clear();

      prevNode = -1;
      node = -1;// stack.list.GetNext(RubikNode(-1, -1, RubikNodeCreate(0, 0, 0, 0, false)));
      validPNode = false;// stack.list.Find(node);
      validPNodeAction = false;
      curDistance = 1;
      validPPrevNode = false;

      maxDistance = minAbove;

      std::cout << "Searching depth : " << maxDistance << '\n';

      minAbove = -1;
    } else {
      node = stack.topNode;
      pNode = *stack.list.Find(node);
      validPNode = true;
      stack.Pop();

      curDistance = pNode.distance + 1;

      validPNodeAction = true;
    }


    if (validPNode && pNode.distance > maxDistance) {
      if (pNode.distance < minAbove)
        minAbove = pNode.distance;
      continue;
    }

    if (validPPrevNode && pPrevNode.ValidAction()) {
      if (pNode.distance <= pPrevNode.distance) {

        RubikNode* aNode = &pPrevNode;
        for (int i = pPrevNode.distance; i >= pNode.distance && aNode->ValidAction(); i--) {
          char dir = aNode->NormDirection();
          if (dir == 'c')
            dir = 'a';
          else if (dir == 'a')
            dir = 'c';
          rubik->Rotate(aNode->NormFace(), dir, aNode->layer);

          if (aNode->prevNode != -1)
            aNode = stack.list.Find(aNode->prevNode);
          else
            break;
        }

      }
    }
    if (validPNodeAction)
      rubik->Rotate(pNode.NormFace(), pNode.NormDirection(), pNode.layer);
    prevNode = node;
    pPrevNode = pNode;
    validPPrevNode = validPNode;


    for (unsigned short i = 0; i < size - 1; i++) {
      if (i == centerLine)
        continue;

      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'f', 'c', i})) {
        rubik->Rotate('f', 'c', i);
        prevNode = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 4, 0, true, i), node);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('f', 'a', i);
      }


      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'f', 'a', i})) {
        rubik->Rotate('f', 'a', i);
        prevNode = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 4, 1, true, i), node);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('f', 'c', i);
      }


      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'f', 't', i})) {
        rubik->Rotate('f', 't', i);
        prevNode = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 4, 2, true, i), node);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('f', 't', i);
      }


      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'b', 'c', i})) {
        rubik->Rotate('b', 'c', i);
        prevNode = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 0, 0, true, i), node);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('b', 'a', i);
      }



      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'b', 'a', i})) {
        rubik->Rotate('b', 'a', i);
        prevNode = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 0, 1, true, i), node);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('b', 'c', i);
      }



      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'b', 't', i})) {
        rubik->Rotate('b', 't', i);
        prevNode = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 0, 2, true, i), node);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('b', 't', i);
      }



      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'l', 'c', i})) {
        rubik->Rotate('l', 'c', i);
        prevNode = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 1, 0, true, i), node);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('l', 'a', i);
      }



      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'l', 'a', i})) {
        rubik->Rotate('l', 'a', i);
        prevNode = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 1, 1, true, i), node);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('l', 'c', i);
      }



      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'l', 't', i})) {
        rubik->Rotate('l', 't', i);
        prevNode = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 1, 2, true, i), node);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('l', 't', i);
      }



      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'r', 'c', i})) {
        rubik->Rotate('r', 'c', i);
        prevNode = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 3, 0, true, i), node);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('r', 'a', i);
      }



      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'r', 'a', i})) {
        rubik->Rotate('r', 'a', i);
        prevNode = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 3, 1, true, i), node);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('r', 'c', i);
      }



      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'r', 't', i})) {
        rubik->Rotate('r', 't', i);
        prevNode = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 3, 2, true, i), node);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('r', 't', i);
      }



      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'u', 'c', i})) {
        rubik->Rotate('u', 'c', i);
        prevNode = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 5, 0, true, i), node);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('u', 'a', i);
      }



      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'u', 'a', i})) {
        rubik->Rotate('u', 'a', i);
        prevNode = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 5, 1, true, i), node);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('u', 'c', i);
      }



      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'u', 't', i})) {
        rubik->Rotate('u', 't', i);
        prevNode = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 5, 2, true, i), node);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('u', 't', i);
      }



      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'d', 'c', i})) {
        rubik->Rotate('d', 'c', i);
        prevNode = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 2, 0, true, i), node);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('d', 'a', i);
      }



      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'d', 'a', i})) {
        rubik->Rotate('d', 'a', i);
        prevNode = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 2, 1, true, i), node);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('d', 'c', i);
      }



      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'d', 't', i})) {
        rubik->Rotate('d', 't', i);
        prevNode = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 2, 2, true, i), node);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('d', 't', i);
      }
    }
  }
  delete(rubik);




  if (found) {
    RubikNode* aNode = stack.list.Find(prevNode);
    while (aNode != nullptr && aNode->ValidAction()) {
      unsigned char dir = aNode->NormDirection();
      if (dir == 'c')
        dir = 'a';
      else if (dir == 'a')
        dir = 'c';

      ret.push_back({aNode->NormFace(), dir, aNode->layer});
      if (aNode->prevNode != -1)
        aNode = stack.list.Find(aNode->prevNode);
      else
        aNode = nullptr;
    }
  }

  _CrtMemState state1, state2, diff;
  _CrtMemCheckpoint(&state1);

  //std::cout << "Ending cornerDB size : " << cornerDB.size() << '\n';
  //std::cout << "Ending firstEdgeDB size : " << firstEdgeDB.size() << '\n';
  //std::cout << "Ending secondEdgeDB size : " << secondEdgeDB.size() << '\n';
  stack.list.Clear();

  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);

  _CrtMemCheckpoint(&state2);
  if (_CrtMemDifference(&diff, &state1, &state2))
    _CrtMemDumpStatistics(&diff);

  return ret;
}




//----------------------------------------------------------------------------------------------------
unsigned int RubikCube::MaxScoreOf(unsigned short distance, std::map<size_t, uint8_t>& cubieCorners, std::map<size_t, uint8_t>& firstEdges, std::map<size_t, uint8_t>& secondEdges) {
  size_t cornerIndex = CornerIndex();
  size_t firstEdgeIndex = FirstEdgeIndex();
  size_t secondEdgeIndex = SecondEdgeIndex();

  unsigned int cornerVal, firstEdgeVal, secondEdgeVal;
  cornerVal = firstEdgeVal = secondEdgeVal = 0;

  std::map<size_t, uint8_t>::iterator search;


  search = cubieCorners.find(cornerIndex);
  if (search == cubieCorners.end()) {
    cubieCorners.insert({cornerIndex, distance});
    cornerVal = distance;
  } else {
    if (search->second > distance)
      search->second = distance;
    cornerVal = distance;
  }


  search = firstEdges.find(firstEdgeIndex);
  if (search == firstEdges.end()) {
    firstEdges.insert({firstEdgeIndex, distance});
    firstEdgeVal = distance;
  } else {
    if (search->second > distance)
      search->second = distance;
    firstEdgeVal = distance;
  }


  search = secondEdges.find(secondEdgeIndex);
  if (search == secondEdges.end()) {
    secondEdges.insert({secondEdgeIndex, distance});
    secondEdgeVal = distance;
  } else {
    if (search->second > distance)
      search->second = distance;
    secondEdgeVal = distance;
  }


  return std::max({cornerVal, firstEdgeVal, secondEdgeVal});
  return distance;
}





//----------------------------------------------------------------------------------------------------
void RubikCube::Rotate(unsigned char face, unsigned char dir, unsigned short layer) {
  if (size <= 1 || layer >= size)
    return;

  if (face == 4 && layer >= size - 1) {
    face = 0;
    if (dir != 2)
      dir = dir == 0 ? 1 : 0;
    layer = 0;
  } else if (face == 0 && layer >= size - 1) {
    face = 4;
    if (dir != 2)
      dir = dir == 0 ? 1 : 0;
    layer = 0;
  } else if (face == 1 && layer >= size - 1) {
    face = 3;
    if (dir != 2)
      dir = dir == 0 ? 1 : 0;
    layer = 0;
  } else if (face == 3 && layer >= size - 1) {
    face = 1;
    if (dir != 2)
      dir = dir == 0 ? 1 : 0;
    layer = 0;
  } else if (face == 5 && layer >= size - 1) {
    face = 2;
    if (dir != 2)
      dir = dir == 0 ? 1 : 0;
    layer = 0;
  } else if (face == 2 && layer >= size - 1) {
    face = 5;
    if (dir != 2)
      dir = dir == 0 ? 1 : 0;
    layer = 0;
  }

  uint8_t* buff = new uint8_t[4 * size];
  uint8_t* lineUp = buff;
  uint8_t* lineRight = lineUp + size;
  uint8_t* lineDown = lineRight + size;
  uint8_t* lineLeft = lineDown + size;
  uint8_t holdC = 0;

  int i;
  int ring;
  int ringSize;
  int offset;
  int ringFaceSize;


  // Front
  switch (face) {
    
  case 4:

    // Clockwise
    if (dir == 0) {

      // Face
      if (layer == 0) {
        holdC = cubieCorners[0].index;
        cubieCorners[0].index = cubieCorners[2].index;
        cubieCorners[2].index = cubieCorners[3].index;
        cubieCorners[3].index = cubieCorners[1].index;
        cubieCorners[1].index = holdC;


        for (ring = 0; ring < size / 2; ring++) {
          ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          offset = ring * (size + 1);
          ringFaceSize = ringSize * ringSize;


          for (i = 0; i < ringSize; i++) {
            lineUp[i] = front[offset + i];
            lineRight[i] = front[offset + ringSize - 1 + (i * size)];
            lineDown[i] = front[faceSize - offset - 1 - i];
            lineLeft[i] = front[faceSize - offset - ringSize - (i * size)];
          }

          for (i = 0; i < ringSize; i++) {
            front[offset + i] = lineLeft[i];

            front[offset + ringSize - 1 + (i * size)] = lineUp[i];
            front[faceSize - offset - 1 - i] = lineRight[i];
            front[faceSize - offset - ringSize - (i * size)] = lineDown[i];
          }
        }

      }

      // Slice

      for (i = 0; i < size; i++) {
        lineUp[i] = up[ce - (layer * size) + i];
        lineRight[i] = right[layer + (i * size)];
        lineDown[i] = down[(layer * size) + re - i];
        lineLeft[i] = left[ae - layer - (i * size)];
      }

      for (i = 0; i < size; i++) {
        up[ce - (layer * size) + i] = lineLeft[i];
        right[layer + (i * size)] = lineUp[i];
        down[(layer * size) + re - i] = lineRight[i];
        left[ae - layer - (i * size)] = lineDown[i];
      }



      // Anticlockwise
    } else if (dir == 1) {

      // Face
      if (layer == 0) {
        holdC = cubieCorners[0].index;
        cubieCorners[0].index = cubieCorners[1].index;
        cubieCorners[1].index = cubieCorners[3].index;
        cubieCorners[3].index = cubieCorners[2].index;
        cubieCorners[2].index = holdC;


        for (ring = 0; ring < size / 2; ring++) {
          ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          offset = ring * (size + 1);
          ringFaceSize = ringSize * ringSize;


          for (i = 0; i < ringSize; i++) {
            lineUp[i] = front[offset + i];
            lineRight[i] = front[offset + ringSize - 1 + (i * size)];
            lineDown[i] = front[faceSize - offset - 1 - i];
            lineLeft[i] = front[faceSize - offset - ringSize - (i * size)];
          }

          for (i = 0; i < ringSize; i++) {
            front[offset + i] = lineRight[i];

            front[offset + ringSize - 1 + (i * size)] = lineDown[i];
            front[faceSize - offset - 1 - i] = lineLeft[i];
            front[faceSize - offset - ringSize - (i * size)] = lineUp[i];
          }
        }

      }

      // Slice

      for (i = 0; i < size; i++) {
        lineUp[i] = up[ce - (layer * size) + i];
        lineRight[i] = right[layer + (i * size)];
        lineDown[i] = down[(layer * size) + re - i];
        lineLeft[i] = left[ae - layer - (i * size)];
      }

      for (i = 0; i < size; i++) {
        up[ce - (layer * size) + i] = lineRight[i];
        right[layer + (i * size)] = lineDown[i];
        down[(layer * size) + re - i] = lineLeft[i];
        left[ae - layer - (i * size)] = lineUp[i];
      }



      // Twist
    } else if (dir == 2) {

      // Face
      if (layer == 0) {
        holdC = cubieCorners[0].index;
        cubieCorners[0].index = cubieCorners[3].index;
        cubieCorners[3].index = holdC;
        holdC = cubieCorners[1].index;
        cubieCorners[1].index = cubieCorners[2].index;
        cubieCorners[2].index = holdC;


        for (ring = 0; ring < size / 2; ring++) {
          ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          offset = ring * (size + 1);
          ringFaceSize = ringSize * ringSize;


          for (i = 0; i < ringSize; i++) {
            lineUp[i] = front[offset + i];
            lineRight[i] = front[offset + ringSize - 1 + (i * size)];
            lineDown[i] = front[faceSize - offset - 1 - i];
            lineLeft[i] = front[faceSize - offset - ringSize - (i * size)];
          }

          for (i = 0; i < ringSize; i++) {
            front[offset + i] = lineDown[i];

            front[offset + ringSize - 1 + (i * size)] = lineLeft[i];
            front[faceSize - offset - 1 - i] = lineUp[i];
            front[faceSize - offset - ringSize - (i * size)] = lineRight[i];
          }
        }

      }

      // Slice

      for (i = 0; i < size; i++) {
        lineUp[i] = up[ce - (layer * size) + i];
        lineRight[i] = right[layer + (i * size)];
        lineDown[i] = down[(layer * size) + re - i];
        lineLeft[i] = left[ae - layer - (i * size)];
      }

      for (i = 0; i < size; i++) {
        up[ce - (layer * size) + i] = lineDown[i];
        right[layer + (i * size)] = lineLeft[i];
        down[(layer * size) + re - i] = lineUp[i];
        left[ae - layer - (i * size)] = lineRight[i];
      }
    }

    break;

    // Back
  case 0:

    // Clockwise
    if (dir == 0) {

      // Face
      if (layer == 0) {
        holdC = cubieCorners[4].index;
        cubieCorners[4].index = cubieCorners[5].index;
        cubieCorners[5].index = cubieCorners[7].index;
        cubieCorners[7].index = cubieCorners[6].index;
        cubieCorners[6].index = holdC;


        for (ring = 0; ring < size / 2; ring++) {
          ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          offset = ring * (size + 1);
          ringFaceSize = ringSize * ringSize;


          for (i = 0; i < ringSize; i++) {
            lineUp[i] = back[offset + i];
            lineRight[i] = back[offset + ringSize - 1 + (i * size)];
            lineDown[i] = back[faceSize - offset - 1 - i];
            lineLeft[i] = back[faceSize - offset - ringSize - (i * size)];
          }

          for (i = 0; i < ringSize; i++) {
            back[offset + i] = lineRight[i];

            back[offset + ringSize - 1 + (i * size)] = lineDown[i];
            back[faceSize - offset - 1 - i] = lineLeft[i];
            back[faceSize - offset - ringSize - (i * size)] = lineUp[i];
          }
        }

      }

      // Slice

      for (i = 0; i < size; i++) {
        lineUp[i] = up[re + (layer * size) - i];
        lineRight[i] = left[layer + (i * size)];
        lineDown[i] = down[ce - (layer * size) + i];
        lineLeft[i] = right[ae - layer - (i * size)];
      }

      for (i = 0; i < size; i++) {
        up[re + (layer * size) - i] = lineLeft[i];
        left[layer + (i * size)] = lineUp[i];
        down[ce - (layer * size) + i] = lineRight[i];
        right[ae - layer - (i * size)] = lineDown[i];
      }



      // Anticlockwise
    } else if (dir == 1) {

      // Face
      if (layer == 0) {
        holdC = cubieCorners[4].index;
        cubieCorners[4].index = cubieCorners[6].index;
        cubieCorners[6].index = cubieCorners[7].index;
        cubieCorners[7].index = cubieCorners[5].index;
        cubieCorners[5].index = holdC;


        for (ring = 0; ring < size / 2; ring++) {
          ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          offset = ring * (size + 1);
          ringFaceSize = ringSize * ringSize;


          for (i = 0; i < ringSize; i++) {
            lineUp[i] = back[offset + i];
            lineRight[i] = back[offset + ringSize - 1 + (i * size)];
            lineDown[i] = back[faceSize - offset - 1 - i];
            lineLeft[i] = back[faceSize - offset - ringSize - (i * size)];
          }

          for (i = 0; i < ringSize; i++) {
            back[offset + i] = lineLeft[i];

            back[offset + ringSize - 1 + (i * size)] = lineUp[i];
            back[faceSize - offset - 1 - i] = lineRight[i];
            back[faceSize - offset - ringSize - (i * size)] = lineDown[i];
          }
        }

      }


      // Slice

      for (i = 0; i < size; i++) {
        lineUp[i] = up[re + (layer * size) - i];
        lineRight[i] = left[layer + (i * size)];
        lineDown[i] = down[ce - (layer * size) + i];
        lineLeft[i] = right[ae - layer - (i * size)];
      }

      for (i = 0; i < size; i++) {
        up[re + (layer * size) - i] = lineRight[i];
        left[layer + (i * size)] = lineDown[i];
        down[ce - (layer * size) + i] = lineLeft[i];
        right[ae - layer - (i * size)] = lineUp[i];
      }



      // Twist
    } else if (dir == 2) {

      // Face
      if (layer == 0) {
        holdC = cubieCorners[4].index;
        cubieCorners[4].index = cubieCorners[7].index;
        cubieCorners[7].index = holdC;
        holdC = cubieCorners[5].index;
        cubieCorners[5].index = cubieCorners[6].index;
        cubieCorners[6].index = holdC;


        for (ring = 0; ring < size / 2; ring++) {
          ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          offset = ring * (size + 1);
          ringFaceSize = ringSize * ringSize;


          for (i = 0; i < ringSize; i++) {
            lineUp[i] = back[offset + i];
            lineRight[i] = back[offset + ringSize - 1 + (i * size)];
            lineDown[i] = back[faceSize - offset - 1 - i];
            lineLeft[i] = back[faceSize - offset - ringSize - (i * size)];
          }

          for (i = 0; i < ringSize; i++) {
            back[offset + i] = lineDown[i];

            back[offset + ringSize - 1 + (i * size)] = lineLeft[i];
            back[faceSize - offset - 1 - i] = lineUp[i];
            back[faceSize - offset - ringSize - (i * size)] = lineRight[i];
          }
        }
      }


      // Slice

      for (i = 0; i < size; i++) {
        lineUp[i] = up[re + (layer * size) - i];
        lineRight[i] = left[layer + (i * size)];
        lineDown[i] = down[ce - (layer * size) + i];
        lineLeft[i] = right[ae - layer - (i * size)];
      }

      for (i = 0; i < size; i++) {
        up[re + (layer * size) - i] = lineDown[i];
        left[layer + (i * size)] = lineLeft[i];
        down[ce - (layer * size) + i] = lineUp[i];
        right[ae - layer - (i * size)] = lineRight[i];
      }
    }

    break;

    // Left
  case 1:

    // Clockwise
    if (dir == 0) {

      // Face
      if (layer == 0) {
        holdC = cubieCorners[4].index;
        cubieCorners[4].index = cubieCorners[6].index;
        cubieCorners[6].index = cubieCorners[2].index;
        cubieCorners[2].index = cubieCorners[0].index;
        cubieCorners[0].index = holdC;


        for (ring = 0; ring < size / 2; ring++) {
          ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          offset = ring * (size + 1);
          ringFaceSize = ringSize * ringSize;


          for (i = 0; i < ringSize; i++) {
            lineUp[i] = left[offset + i];
            lineRight[i] = left[offset + ringSize - 1 + (i * size)];
            lineDown[i] = left[faceSize - offset - 1 - i];
            lineLeft[i] = left[faceSize - offset - ringSize - (i * size)];
          }

          for (i = 0; i < ringSize; i++) {
            left[offset + i] = lineLeft[i];

            left[offset + ringSize - 1 + (i * size)] = lineUp[i];
            left[faceSize - offset - 1 - i] = lineRight[i];
            left[faceSize - offset - ringSize - (i * size)] = lineDown[i];
          }
        }

      }

      // Slice

      for (i = 0; i < size; i++) {
        lineUp[i] = up[layer + (i * size)];
        lineRight[i] = front[layer + (i * size)];
        lineDown[i] = down[layer + (i * size)];
        lineLeft[i] = back[ce + layer - (i * size)];
      }

      for (i = 0; i < size; i++) {
        up[layer + (i * size)] = lineLeft[i];
        front[layer + (i * size)] = lineUp[i];
        down[layer + (i * size)] = lineRight[i];
        back[ce + layer - (i * size)] = lineDown[i];
      }

      // Anticlockwise
    } else if (dir == 1) {

      // Face
      if (layer == 0) {
        holdC = cubieCorners[4].index;
        cubieCorners[4].index = cubieCorners[0].index;
        cubieCorners[0].index = cubieCorners[2].index;
        cubieCorners[2].index = cubieCorners[6].index;
        cubieCorners[6].index = holdC;


        for (ring = 0; ring < size / 2; ring++) {
          ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          offset = ring * (size + 1);
          ringFaceSize = ringSize * ringSize;


          for (i = 0; i < ringSize; i++) {
            lineUp[i] = left[offset + i];
            lineRight[i] = left[offset + ringSize - 1 + (i * size)];
            lineDown[i] = left[faceSize - offset - 1 - i];
            lineLeft[i] = left[faceSize - offset - ringSize - (i * size)];
          }

          for (i = 0; i < ringSize; i++) {
            left[offset + i] = lineRight[i];

            left[offset + ringSize - 1 + (i * size)] = lineDown[i];
            left[faceSize - offset - 1 - i] = lineLeft[i];
            left[faceSize - offset - ringSize - (i * size)] = lineUp[i];
          }

        }

        // Slice

        for (i = 0; i < size; i++) {
          lineUp[i] = up[layer + (i * size)];
          lineRight[i] = front[layer + (i * size)];
          lineDown[i] = down[layer + (i * size)];
          lineLeft[i] = back[ce + layer - (i * size)];
        }

        for (i = 0; i < size; i++) {
          up[layer + (i * size)] = lineRight[i];
          front[layer + (i * size)] = lineDown[i];
          down[layer + (i * size)] = lineLeft[i];
          back[ce + layer - (i * size)] = lineUp[i];
        }
      }



      // Twist
    } else if (dir == 2) {

      // Face
      if (layer == 0) {
        holdC = cubieCorners[4].index;
        cubieCorners[4].index = cubieCorners[2].index;
        cubieCorners[2].index = holdC;
        holdC = cubieCorners[0].index;
        cubieCorners[0].index = cubieCorners[6].index;
        cubieCorners[6].index = holdC;


        for (ring = 0; ring < size / 2; ring++) {
          ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          offset = ring * (size + 1);
          ringFaceSize = ringSize * ringSize;


          for (i = 0; i < ringSize; i++) {
            lineUp[i] = left[offset + i];
            lineRight[i] = left[offset + ringSize - 1 + (i * size)];
            lineDown[i] = left[faceSize - offset - 1 - i];
            lineLeft[i] = left[faceSize - offset - ringSize - (i * size)];
          }

          for (i = 0; i < ringSize; i++) {
            left[offset + i] = lineDown[i];

            left[offset + ringSize - 1 + (i * size)] = lineLeft[i];
            left[faceSize - offset - 1 - i] = lineUp[i];
            left[faceSize - offset - ringSize - (i * size)] = lineRight[i];
          }

        }

        // Slice

        for (i = 0; i < size; i++) {
          lineUp[i] = up[layer + (i * size)];
          lineRight[i] = front[layer + (i * size)];
          lineDown[i] = down[layer + (i * size)];
          lineLeft[i] = back[ce + layer - (i * size)];
        }

        for (i = 0; i < size; i++) {
          up[layer + (i * size)] = lineDown[i];
          front[layer + (i * size)] = lineLeft[i];
          down[layer + (i * size)] = lineUp[i];
          back[ce + layer - (i * size)] = lineRight[i];
        }
      }
    }

    break;

    // Right
  case 3:

    // Clockwise
    if (dir == 0) {

      // Face
      if (layer == 0) {
        holdC = cubieCorners[1].index;
        cubieCorners[1].index = cubieCorners[3].index;
        cubieCorners[3].index = cubieCorners[7].index;
        cubieCorners[7].index = cubieCorners[5].index;
        cubieCorners[5].index = holdC;


        for (ring = 0; ring < size / 2; ring++) {
          ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          offset = ring * (size + 1);
          ringFaceSize = ringSize * ringSize;


          for (i = 0; i < ringSize; i++) {
            lineUp[i] = right[offset + i];
            lineRight[i] = right[offset + ringSize - 1 + (i * size)];
            lineDown[i] = right[faceSize - offset - 1 - i];
            lineLeft[i] = right[faceSize - offset - ringSize - (i * size)];
          }

          for (i = 0; i < ringSize; i++) {
            right[offset + i] = lineLeft[i];

            right[offset + ringSize - 1 + (i * size)] = lineUp[i];
            right[faceSize - offset - 1 - i] = lineRight[i];
            right[faceSize - offset - ringSize - (i * size)] = lineDown[i];
          }
        }

      }

      // Slice

      for (i = 0; i < size; i++) {
        lineUp[i] = up[re - layer + (i * size)];
        lineRight[i] = back[ae - (i * size) + layer];
        lineDown[i] = down[re - layer + (i * size)];
        lineLeft[i] = front[re - layer + (i * size)];
      }

      for (i = 0; i < size; i++) {
        up[re - layer + (i * size)] = lineLeft[i];
        back[ae - (i * size) + layer] = lineUp[i];
        down[re - layer + (i * size)] = lineRight[i];
        front[re - layer + (i * size)] = lineDown[i];
      }



      // Anticlockwise
    } else if (dir == 1) {

      // Face
      if (layer == 0) {
        holdC = cubieCorners[1].index;
        cubieCorners[1].index = cubieCorners[5].index;
        cubieCorners[5].index = cubieCorners[7].index;
        cubieCorners[7].index = cubieCorners[3].index;
        cubieCorners[3].index = holdC;


        for (ring = 0; ring < size / 2; ring++) {
          ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          offset = ring * (size + 1);
          ringFaceSize = ringSize * ringSize;


          for (i = 0; i < ringSize; i++) {
            lineUp[i] = right[offset + i];
            lineRight[i] = right[offset + ringSize - 1 + (i * size)];
            lineDown[i] = right[faceSize - offset - 1 - i];
            lineLeft[i] = right[faceSize - offset - ringSize - (i * size)];
          }

          for (i = 0; i < ringSize; i++) {
            right[offset + i] = lineRight[i];

            right[offset + ringSize - 1 + (i * size)] = lineDown[i];
            right[faceSize - offset - 1 - i] = lineLeft[i];
            right[faceSize - offset - ringSize - (i * size)] = lineUp[i];
          }
        }

      }

      // Slice

      for (i = 0; i < size; i++) {
        lineUp[i] = up[re - layer + (i * size)];
        lineRight[i] = back[ae - (i * size) + layer];
        lineDown[i] = down[re - layer + (i * size)];
        lineLeft[i] = front[re - layer + (i * size)];
      }

      for (i = 0; i < size; i++) {
        up[re - layer + (i * size)] = lineRight[i];
        back[ae - (i * size) + layer] = lineDown[i];
        down[re - layer + (i * size)] = lineLeft[i];
        front[re - layer + (i * size)] = lineUp[i];
      }



      // Twist
    } else if (dir == 2) {

      // Face
      if (layer == 0) {
        holdC = cubieCorners[1].index;
        cubieCorners[1].index = cubieCorners[7].index;
        cubieCorners[7].index = holdC;
        holdC = cubieCorners[5].index;
        cubieCorners[5].index = cubieCorners[3].index;
        cubieCorners[3].index = holdC;


        for (ring = 0; ring < size / 2; ring++) {
          ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          offset = ring * (size + 1);
          ringFaceSize = ringSize * ringSize;


          for (i = 0; i < ringSize; i++) {
            lineUp[i] = right[offset + i];
            lineRight[i] = right[offset + ringSize - 1 + (i * size)];
            lineDown[i] = right[faceSize - offset - 1 - i];
            lineLeft[i] = right[faceSize - offset - ringSize - (i * size)];
          }

          for (i = 0; i < ringSize; i++) {
            right[offset + i] = lineDown[i];

            right[offset + ringSize - 1 + (i * size)] = lineLeft[i];
            right[faceSize - offset - 1 - i] = lineUp[i];
            right[faceSize - offset - ringSize - (i * size)] = lineRight[i];
          }
        }

      }

      // Slice

      for (i = 0; i < size; i++) {
        lineUp[i] = up[re - layer + (i * size)];
        lineRight[i] = back[ae - (i * size) + layer];
        lineDown[i] = down[re - layer + (i * size)];
        lineLeft[i] = front[re - layer + (i * size)];
      }

      for (i = 0; i < size; i++) {
        up[re - layer + (i * size)] = lineDown[i];
        back[ae - (i * size) + layer] = lineLeft[i];
        down[re - layer + (i * size)] = lineUp[i];
        front[re - layer + (i * size)] = lineRight[i];
      }
    }

    break;

    // Up
  case 5:

    // Clockwise
    if (dir == 0) {

      // Face
      if (layer == 0) {
        holdC = cubieCorners[4].index;
        cubieCorners[4].index = cubieCorners[0].index;
        cubieCorners[0].index = cubieCorners[1].index;
        cubieCorners[1].index = cubieCorners[5].index;
        cubieCorners[5].index = holdC;

        for (ring = 0; ring < size / 2; ring++) {
          ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          offset = ring * (size + 1);
          ringFaceSize = ringSize * ringSize;


          for (i = 0; i < ringSize; i++) {
            lineUp[i] = up[offset + i];
            lineRight[i] = up[offset + ringSize - 1 + (i * size)];
            lineDown[i] = up[faceSize - offset - 1 - i];
            lineLeft[i] = up[faceSize - offset - ringSize - (i * size)];
          }

          for (i = 0; i < ringSize; i++) {
            up[offset + i] = lineLeft[i];

            up[offset + ringSize - 1 + (i * size)] = lineUp[i];
            up[faceSize - offset - 1 - i] = lineRight[i];
            up[faceSize - offset - ringSize - (i * size)] = lineDown[i];
          }
        }

      }

      // Slice

      for (i = 0; i < size; i++) {
        lineUp[i] = back[(size * layer) + re - i];
        lineRight[i] = right[(size * layer) + i];
        lineDown[i] = front[(size * layer) + i];
        lineLeft[i] = left[(size * layer) + i];
      }

      for (i = 0; i < size; i++) {
        back[(size * layer) + re - i] = lineLeft[i];
        right[(size * layer) + i] = lineUp[i];
        front[(size * layer) + i] = lineRight[i];
        left[(size * layer) + i] = lineDown[i];
      }


      // Anticlockwise
    } else if (dir == 1) {

      // Face
      if (layer == 0) {
        holdC = cubieCorners[4].index;
        cubieCorners[4].index = cubieCorners[5].index;
        cubieCorners[5].index = cubieCorners[1].index;
        cubieCorners[1].index = cubieCorners[0].index;
        cubieCorners[0].index = holdC;


        for (ring = 0; ring < size / 2; ring++) {
          ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          offset = ring * (size + 1);
          ringFaceSize = ringSize * ringSize;


          for (i = 0; i < ringSize; i++) {
            lineUp[i] = up[offset + i];
            lineRight[i] = up[offset + ringSize - 1 + (i * size)];
            lineDown[i] = up[faceSize - offset - 1 - i];
            lineLeft[i] = up[faceSize - offset - ringSize - (i * size)];
          }

          for (i = 0; i < ringSize; i++) {
            up[offset + i] = lineRight[i];

            up[offset + ringSize - 1 + (i * size)] = lineDown[i];
            up[faceSize - offset - 1 - i] = lineLeft[i];
            up[faceSize - offset - ringSize - (i * size)] = lineUp[i];
          }
        }

      }

      // Slice

      for (i = 0; i < size; i++) {
        lineUp[i] = back[(size * layer) + re - i];
        lineRight[i] = right[(size * layer) + i];
        lineDown[i] = front[(size * layer) + i];
        lineLeft[i] = left[(size * layer) + i];
      }

      for (i = 0; i < size; i++) {
        back[(size * layer) + re - i] = lineRight[i];
        right[(size * layer) + i] = lineDown[i];
        front[(size * layer) + i] = lineLeft[i];
        left[(size * layer) + i] = lineUp[i];
      }



      // Twist
    } else if (dir == 2) {

      // Face
      if (layer == 0) {
        holdC = cubieCorners[4].index;
        cubieCorners[4].index = cubieCorners[1].index;
        cubieCorners[1].index = holdC;
        holdC = cubieCorners[5].index;
        cubieCorners[5].index = cubieCorners[0].index;
        cubieCorners[0].index = holdC;


        for (ring = 0; ring < size / 2; ring++) {
          ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          offset = ring * (size + 1);
          ringFaceSize = ringSize * ringSize;


          for (i = 0; i < ringSize; i++) {
            lineUp[i] = up[offset + i];
            lineRight[i] = up[offset + ringSize - 1 + (i * size)];
            lineDown[i] = up[faceSize - offset - 1 - i];
            lineLeft[i] = up[faceSize - offset - ringSize - (i * size)];
          }

          for (i = 0; i < ringSize; i++) {
            up[offset + i] = lineDown[i];

            up[offset + ringSize - 1 + (i * size)] = lineLeft[i];
            up[faceSize - offset - 1 - i] = lineUp[i];
            up[faceSize - offset - ringSize - (i * size)] = lineRight[i];
          }
        }

      }

      // Slice

      for (i = 0; i < size; i++) {
        lineUp[i] = back[(size * layer) + re - i];
        lineRight[i] = right[(size * layer) + i];
        lineDown[i] = front[(size * layer) + i];
        lineLeft[i] = left[(size * layer) + i];
      }

      for (i = 0; i < size; i++) {
        back[(size * layer) + re - i] = lineDown[i];
        right[(size * layer) + i] = lineLeft[i];
        front[(size * layer) + i] = lineUp[i];
        left[(size * layer) + i] = lineRight[i];
      }
    }

    break;

    // Down
  case 2:

    // Clockwise
    if (dir == 0) {

      // Face
      if (layer == 0) {
        holdC = cubieCorners[2].index;
        cubieCorners[2].index = cubieCorners[6].index;
        cubieCorners[6].index = cubieCorners[7].index;
        cubieCorners[7].index = cubieCorners[3].index;
        cubieCorners[3].index = holdC;


        for (ring = 0; ring < size / 2; ring++) {
          ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          offset = ring * (size + 1);
          ringFaceSize = ringSize * ringSize;


          for (i = 0; i < ringSize; i++) {
            lineUp[i] = down[offset + i];
            lineRight[i] = down[offset + ringSize - 1 + (i * size)];
            lineDown[i] = down[faceSize - offset - 1 - i];
            lineLeft[i] = down[faceSize - offset - ringSize - (i * size)];
          }

          for (i = 0; i < ringSize; i++) {
            down[offset + i] = lineLeft[i];

            down[offset + ringSize - 1 + (i * size)] = lineUp[i];
            down[faceSize - offset - 1 - i] = lineRight[i];
            down[faceSize - offset - ringSize - (i * size)] = lineDown[i];
          }
        }

      }

      // Slice

      for (i = 0; i < size; i++) {
        lineUp[i] = front[ce + i - (layer * size)];
        lineRight[i] = right[ce + i - (layer * size)];
        lineDown[i] = back[ae - i - (layer * size)];
        lineLeft[i] = left[ce + i - (layer * size)];
      }

      for (i = 0; i < size; i++) {
        front[ce + i - (layer * size)] = lineLeft[i];
        right[ce + i - (layer * size)] = lineUp[i];
        back[ae - i - (layer * size)] = lineRight[i];
        left[ce + i - (layer * size)] = lineDown[i];
      }



      // Anticlockwise
    } else if (dir == 1) {

      // Face
      if (layer == 0) {
        holdC = cubieCorners[2].index;
        cubieCorners[2].index = cubieCorners[3].index;
        cubieCorners[3].index = cubieCorners[7].index;
        cubieCorners[7].index = cubieCorners[6].index;
        cubieCorners[6].index = holdC;


        for (ring = 0; ring < size / 2; ring++) {
          ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          offset = ring * (size + 1);
          ringFaceSize = ringSize * ringSize;


          for (i = 0; i < ringSize; i++) {
            lineUp[i] = down[offset + i];
            lineRight[i] = down[offset + ringSize - 1 + (i * size)];
            lineDown[i] = down[faceSize - offset - 1 - i];
            lineLeft[i] = down[faceSize - offset - ringSize - (i * size)];
          }

          for (i = 0; i < ringSize; i++) {
            down[offset + i] = lineRight[i];

            down[offset + ringSize - 1 + (i * size)] = lineDown[i];
            down[faceSize - offset - 1 - i] = lineLeft[i];
            down[faceSize - offset - ringSize - (i * size)] = lineUp[i];
          }
        }

      }

      // Slice

      for (i = 0; i < size; i++) {
        lineUp[i] = front[ce + i - (layer * size)];
        lineRight[i] = right[ce + i - (layer * size)];
        lineDown[i] = back[ae - i - (layer * size)];
        lineLeft[i] = left[ce + i - (layer * size)];
      }

      for (i = 0; i < size; i++) {
        front[ce + i - (layer * size)] = lineRight[i];
        right[ce + i - (layer * size)] = lineDown[i];
        back[ae - i - (layer * size)] = lineLeft[i];
        left[ce + i - (layer * size)] = lineUp[i];
      }



      // Twist
    } else if (dir == 2) {

      // Face
      if (layer == 0) {
        holdC = cubieCorners[2].index;
        cubieCorners[2].index = cubieCorners[7].index;
        cubieCorners[7].index = holdC;
        holdC = cubieCorners[3].index;
        cubieCorners[3].index = cubieCorners[6].index;
        cubieCorners[6].index = holdC;


        for (ring = 0; ring < size / 2; ring++) {
          ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          offset = ring * (size + 1);
          ringFaceSize = ringSize * ringSize;


          for (i = 0; i < ringSize; i++) {
            lineUp[i] = down[offset + i];
            lineRight[i] = down[offset + ringSize - 1 + (i * size)];
            lineDown[i] = down[faceSize - offset - 1 - i];
            lineLeft[i] = down[faceSize - offset - ringSize - (i * size)];
          }

          for (i = 0; i < ringSize; i++) {
            down[offset + i] = lineDown[i];

            down[offset + ringSize - 1 + (i * size)] = lineLeft[i];
            down[faceSize - offset - 1 - i] = lineUp[i];
            down[faceSize - offset - ringSize - (i * size)] = lineRight[i];
          }
        }

      }

      // Slice

      for (i = 0; i < size; i++) {
        lineUp[i] = front[ce + i - (layer * size)];
        lineRight[i] = right[ce + i - (layer * size)];
        lineDown[i] = back[ae - i - (layer * size)];
        lineLeft[i] = left[ce + i - (layer * size)];
      }

      for (i = 0; i < size; i++) {
        front[ce + i - (layer * size)] = lineDown[i];
        right[ce + i - (layer * size)] = lineLeft[i];
        back[ae - i - (layer * size)] = lineUp[i];
        left[ce + i - (layer * size)] = lineRight[i];
      }
    }


    break;
  }

  delete[](buff);
}





//----------------------------------------------------------------------------------------------------
std::vector<RubikActionSingle> RubikCube::Randomize(unsigned int steps, unsigned int seed) {
  std::random_device rd;
  std::mt19937 gen(seed ? seed : rd());
  std::uniform_int_distribution<> faceRand(0, 5);
  std::uniform_int_distribution<> directionRand(0, 2);
  std::uniform_int_distribution<> layerRand(0, size - 2);

  std::vector<RubikActionSingle> stepList;

  for (int i = 0; i < steps; i++) {
    unsigned char face = faceRand(gen);
    unsigned char dir = directionRand(gen);
    unsigned short layer = layerRand(gen);
    if (layer == (size - 1) / 2)
      layer = 0;

    switch (face) {
    case 0:
      face = 'f';
      break;
    case 1:
      face = 'b';
      break;
    case 2:
      face = 'u';
      break;
    case 3:
      face = 'd';
      break;
    case 4:
      face = 'l';
      break;
    default:
      face = 'r';
      break;
    }

    if (dir == 0)
      dir = 'c';
    else if (dir == 1)
      dir = 'a';
    else
      dir = 't';



    Rotate(face, dir, layer);
    stepList.push_back({face, dir, layer});
  }

  return stepList;
}





//----------------------------------------------------------------------------------------------------
std::string RubikCube::AsString() {
  std::string str = std::string();
  for (int i = 0; i < faceSize; i++) {
    if (i % size == 0)
      str.insert(str.end(), size * 2 + 1, ' ');
    str += back[i] + 48;

    if ((i + 1) % size == 0)
      str += '\n';
    else
      str += ' ';
  }
  str += '\n';



  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      str += left[i * size + j] + 48;
      str += ' ';
    }
    str += ' ';
    for (int j = 0; j < size; j++) {
      str += down[i * size + j] + 48;
      str += ' ';
    }
    str += ' ';
    for (int j = 0; j < size; j++) {
      str += right[i * size + j] + 48;
      str += ' ';
    }

    str += '\n';
  }
  str += '\n';



  for (int i = 0; i < faceSize; i++) {
    if (i % size == 0)
      str.insert(str.end(), size * 2 + 1, ' ');
    str += front[i] + 48;

    if ((i + 1) % size == 0)
      str += '\n';
    else
      str += ' ';
  }
  str += '\n';

  for (int i = 0; i < faceSize; i++) {
    if (i % size == 0)
      str.insert(str.end(), size * 2 + 1, ' ');
    str += up[i] + 48;

    if ((i + 1) % size == 0)
      str += '\n';
    else
      str += ' ';
  }


  return str;
}





//----------------------------------------------------------------------------------------------------
uint8_t* RubikCube::Copy() {
  uint8_t* cpy = new uint8_t[cubeSize];
  memcpy(cpy, cubiePlanes, faceSize * 6);
  return cpy;
}





//----------------------------------------------------------------------------------------------------
bool RubikCube::operator ==(RubikCube& cube) {
  if (size != cube.size)
    return false;

  for (unsigned int i = 0; i < cubeSize; i++) {
    if (cubiePlanes[i] != cube.cubiePlanes[i])
      return false;
  }
  return true;
}





//----------------------------------------------------------------------------------------------------
void RubikCube::Reset() {
  for (int i = 0; i < faceSize; i++) {
    front[i] = 4;
    back[i] = 0;
    left[i] = 1;
    right[i] = 3;
    down[i] = 2;
    up[i] = 5;
  }
}





//----------------------------------------------------------------------------------------------------
size_t RubikCube::CornerIndex() {
  size_t posIndex = 0;
  size_t orIndex = 0;

  size_t orMul = 1;
  size_t posMul = 1;
  uint8_t i = 0, j = 0, a = 0;

  for (i = 0; i < 7; i++) {
    orIndex += orMul * cubieCorners[i].shiftFrom(cornerRotationStillList[cubieCorners[i].index]);

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
}





//----------------------------------------------------------------------------------------------------
RubikCube::RubikCube(unsigned short Size, uint8_t* planes) : size(Size), faceSize(Size * Size), cubeSize(6 * Size * Size),
edgeCount(0), edgeFirstBoundry(0), cubieCorners(nullptr),
cubiePlanes(nullptr), front(nullptr), back(nullptr), left(nullptr), right(nullptr), down(nullptr),
up(nullptr), edges(nullptr), centerLine(-1), re(size - 1), ce(size * size - size), ae(size * size - 1) {

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


  cubieCorners = new CubeCorner[8]{
    CubeCorner(left + re, up + ce, front, 0),
    CubeCorner(up + ae, right, front + re, 1),
    CubeCorner(left + ae, front + ce, down, 2),
    CubeCorner(front + ae, right + ce, down + re, 3),
    CubeCorner(up, left, back, 4),
    CubeCorner(right + re, up + re, back + re, 5),
    CubeCorner(back + ce, left + ce, down + ce, 6),
    CubeCorner(right + ae, back + ae, down + ae, 7)
  };


  if (size > 2) {
    edgeCount = 12 * (size - 2) * (size - 2);
    edges = new CubeEdge[edgeCount];
    edgeFirstBoundry = edgeCount / 2;

    unsigned int ei = 0;
    for (unsigned int i = 1; i <= size - 2; i++) {
      unsigned int ri = i * size;

      edges[ei] = CubeEdge(front + i, up + ce + i);
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
      //uint8_t j = i % 4;

      back[i] = 0;
      left[i] = 1;
      down[i] = 2;
      right[i] = 3;
      front[i] = 4;
      up[i] = 5;
    }
  }
}





//----------------------------------------------------------------------------------------------------
bool ValidRubikAction(RubikActionSingle prevNode, RubikActionSingle cur) {
  if (prevNode.face == cur.face)
    return false;

  if (prevNode.face == 'b' && cur.face == 'f')
    return false;
  if (prevNode.face == 'l' && cur.face == 'r')
    return false;
  if (prevNode.face == 'd' && cur.face == 'u')
    return false;

  return true;
}





//----------------------------------------------------------------------------------------------------
void GenerateHeuristicDatabases() {
  CornerCube cube;

  HeuristicPair* db = new HeuristicPair[44089920];
  RubikQueue queue("queueSwapFile");
  RubikNode* searchNode = nullptr;
  size_t score = 0;
  size_t prevCount = 0;
  unsigned short disPOne = 0;
  uint8_t maxDepth = 0;
  int8_t i = 0, farBad = -1, depth = 0, depI = 0;

  uint8_t* cubeCheck1 = new uint8_t[24];
  CubeCorner* cubeCorner1 = new CubeCorner[8];

  uint8_t* cubeCheck2 = new uint8_t[24];
  CubeCorner* cubeCorner2 = new CubeCorner[8];




  std::chrono::time_point start = std::chrono::steady_clock::now();


  std::pair<RubikNode*, size_t> search;

  std::vector<RubikActionSingle> depthActions;
  std::vector<RubikActionSingle> rubikHistoryList;
  std::vector<RubikActionSingle> rubikPrevList = std::vector<RubikActionSingle>(12);

  std::vector<RubikActionSingle> actionList = {
    {b, c, 0},
    {b, a, 0},
    {b, t, 0},

    {l, c, 0},
    {l, a, 0},
    {l, t, 0},

    {d, c, 0},
    {d, a, 0},
    {d, t, 0},

    {r, c, 0},
    {r, a, 0},
    {r, t, 0},

    {f, c, 0},
    {f, a, 0},
    {f, t, 0},

    {u, c, 0},
    {u, a, 0},
    {u, t, 0},
  };

  RubikNode n;
  RubikNode* tempNode;



  queue.Push(RubikNodeCreate(1, b, c, true, 0), -1);
  queue.Push(RubikNodeCreate(1, b, a, true, 0), -1);
  queue.Push(RubikNodeCreate(1, b, t, true, 0), -1);

  queue.Push(RubikNodeCreate(1, l, c, true, 0), -1);
  queue.Push(RubikNodeCreate(1, l, a, true, 0), -1);
  queue.Push(RubikNodeCreate(1, l, t, true, 0), -1);

  queue.Push(RubikNodeCreate(1, d, c, true, 0), -1);
  queue.Push(RubikNodeCreate(1, d, a, true, 0), -1);
  queue.Push(RubikNodeCreate(1, d, t, true, 0), -1);

  queue.Push(RubikNodeCreate(1, r, c, true, 0), -1);
  queue.Push(RubikNodeCreate(1, r, a, true, 0), -1);
  queue.Push(RubikNodeCreate(1, r, t, true, 0), -1);

  queue.Push(RubikNodeCreate(1, f, c, true, 0), -1);
  queue.Push(RubikNodeCreate(1, f, a, true, 0), -1);
  queue.Push(RubikNodeCreate(1, f, t, true, 0), -1);

  queue.Push(RubikNodeCreate(1, u, c, true, 0), -1);
  queue.Push(RubikNodeCreate(1, u, a, true, 0), -1);
  queue.Push(RubikNodeCreate(1, u, t, true, 0), -1);



  db[0].Set(0, 0);


  cube.Rotate(b, c);
  score = cube.CornerIndex();
  db[score >> 1].Set(score, 1);
  cube.Rotate(b, a);

  cube.Rotate(b, a);
  score = cube.CornerIndex();
  db[score >> 1].Set(score, 1);
  cube.Rotate(b, c);

  cube.Rotate(b, t);
  score = cube.CornerIndex();
  db[score >> 1].Set(score, 1);
  cube.Rotate(b, t);



  cube.Rotate(l, c);
  score = cube.CornerIndex();
  db[score >> 1].Set(score, 1);
  cube.Rotate(l, a);

  cube.Rotate(l, a);
  score = cube.CornerIndex();
  db[score >> 1].Set(score, 1);
  cube.Rotate(l, c);

  cube.Rotate(l, t);
  score = cube.CornerIndex();
  db[score >> 1].Set(score, 1);
  cube.Rotate(l, t);



  cube.Rotate(d, c);
  score = cube.CornerIndex();
  db[score >> 1].Set(score, 1);
  cube.Rotate(d, a);

  cube.Rotate(d, a);
  score = cube.CornerIndex();
  db[score >> 1].Set(score, 1);
  cube.Rotate(d, c);

  cube.Rotate(d, t);
  score = cube.CornerIndex();
  db[score >> 1].Set(score, 1);
  cube.Rotate(d, t);



  cube.Rotate(r, c);
  score = cube.CornerIndex();
  db[score >> 1].Set(score, 1);
  cube.Rotate(r, a);

  cube.Rotate(r, a);
  score = cube.CornerIndex();
  db[score >> 1].Set(score, 1);
  cube.Rotate(r, c);

  cube.Rotate(r, t);
  score = cube.CornerIndex();
  db[score >> 1].Set(score, 1);
  cube.Rotate(r, t);



  cube.Rotate(f, c);
  score = cube.CornerIndex();
  db[score >> 1].Set(score, 1);
  cube.Rotate(f, a);

  cube.Rotate(f, a);
  score = cube.CornerIndex();
  db[score >> 1].Set(score, 1);
  cube.Rotate(f, c);

  cube.Rotate(f, t);
  score = cube.CornerIndex();
  db[score >> 1].Set(score, 1);
  cube.Rotate(f, t);



  cube.Rotate(u, c);
  score = cube.CornerIndex();
  db[score >> 1].Set(score, 1);
  cube.Rotate(u, a);

  cube.Rotate(u, a);
  score = cube.CornerIndex();
  db[score >> 1].Set(score, 1);
  cube.Rotate(u, c);

  cube.Rotate(u, t);
  score = cube.CornerIndex();
  db[score >> 1].Set(score, 1);
  cube.Rotate(u, t);


  cube.MakeCopy(cubeCheck1, cubeCorner1);


  std::cout << "Starting depth at 0\n";
  while (!queue.Empty() && maxDepth < 12) {
    search = queue.Pop();
    n = *search.first;
    tempNode = search.first;


    if (n.distance > maxDepth) [[unlikely]] {
      maxDepth = n.distance;
      std::cout << "Max depth at "  << (int)maxDepth << 
        " : " << queue.list.nodeNumberNext <<
        " : " << queue.list.nodeNumberNext - prevCount <<
        " : " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() << "ms\n";
      prevCount = queue.list.nodeNumberNext;
    } else
      cube.SetTo(cubeCheck1, cubeCorner1);



    if (tempNode->prevNode != -1) [[likely]] {
      i = 0, farBad = -1, depth = depthActions.size() - 2;

      if (maxDepth > depth + 2)
        depth++;
      depI = depth;

      while (tempNode->prevNode != -1) {
        tempNode = queue.list.Find(tempNode->prevNode);

        rubikPrevList[i] = tempNode->DecomposeAction();

        if (rubikPrevList[i] != depthActions[depI]) {
          farBad = depI;
        }

        i++;
        depI = depth - i;
      }

      if (farBad == -1)
        goto continueStep;


      for (i = depth; i >= farBad; i--)
        cube.Rotate(depthActions[i].face, depthActions[i].ReversedDirection());

      farBad = depth - farBad;

      for (i = farBad; i >= 0; i--) {
        cube.Rotate(rubikPrevList[i].face, rubikPrevList[i].direction);
        depthActions[depth - i] = rubikPrevList[i];
      }

      cube.MakeCopy(cubeCheck1, cubeCorner1);
    }
    continueStep:

    
    cube.Rotate(n.Face(), n.Direction());

    if (n.distance <= depthActions.size()) [[likely]]
      depthActions[n.distance - 1] = n.DecomposeAction();
    else [[unlikely]]
      depthActions.push_back(n.DecomposeAction());


    disPOne = n.distance + 1;
    cube.MakeCopy(cubeCheck2, cubeCorner2);

    for (const auto& action : actionList) {
      if (depthActions[n.distance - 1].face != action.face) {
        cube.Rotate(action.face, action.direction);
        score = cube.CornerIndex();
        if (db[score >> 1].SetIfEmpty(score, disPOne)) {
          queue.Push(RubikNodeCreate(disPOne, action.face, action.direction, true, 0), search.second);
        }

        cube.SetTo(cubeCheck2, cubeCorner2);
      }
    }
  }



  size_t sum = 0;
  size_t highNum = 0;
  size_t lowNum = -1;
  size_t highIndex = 0;
  size_t filled = 0;
  size_t empty = 0;
  for (size_t i = 0; i < 44089920; i++) {
    if (db[i].First() != 0xf) {
      if (highNum < db[i].First())
        highNum = db[i].First();
      if (lowNum > db[i].First())
        lowNum = db[i].First();
      sum += db[i].First();

      highIndex = i << 1;
      filled++;
    } else
      empty++;


    if (db[i].Second() != 0xf) {
      if (highNum < db[i].Second())
        highNum = db[i].Second();
      if (lowNum > db[i].Second())
        lowNum = db[i].Second();
      sum += db[i].Second();

      highIndex = (i << 1) + 1;
      filled++;
    } else
      empty++;
  }


  std::cout << "Sum : " << sum << '\n';
  std::cout << "Average : " << (double)sum / 88179840.0 << '\n';
  std::cout << "Nodes generated : " << queue.list.nodeNumberNext << '\n';
  std::cout << "High number : " << highNum << '\n';
  std::cout << "Low number : " << lowNum << '\n';
  std::cout << "High index : " << highIndex << '\n';
  std::cout << "Filled : " << filled << '\n';
  std::cout << "Empty : " << empty << '\n';
  std::cout << "Total Time : " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() << "ms\n";

  delete[](cubeCorner1);
  delete[](cubeCheck1);
  delete[](cubeCorner2);
  delete[](cubeCheck2);
  delete[](db);
}





//----------------------------------------------------------------------------------------------------
HeuristicPair* GetCornerHeuristicDatabase() {
  HeuristicPair* db = (HeuristicPair*)calloc(44089920, sizeof(HeuristicPair));

  if (!std::filesystem::exists(patternHeuristicLocation))
    GenerateHeuristicDatabases();


  std::fstream str(patternHeuristicLocation, std::ios::binary | std::ios::in);
  str.read((char*)db, 44089920);
  str.close();
  return db;
}