#include "rubikCube.h"

#include <random>
#include <ctime>
#include <string>

#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <deque>
#include <vector>

bool ValidRubikAction(RubikActionSingle prevAction, RubikActionSingle cur);


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
  RubikStack stack;

  RubikNodeList list;

  //stack.Push(RubikNodeCreate(0, 0, 0, 0, false), -1, list);

  RubikNode pNode = {};
  RubikNode pPrevNode = {};

  bool validPNode = false;
  bool validPPrevNode = false;

  bool validPNodeAction = false;

  unsigned short curDistance = 1;


  size_t node = 0;
  size_t prevNode = -1;
  size_t prevAction = -1;


  size_t score = 0;

  bool found = false;

  size_t minAbove = 1;



  while (!found && maxDistance < 10) {

    if (stack.empty) {
      rubik->Reset();
      list.Clear();

      prevNode = -1;
      node = -1;// list.GetNext(RubikNode(-1, -1, RubikNodeCreate(0, 0, 0, 0, false)));
      validPNode = false;// list.Find(node);
      validPNodeAction = false;
      curDistance = 1;
      validPPrevNode = false;
      
      maxDistance = minAbove;

      std::cout << "Searching depth : " << maxDistance << '\n';

      minAbove = -1;
    } else {
      node = stack.topNode;
      pNode = *list.Find(node);
      validPNode = true;
      stack.Pop(list);

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

          if (aNode->prevAction != -1)
            aNode = list.Find(aNode->prevAction);
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
        prevAction = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 4, 0, true, i), node, list);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('f', 'a', i);
      }


      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'f', 'a', i})) {
        rubik->Rotate('f', 'a', i);
        prevAction = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 4, 1, true, i), node, list);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('f', 'c', i);
      }


      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'f', 't', i})) {
        rubik->Rotate('f', 't', i);
        prevAction = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 4, 2, true, i), node, list);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('f', 't', i);
      }


      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'b', 'c', i})) {
        rubik->Rotate('b', 'c', i);
        prevAction = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 0, 0, true, i), node, list);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('b', 'a', i);
      }



      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'b', 'a', i})) {
        rubik->Rotate('b', 'a', i);
        prevAction = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 0, 1, true, i), node, list);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('b', 'c', i);
      }



      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'b', 't', i})) {
        rubik->Rotate('b', 't', i);
        prevAction = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 0, 2, true, i), node, list);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('b', 't', i);
      }



      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'l', 'c', i})) {
        rubik->Rotate('l', 'c', i);
        prevAction = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 1, 0, true, i), node, list);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('l', 'a', i);
      }



      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'l', 'a', i})) {
        rubik->Rotate('l', 'a', i);
        prevAction = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 1, 1, true, i), node, list);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('l', 'c', i);
      }



      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'l', 't', i})) {
        rubik->Rotate('l', 't', i);
        prevAction = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 1, 2, true, i), node, list);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('l', 't', i);
      }



      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'r', 'c', i})) {
        rubik->Rotate('r', 'c', i);
        prevAction = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 3, 0, true, i), node, list);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('r', 'a', i);
      }



      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'r', 'a', i})) {
        rubik->Rotate('r', 'a', i);
        prevAction = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 3, 1, true, i), node, list);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('r', 'c', i);
      }



      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'r', 't', i})) {
        rubik->Rotate('r', 't', i);
        prevAction = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 3, 2, true, i), node, list);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('r', 't', i);
      }



      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'u', 'c', i})) {
        rubik->Rotate('u', 'c', i);
        prevAction = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 5, 0, true, i), node, list);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('u', 'a', i);
      }



      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'u', 'a', i})) {
        rubik->Rotate('u', 'a', i);
        prevAction = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 5, 1, true, i), node, list);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('u', 'c', i);
      }



      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'u', 't', i})) {
        rubik->Rotate('u', 't', i);
        prevAction = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 5, 2, true, i), node, list);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('u', 't', i);
      }



      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'d', 'c', i})) {
        rubik->Rotate('d', 'c', i);
        prevAction = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 2, 0, true, i), node, list);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('d', 'a', i);
      }



      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'d', 'a', i}))  {
        rubik->Rotate('d', 'a', i);
        prevAction = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 2, 1, true, i), node, list);
        if (*rubik == *this) {
          found = true;
          break;
        }
        rubik->Rotate('d', 'c', i);
      }



      if (!validPNodeAction || ValidRubikAction(pNode.DecomposeAction(), {'d', 't', i})) {
        rubik->Rotate('d', 't', i);
        prevAction = stack.Push(RubikNodeCreate(MaxScoreOf(curDistance, cornerDB, firstEdgeDB, secondEdgeDB), 2, 2, true, i), node, list);
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
    RubikNode* aNode = list.Find(prevAction);
    while (aNode != nullptr && aNode->ValidAction()) {
      unsigned char dir = aNode->NormDirection();
      if (dir == 'c')
        dir = 'a';
      else if (dir == 'a')
        dir = 'c';

      ret.push_back({aNode->NormFace(), dir, aNode->layer});
      if (aNode->prevAction != -1)
        aNode = list.Find(aNode->prevAction);
      else
        aNode = nullptr;
    }
  }

  _CrtMemState state1, state2, diff;
  _CrtMemCheckpoint(&state1);

  //std::cout << "Ending cornerDB size : " << cornerDB.size() << '\n';
  //std::cout << "Ending firstEdgeDB size : " << firstEdgeDB.size() << '\n';
  //std::cout << "Ending secondEdgeDB size : " << secondEdgeDB.size() << '\n';
  list.Clear();

  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);

  _CrtMemCheckpoint(&state2);
  if (_CrtMemDifference(&diff, &state1, &state2))
    _CrtMemDumpStatistics(&diff);

  return ret;
}




