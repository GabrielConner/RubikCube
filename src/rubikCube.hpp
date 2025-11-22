#ifndef RUBIK_CUBE_SRC_RUBIK_CUBE_H
#define RUBIK_CUBE_SRC_RUBIK_CUBE_H

#include <cstdint>
#include <string>
#include "helperClasses.hpp"
#include <map>
#include <deque>
#include <vector>



class RubikCube {
public:
  struct CubeEdge {
    uint8_t* f1;
    uint8_t* f2;

    size_t Score() {
      return (*f1) + (2 * (*f2));
    }
  };


  static StillCubeCorner cornerStillList[8];



  unsigned short size;
  unsigned short ae;
  unsigned short re;
  unsigned short ce;


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

  
  CubeCorner* corners;
  CubeEdge* edges;

  short centerLine;




  size_t CornerIndex();





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


  RubikCube(unsigned short Size, uint8_t* planes = nullptr);

  ~RubikCube() {
    if (cubiePlanes)
      delete[](cubiePlanes);
    if (edges)
      delete[](edges);
    if (corners)
      delete[](corners);
  }
};

#endif