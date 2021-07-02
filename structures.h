#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <string>
#include <vector>
using namespace std;

// from rsmtAware.h
struct EdgeS {
  int row;
  int col;
};

// from compress.h
struct EdgeSupply {
  int row; // upSupply
  int col; // leftSupply
};
struct GridSupply {
  int h;
  int v;
};
struct Pos {
  int row;
  int col;
};

struct Route2D {
  Pos sIdx;
  Pos eIdx;
};
struct Pin {
  Pos locate;
  int lay;
};
struct Net2D {
  vector<Pos> pin2Ds;
  vector<Pin> pinWithLay;
  string name;
  float weight;
  vector<Route2D> route2Ds;
  int minRouteLay;
};

struct TwoPinRoute2D {
  Pos sPin;
  Pos ePin;
  string name;
  float weight;
  vector<Route2D> route;
};
struct TwoPinNets {
  string name;
  vector<TwoPinRoute2D> net;
  vector<vector<GridSupply>> usage;
  vector<vector<EdgeS>> skeleton;
};

struct BLMRgrid {
  Pos further;
  int len;
  int cost;
  bool isBLC;
  bool isFixed;
};
#endif