//----------------------------------------------------------------------------------------------------
unsigned int RubikCube::MaxScoreOf(unsigned short distance, std::map<size_t, uint8_t>& corners, std::map<size_t, uint8_t>& firstEdges, std::map<size_t, uint8_t>& secondEdges) {
  //size_t cornerIndex = CornerIndex();
  //size_t firstEdgeIndex = FirstEdgeIndex();
  //size_t secondEdgeIndex = SecondEdgeIndex();

  //unsigned int cornerVal, firstEdgeVal, secondEdgeVal;
  //cornerVal = firstEdgeVal = secondEdgeVal = 0;

  //std::map<size_t, uint8_t>::iterator search;


  //search = corners.find(cornerIndex);
  //if (search == corners.end()) {
  //  corners.insert({cornerIndex, distance});
  //  cornerVal = distance;
  //} else {
  //  if (search->second > distance)
  //    search->second = distance;
  //  cornerVal = distance;
  //}


  //search = firstEdges.find(firstEdgeIndex);
  //if (search == firstEdges.end()) {
  //  firstEdges.insert({firstEdgeIndex, distance});
  //  firstEdgeVal = distance;
  //} else {
  //  if (search->second > distance)
  //    search->second = distance;
  //  firstEdgeVal = distance;
  //}


  //search = secondEdges.find(secondEdgeIndex);
  //if (search == secondEdges.end()) {
  //  secondEdges.insert({secondEdgeIndex, distance});
  //  secondEdgeVal = distance;
  //} else {
  //  if (search->second > distance)
  //    search->second = distance;
  //  secondEdgeVal = distance;
  //}


  //return std::max({cornerVal, firstEdgeVal, secondEdgeVal});
  return distance;
}





//----------------------------------------------------------------------------------------------------
void RubikCube::Rotate(unsigned char face, unsigned char dir, unsigned short layer) {
  if (size <= 1 || layer >= size)
    return;

  if (face == 'f' && layer >= size - 1) {
    face = 'b';
    if (dir != 't')
      dir = dir == 'c' ? 'a' : 'c';
    layer = 0;
  } else if (face == 'b' && layer >= size - 1) {
    face = 'f';
    if (dir != 't')
      dir = dir == 'c' ? 'a' : 'c';
    layer = 0;
  } else if (face == 'l' && layer >= size - 1) {
    face = 'r';
    if (dir != 't')
      dir = dir == 'c' ? 'a' : 'c';
    layer = 0;
  } else if (face == 'r' && layer >= size - 1) {
    face = 'l';
    if (dir != 't')
      dir = dir == 'c' ? 'a' : 'c';
    layer = 0;
  } else if (face == 'u' && layer >= size - 1) {
    face = 'd';
    if (dir != 't')
      dir = dir == 'c' ? 'a' : 'c';
    layer = 0;
  } else if (face == 'd' && layer >= size - 1) {
    face = 'u';
    if (dir != 't')
      dir = dir == 'c' ? 'a' : 'c';
    layer = 0;
  }

  uint8_t* lineUp = new uint8_t[size];
  uint8_t* lineRight = new uint8_t[size];
  uint8_t* lineDown = new uint8_t[size];
  uint8_t* lineLeft = new uint8_t[size];


  // Front
  if (face == 'f') {

    // Clockwise
    if (dir == 'c') {

      // Face
      if (layer == 0) {
        for (int ring = 0; ring < size / 2; ring++) {
          int ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          int offset = ring * (size + 1);
          int ringFaceSize = ringSize * ringSize;


          for (int i = 0; i < ringSize; i++) {
            lineUp[i] = front[offset + i];
            lineRight[i] = front[offset + ringSize - 1 + (i * size)];
            lineDown[i] = front[faceSize - offset - 1 - i];
            lineLeft[i] = front[faceSize - offset - ringSize - (i * size)];
          }

          for (int i = 0; i < ringSize; i++) {
            front[offset + i] = lineLeft[i];
            if (i == 0)
              continue;

            front[offset + ringSize - 1 + (i * size)] = lineUp[i];
            front[faceSize - offset - 1 - i] = lineRight[i];
            front[faceSize - offset - ringSize - (i * size)] = lineDown[i];
          }
        }

      }

      // Slice

      for (int i = 0; i < size; i++) {
        lineUp[i] = up[size * (size - layer - 1) + i];
        lineRight[i] = right[layer + (i * size)];
        lineDown[i] = down[(layer * size) + size - 1 - i];
        lineLeft[i] = left[(size * size - 1) - layer - (i * size)];
      }

      for (int i = 0; i < size; i++) {
        up[size * (size - layer - 1) + i] = lineLeft[i];
        right[layer + (i * size)] = lineUp[i];
        down[(layer * size) + size - 1 - i] = lineRight[i];
        left[(size * size - 1) - layer - (i * size)] = lineDown[i];
      }



      // Anticlockwise
    } else if (dir == 'a') {

      // Face
      if (layer == 0) {
        for (int ring = 0; ring < size / 2; ring++) {
          int ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          int offset = ring * (size + 1);
          int ringFaceSize = ringSize * ringSize;


          for (int i = 0; i < ringSize; i++) {
            lineUp[i] = front[offset + i];
            lineRight[i] = front[offset + ringSize - 1 + (i * size)];
            lineDown[i] = front[faceSize - offset - 1 - i];
            lineLeft[i] = front[faceSize - offset - ringSize - (i * size)];
          }

          for (int i = 0; i < ringSize; i++) {
            front[offset + i] = lineRight[i];
            if (i == 0)
              continue;

            front[offset + ringSize - 1 + (i * size)] = lineDown[i];
            front[faceSize - offset - 1 - i] = lineLeft[i];
            front[faceSize - offset - ringSize - (i * size)] = lineUp[i];
          }
        }

      }

      // Slice

      for (int i = 0; i < size; i++) {
        lineUp[i] = up[size * (size - layer - 1) + i];
        lineRight[i] = right[layer + (i * size)];
        lineDown[i] = down[(layer * size) + size - 1 - i];
        lineLeft[i] = left[(size * size - 1) - layer - (i * size)];
      }

      for (int i = 0; i < size; i++) {
        up[size * (size - layer - 1) + i] = lineRight[i];
        right[layer + (i * size)] = lineDown[i];
        down[(layer * size) + size - 1 - i] = lineLeft[i];
        left[(size * size - 1) - layer - (i * size)] = lineUp[i];
      }



      // Twist
    } else if (dir == 't') {

      // Face
      if (layer == 0) {
        for (int ring = 0; ring < size / 2; ring++) {
          int ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          int offset = ring * (size + 1);
          int ringFaceSize = ringSize * ringSize;


          for (int i = 0; i < ringSize; i++) {
            lineUp[i] = front[offset + i];
            lineRight[i] = front[offset + ringSize - 1 + (i * size)];
            lineDown[i] = front[faceSize - offset - 1 - i];
            lineLeft[i] = front[faceSize - offset - ringSize - (i * size)];
          }

          for (int i = 0; i < ringSize; i++) {
            front[offset + i] = lineDown[i];
            if (i == 0)
              continue;

            front[offset + ringSize - 1 + (i * size)] = lineLeft[i];
            front[faceSize - offset - 1 - i] = lineUp[i];
            front[faceSize - offset - ringSize - (i * size)] = lineRight[i];
          }
        }

      }

      // Slice

      for (int i = 0; i < size; i++) {
        lineUp[i] = up[size * (size - layer - 1) + i];
        lineRight[i] = right[layer + (i * size)];
        lineDown[i] = down[(layer * size) + size - 1 - i];
        lineLeft[i] = left[(size * size - 1) - layer - (i * size)];
      }

      for (int i = 0; i < size; i++) {
        up[size * (size - layer - 1) + i] = lineDown[i];
        right[layer + (i * size)] = lineLeft[i];
        down[(layer * size) + size - 1 - i] = lineUp[i];
        left[(size * size - 1) - layer - (i * size)] = lineRight[i];
      }
    }



    // Back
  } else if (face == 'b') {

    // Clockwise
    if (dir == 'c') {

      // Face
      if (layer == 0) {
        for (int ring = 0; ring < size / 2; ring++) {
          int ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          int offset = ring * (size + 1);
          int ringFaceSize = ringSize * ringSize;


          for (int i = 0; i < ringSize; i++) {
            lineUp[i] = back[offset + i];
            lineRight[i] = back[offset + ringSize - 1 + (i * size)];
            lineDown[i] = back[faceSize - offset - 1 - i];
            lineLeft[i] = back[faceSize - offset - ringSize - (i * size)];
          }

          for (int i = 0; i < ringSize; i++) {
            back[offset + i] = lineRight[i];
            if (i == 0)
              continue;

            back[offset + ringSize - 1 + (i * size)] = lineDown[i];
            back[faceSize - offset - 1 - i] = lineLeft[i];
            back[faceSize - offset - ringSize - (i * size)] = lineUp[i];
          }
        }

      }

      // Slice

      for (int i = 0; i < size; i++) {
        lineUp[i] = up[size - 1 + (layer * size) - i];
        lineRight[i] = left[layer + (i * size)];
        lineDown[i] = down[size * (size - layer - 1) + i];
        lineLeft[i] = right[size * size - 1 - layer - (i * size)];
      }

      for (int i = 0; i < size; i++) {
        up[size - 1 + (layer * size) - i] = lineLeft[i];
        left[layer + (i * size)] = lineUp[i];
        down[size * (size - layer - 1) + i] = lineRight[i];
        right[size * size - 1 - layer - (i * size)] = lineDown[i];
      }



      // Anticlockwise
    } else if (dir == 'a') {

      // Face
      if (layer == 0) {
        for (int ring = 0; ring < size / 2; ring++) {
          int ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          int offset = ring * (size + 1);
          int ringFaceSize = ringSize * ringSize;


          for (int i = 0; i < ringSize; i++) {
            lineUp[i] = back[offset + i];
            lineRight[i] = back[offset + ringSize - 1 + (i * size)];
            lineDown[i] = back[faceSize - offset - 1 - i];
            lineLeft[i] = back[faceSize - offset - ringSize - (i * size)];
          }

          for (int i = 0; i < ringSize; i++) {
            back[offset + i] = lineLeft[i];
            if (i == 0)
              continue;

            back[offset + ringSize - 1 + (i * size)] = lineUp[i];
            back[faceSize - offset - 1 - i] = lineRight[i];
            back[faceSize - offset - ringSize - (i * size)] = lineDown[i];
          }
        }

      }


      // Slice

      for (int i = 0; i < size; i++) {
        lineUp[i] = up[(size - 1) + (layer * size) - i];
        lineRight[i] = left[layer + (i * size)];
        lineDown[i] = down[size * (size - layer - 1) + i];
        lineLeft[i] = right[size * size - 1 - layer - (i * size)];
      }

      for (int i = 0; i < size; i++) {
        up[(size - 1) + (layer * size) - i] = lineRight[i];
        left[layer + (i * size)] = lineDown[i];
        down[size * (size - layer - 1) + i] = lineLeft[i];
        right[size * size - 1 - layer - (i * size)] = lineUp[i];
      }



      // Twist
    } else if (dir == 't') {

      // Face
      if (layer == 0) {
        for (int ring = 0; ring < size / 2; ring++) {
          int ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          int offset = ring * (size + 1);
          int ringFaceSize = ringSize * ringSize;


          for (int i = 0; i < ringSize; i++) {
            lineUp[i] = back[offset + i];
            lineRight[i] = back[offset + ringSize - 1 + (i * size)];
            lineDown[i] = back[faceSize - offset - 1 - i];
            lineLeft[i] = back[faceSize - offset - ringSize - (i * size)];
          }

          for (int i = 0; i < ringSize; i++) {
            back[offset + i] = lineDown[i];
            if (i == 0)
              continue;

            back[offset + ringSize - 1 + (i * size)] = lineLeft[i];
            back[faceSize - offset - 1 - i] = lineUp[i];
            back[faceSize - offset - ringSize - (i * size)] = lineRight[i];
          }
        }
      }


      // Slice

      for (int i = 0; i < size; i++) {
        lineUp[i] = up[(size - 1) - (layer * size) - i];
        lineRight[i] = left[layer + (i * size)];
        lineDown[i] = down[size * (size - layer - 1) + i];
        lineLeft[i] = right[size * size - 1 - layer - (i * size)];
      }

      for (int i = 0; i < size; i++) {
        up[(size - 1) - (layer * size) - i] = lineDown[i];
        left[layer + (i * size)] = lineLeft[i];
        down[size * (size - layer - 1) + i] = lineUp[i];
        right[size * size - 1 - layer - (i * size)] = lineRight[i];
      }
    }



    // Left
  } else if (face == 'l') {

    // Clockwise
    if (dir == 'c') {

      // Face
      if (layer == 0) {
        for (int ring = 0; ring < size / 2; ring++) {
          int ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          int offset = ring * (size + 1);
          int ringFaceSize = ringSize * ringSize;


          for (int i = 0; i < ringSize; i++) {
            lineUp[i] = left[offset + i];
            lineRight[i] = left[offset + ringSize - 1 + (i * size)];
            lineDown[i] = left[faceSize - offset - 1 - i];
            lineLeft[i] = left[faceSize - offset - ringSize - (i * size)];
          }

          for (int i = 0; i < ringSize; i++) {
            left[offset + i] = lineLeft[i];
            if (i == 0)
              continue;

            left[offset + ringSize - 1 + (i * size)] = lineUp[i];
            left[faceSize - offset - 1 - i] = lineRight[i];
            left[faceSize - offset - ringSize - (i * size)] = lineDown[i];
          }
        }

      }

      // Slice

      for (int i = 0; i < size; i++) {
        lineUp[i] = up[layer + (i * size)];
        lineRight[i] = front[layer + (i * size)];
        lineDown[i] = down[layer + (i * size)];
        lineLeft[i] = back[layer + (i * size)];
      }

      for (int i = 0; i < size; i++) {
        up[layer + (i * size)] = lineLeft[i];
        front[layer + (i * size)] = lineUp[i];
        down[layer + (i * size)] = lineRight[i];
        back[layer + (i * size)] = lineDown[i];
      }

      // Anticlockwise
    } else if (dir == 'a') {

      // Face
      if (layer == 0) {
        for (int ring = 0; ring < size / 2; ring++) {
          int ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          int offset = ring * (size + 1);
          int ringFaceSize = ringSize * ringSize;


          for (int i = 0; i < ringSize; i++) {
            lineUp[i] = left[offset + i];
            lineRight[i] = left[offset + ringSize - 1 + (i * size)];
            lineDown[i] = left[faceSize - offset - 1 - i];
            lineLeft[i] = left[faceSize - offset - ringSize - (i * size)];
          }

          for (int i = 0; i < ringSize; i++) {
            left[offset + i] = lineRight[i];
            if (i == 0)
              continue;

            left[offset + ringSize - 1 + (i * size)] = lineDown[i];
            left[faceSize - offset - 1 - i] = lineLeft[i];
            left[faceSize - offset - ringSize - (i * size)] = lineUp[i];
          }

        }

        // Slice

        for (int i = 0; i < size; i++) {
          lineUp[i] = up[layer + (i * size)];
          lineRight[i] = front[layer + (i * size)];
          lineDown[i] = down[layer + (i * size)];
          lineLeft[i] = back[layer + (i * size)];
        }

        for (int i = 0; i < size; i++) {
          up[layer + (i * size)] = lineRight[i];
          front[layer + (i * size)] = lineDown[i];
          down[layer + (i * size)] = lineLeft[i];
          back[layer + (i * size)] = lineUp[i];
        }
      }



      // Twist
    } else if (dir == 't') {

      // Face
      if (layer == 0) {
        for (int ring = 0; ring < size / 2; ring++) {
          int ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          int offset = ring * (size + 1);
          int ringFaceSize = ringSize * ringSize;


          for (int i = 0; i < ringSize; i++) {
            lineUp[i] = left[offset + i];
            lineRight[i] = left[offset + ringSize - 1 + (i * size)];
            lineDown[i] = left[faceSize - offset - 1 - i];
            lineLeft[i] = left[faceSize - offset - ringSize - (i * size)];
          }

          for (int i = 0; i < ringSize; i++) {
            left[offset + i] = lineDown[i];
            if (i == 0)
              continue;

            left[offset + ringSize - 1 + (i * size)] = lineLeft[i];
            left[faceSize - offset - 1 - i] = lineUp[i];
            left[faceSize - offset - ringSize - (i * size)] = lineRight[i];
          }

        }

        // Slice

        for (int i = 0; i < size; i++) {
          lineUp[i] = up[layer + (i * size)];
          lineRight[i] = front[layer + (i * size)];
          lineDown[i] = down[layer + (i * size)];
          lineLeft[i] = back[layer + (i * size)];
        }

        for (int i = 0; i < size; i++) {
          up[layer + (i * size)] = lineDown[i];
          front[layer + (i * size)] = lineLeft[i];
          down[layer + (i * size)] = lineUp[i];
          back[layer + (i * size)] = lineRight[i];
        }
      }
    }



    // Right
  } else if (face == 'r') {

    // Clockwise
    if (dir == 'c') {

      // Face
      if (layer == 0) {
        for (int ring = 0; ring < size / 2; ring++) {
          int ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          int offset = ring * (size + 1);
          int ringFaceSize = ringSize * ringSize;


          for (int i = 0; i < ringSize; i++) {
            lineUp[i] = right[offset + i];
            lineRight[i] = right[offset + ringSize - 1 + (i * size)];
            lineDown[i] = right[faceSize - offset - 1 - i];
            lineLeft[i] = right[faceSize - offset - ringSize - (i * size)];
          }

          for (int i = 0; i < ringSize; i++) {
            right[offset + i] = lineLeft[i];
            if (i == 0)
              continue;

            right[offset + ringSize - 1 + (i * size)] = lineUp[i];
            right[faceSize - offset - 1 - i] = lineRight[i];
            right[faceSize - offset - ringSize - (i * size)] = lineDown[i];
          }
        }

      }

      // Slice

      for (int i = 0; i < size; i++) {
        lineUp[i] = up[(size - layer - 1) + (i * size)];
        lineRight[i] = back[(size * size - 1) - (i * size) + layer];
        lineDown[i] = down[(size - layer - 1) + (i * size)];
        lineLeft[i] = front[(size - layer - 1) + (i * size)];
      }

      for (int i = 0; i < size; i++) {
        up[(size - layer - 1) + (i * size)] = lineLeft[i];
        back[(size * size - 1) - (i * size) + layer] = lineUp[i];
        down[(size - layer - 1) + (i * size)] = lineRight[i];
        front[(size - layer - 1) + (i * size)] = lineDown[i];
      }



      // Anticlockwise
    } else if (dir == 'a') {

      // Face
      if (layer == 0) {
        for (int ring = 0; ring < size / 2; ring++) {
          int ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          int offset = ring * (size + 1);
          int ringFaceSize = ringSize * ringSize;


          for (int i = 0; i < ringSize; i++) {
            lineUp[i] = right[offset + i];
            lineRight[i] = right[offset + ringSize - 1 + (i * size)];
            lineDown[i] = right[faceSize - offset - 1 - i];
            lineLeft[i] = right[faceSize - offset - ringSize - (i * size)];
          }

          for (int i = 0; i < ringSize; i++) {
            right[offset + i] = lineRight[i];
            if (i == 0)
              continue;

            right[offset + ringSize - 1 + (i * size)] = lineDown[i];
            right[faceSize - offset - 1 - i] = lineLeft[i];
            right[faceSize - offset - ringSize - (i * size)] = lineUp[i];
          }
        }

      }

      // Slice

      for (int i = 0; i < size; i++) {
        lineUp[i] = up[(size - layer - 1) + (i * size)];
        lineRight[i] = back[(size * size - 1) - (i * size) + layer];
        lineDown[i] = down[(size - layer - 1) + (i * size)];
        lineLeft[i] = front[(size - layer - 1) + (i * size)];
      }

      for (int i = 0; i < size; i++) {
        up[(size - layer - 1) + (i * size)] = lineRight[i];
        back[(size * size - 1) - (i * size) + layer] = lineDown[i];
        down[(size - layer - 1) + (i * size)] = lineLeft[i];
        front[(size - layer - 1) + (i * size)] = lineUp[i];
      }



      // Twist
    } else if (dir == 't') {

      // Face
      if (layer == 0) {
        for (int ring = 0; ring < size / 2; ring++) {
          int ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          int offset = ring * (size + 1);
          int ringFaceSize = ringSize * ringSize;


          for (int i = 0; i < ringSize; i++) {
            lineUp[i] = right[offset + i];
            lineRight[i] = right[offset + ringSize - 1 + (i * size)];
            lineDown[i] = right[faceSize - offset - 1 - i];
            lineLeft[i] = right[faceSize - offset - ringSize - (i * size)];
          }

          for (int i = 0; i < ringSize; i++) {
            right[offset + i] = lineDown[i];
            if (i == 0)
              continue;

            right[offset + ringSize - 1 + (i * size)] = lineLeft[i];
            right[faceSize - offset - 1 - i] = lineUp[i];
            right[faceSize - offset - ringSize - (i * size)] = lineRight[i];
          }
        }

      }

      // Slice

      for (int i = 0; i < size; i++) {
        lineUp[i] = up[(size - layer - 1) + (i * size)];
        lineRight[i] = back[(size * size - 1) - (i * size) + layer];
        lineDown[i] = down[(size - layer - 1) + (i * size)];
        lineLeft[i] = front[(size - layer - 1) + (i * size)];
      }

      for (int i = 0; i < size; i++) {
        up[(size - layer - 1) + (i * size)] = lineDown[i];
        back[(size * size - 1) - (i * size) + layer] = lineLeft[i];
        down[(size - layer - 1) + (i * size)] = lineUp[i];
        front[(size - layer - 1) + (i * size)] = lineRight[i];
      }
    }



    // Up
  } else if (face == 'u') {

    // Clockwise
    if (dir == 'c') {

      // Face
      if (layer == 0) {
        for (int ring = 0; ring < size / 2; ring++) {
          int ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          int offset = ring * (size + 1);
          int ringFaceSize = ringSize * ringSize;


          for (int i = 0; i < ringSize; i++) {
            lineUp[i] = up[offset + i];
            lineRight[i] = up[offset + ringSize - 1 + (i * size)];
            lineDown[i] = up[faceSize - offset - 1 - i];
            lineLeft[i] = up[faceSize - offset - ringSize - (i * size)];
          }

          for (int i = 0; i < ringSize; i++) {
            up[offset + i] = lineLeft[i];
            if (i == 0)
              continue;

            up[offset + ringSize - 1 + (i * size)] = lineUp[i];
            up[faceSize - offset - 1 - i] = lineRight[i];
            up[faceSize - offset - ringSize - (i * size)] = lineDown[i];
          }
        }

      }

      // Slice

      for (int i = 0; i < size; i++) {
        lineUp[i] = back[(size * layer) + size - 1 - i];
        lineRight[i] = right[(size * layer) + i];
        lineDown[i] = front[(size * layer) + i];
        lineLeft[i] = left[(size * layer) + i];
      }

      for (int i = 0; i < size; i++) {
        back[(size * layer) + size - 1 - i] = lineLeft[i];
        right[(size * layer) + i] = lineUp[i];
        front[(size * layer) + i] = lineRight[i];
        left[(size * layer) + i] = lineDown[i];
      }


      // Anticlockwise
    } else if (dir == 'a') {

      // Face
      if (layer == 0) {
        for (int ring = 0; ring < size / 2; ring++) {
          int ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          int offset = ring * (size + 1);
          int ringFaceSize = ringSize * ringSize;


          for (int i = 0; i < ringSize; i++) {
            lineUp[i] = up[offset + i];
            lineRight[i] = up[offset + ringSize - 1 + (i * size)];
            lineDown[i] = up[faceSize - offset - 1 - i];
            lineLeft[i] = up[faceSize - offset - ringSize - (i * size)];
          }

          for (int i = 0; i < ringSize; i++) {
            up[offset + i] = lineRight[i];
            if (i == 0)
              continue;

            up[offset + ringSize - 1 + (i * size)] = lineDown[i];
            up[faceSize - offset - 1 - i] = lineLeft[i];
            up[faceSize - offset - ringSize - (i * size)] = lineUp[i];
          }
        }

      }

      // Slice

      for (int i = 0; i < size; i++) {
        lineUp[i] = back[(size * layer) + size - 1 - i];
        lineRight[i] = right[(size * layer) + i];
        lineDown[i] = front[(size * layer) + i];
        lineLeft[i] = left[(size * layer) + i];
      }

      for (int i = 0; i < size; i++) {
        back[(size * layer) + size - 1 - i] = lineRight[i];
        right[(size * layer) + i] = lineDown[i];
        front[(size * layer) + i] = lineLeft[i];
        left[(size * layer) + i] = lineUp[i];
      }



      // Twist
    } else if (dir == 't') {

      // Face
      if (layer == 0) {
        for (int ring = 0; ring < size / 2; ring++) {
          int ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          int offset = ring * (size + 1);
          int ringFaceSize = ringSize * ringSize;


          for (int i = 0; i < ringSize; i++) {
            lineUp[i] = up[offset + i];
            lineRight[i] = up[offset + ringSize - 1 + (i * size)];
            lineDown[i] = up[faceSize - offset - 1 - i];
            lineLeft[i] = up[faceSize - offset - ringSize - (i * size)];
          }

          for (int i = 0; i < ringSize; i++) {
            up[offset + i] = lineDown[i];
            if (i == 0)
              continue;

            up[offset + ringSize - 1 + (i * size)] = lineLeft[i];
            up[faceSize - offset - 1 - i] = lineUp[i];
            up[faceSize - offset - ringSize - (i * size)] = lineRight[i];
          }
        }

      }

      // Slice

      for (int i = 0; i < size; i++) {
        lineUp[i] = back[(size * layer) + size - 1 - i];
        lineRight[i] = right[(size * layer) + i];
        lineDown[i] = front[(size * layer) + i];
        lineLeft[i] = left[(size * layer) + i];
      }

      for (int i = 0; i < size; i++) {
        back[(size * layer) + size - 1 - i] = lineDown[i];
        right[(size * layer) + i] = lineLeft[i];
        front[(size * layer) + i] = lineUp[i];
        left[(size * layer) + i] = lineRight[i];
      }
    }



    // Down
  } else if (face == 'd') {

    // Clockwise
    if (dir == 'c') {

      // Face
      if (layer == 0) {
        for (int ring = 0; ring < size / 2; ring++) {
          int ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          int offset = ring * (size + 1);
          int ringFaceSize = ringSize * ringSize;


          for (int i = 0; i < ringSize; i++) {
            lineUp[i] = down[offset + i];
            lineRight[i] = down[offset + ringSize - 1 + (i * size)];
            lineDown[i] = down[faceSize - offset - 1 - i];
            lineLeft[i] = down[faceSize - offset - ringSize - (i * size)];
          }

          for (int i = 0; i < ringSize; i++) {
            down[offset + i] = lineLeft[i];
            if (i == 0)
              continue;

            down[offset + ringSize - 1 + (i * size)] = lineUp[i];
            down[faceSize - offset - 1 - i] = lineRight[i];
            down[faceSize - offset - ringSize - (i * size)] = lineDown[i];
          }
        }

      }

      // Slice

      for (int i = 0; i < size; i++) {
        lineUp[i] = front[(size - 1) * size + i - (layer * size)];
        lineRight[i] = right[(size - 1) * size + i - (layer * size)];
        lineDown[i] = back[(size * size - 1) - i - (layer * size)];
        lineLeft[i] = left[(size - 1) * size + i - (layer * size)];
      }

      for (int i = 0; i < size; i++) {
        front[(size - 1) * size + i - (layer * size)] = lineLeft[i];
        right[(size - 1) * size + i - (layer * size)] = lineUp[i];
        back[(size * size - 1) - i - (layer * size)] = lineRight[i];
        left[(size - 1) * size + i - (layer * size)] = lineDown[i];
      }



      // Anticlockwise
    } else if (dir == 'a') {

      // Face
      if (layer == 0) {
        for (int ring = 0; ring < size / 2; ring++) {
          int ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          int offset = ring * (size + 1);
          int ringFaceSize = ringSize * ringSize;


          for (int i = 0; i < ringSize; i++) {
            lineUp[i] = down[offset + i];
            lineRight[i] = down[offset + ringSize - 1 + (i * size)];
            lineDown[i] = down[faceSize - offset - 1 - i];
            lineLeft[i] = down[faceSize - offset - ringSize - (i * size)];
          }

          for (int i = 0; i < ringSize; i++) {
            down[offset + i] = lineRight[i];
            if (i == 0)
              continue;

            down[offset + ringSize - 1 + (i * size)] = lineDown[i];
            down[faceSize - offset - 1 - i] = lineLeft[i];
            down[faceSize - offset - ringSize - (i * size)] = lineUp[i];
          }
        }

      }

      // Slice

      for (int i = 0; i < size; i++) {
        lineUp[i] = front[(size - 1) * size + i - (layer * size)];
        lineRight[i] = right[(size - 1) * size + i - (layer * size)];
        lineDown[i] = back[(size * size - 1) - i - (layer * size)];
        lineLeft[i] = left[(size - 1) * size + i - (layer * size)];
      }

      for (int i = 0; i < size; i++) {
        front[(size - 1) * size + i - (layer * size)] = lineRight[i];
        right[(size - 1) * size + i - (layer * size)] = lineDown[i];
        back[(size * size - 1) - i - (layer * size)] = lineLeft[i];
        left[(size - 1) * size + i - (layer * size)] = lineUp[i];
      }



      // Twist
    } else if (dir == 't') {

      // Face
      if (layer == 0) {
        for (int ring = 0; ring < size / 2; ring++) {
          int ringSize = size - (ring * 2);
          if (ringSize == 1)
            break;

          int offset = ring * (size + 1);
          int ringFaceSize = ringSize * ringSize;


          for (int i = 0; i < ringSize; i++) {
            lineUp[i] = down[offset + i];
            lineRight[i] = down[offset + ringSize - 1 + (i * size)];
            lineDown[i] = down[faceSize - offset - 1 - i];
            lineLeft[i] = down[faceSize - offset - ringSize - (i * size)];
          }

          for (int i = 0; i < ringSize; i++) {
            down[offset + i] = lineDown[i];
            if (i == 0)
              continue;

            down[offset + ringSize - 1 + (i * size)] = lineLeft[i];
            down[faceSize - offset - 1 - i] = lineUp[i];
            down[faceSize - offset - ringSize - (i * size)] = lineRight[i];
          }
        }

      }

      // Slice

      for (int i = 0; i < size; i++) {
        lineUp[i] = front[(size - 1) * size + i - (layer * size)];
        lineRight[i] = right[(size - 1) * size + i - (layer * size)];
        lineDown[i] = back[(size * size - 1) - i - (layer * size)];
        lineLeft[i] = left[(size - 1) * size + i - (layer * size)];
      }

      for (int i = 0; i < size; i++) {
        front[(size - 1) * size + i - (layer * size)] = lineDown[i];
        right[(size - 1) * size + i - (layer * size)] = lineLeft[i];
        back[(size * size - 1) - i - (layer * size)] = lineUp[i];
        left[(size - 1) * size + i - (layer * size)] = lineRight[i];
      }


    }
  }

  delete[](lineUp);
  delete[](lineRight);
  delete[](lineDown);
  delete[](lineLeft);
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



bool ValidRubikAction(RubikActionSingle prevAction, RubikActionSingle cur) {
  if (prevAction.face == cur.face)
    return false;

  if (prevAction.face == 'b' && cur.face == 'f')
    return false;
  if (prevAction.face == 'l' && cur.face == 'r')
    return false;
  if (prevAction.face == 'd' && cur.face == 'u')
    return false;

  return true;
}