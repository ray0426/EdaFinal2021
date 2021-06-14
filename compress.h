#ifndef COMPRESS_H
#define COMPRESS_H

// #include <iostream>
#include <vector>
#include <string>
#include "problem.h"
using namespace std;

struct EdgeSupply {
    int row;  //upSupply
    int col;  //leftSupply
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
struct Net2D {
    vector<Pos> pin2Ds;
    string name;
    float weight;
    vector<Route2D> route2Ds;
};

struct TwoPinRoute2D {
    Pos sPin;
    Pos ePin;
    string name;
    float weight;
    vector<Route2D> route;
};

vector<vector<GridSupply>>* GenerateGridSupplyGraph(Problem *pro);
vector<vector<EdgeSupply>>* Grid2EdgeSupply(vector<vector<GridSupply>> *graph);
vector<Net2D> Three2Two(Problem *pro);
TwoPinRoute2D Multi2TwoPinRoute(Net2D *net, Pos sPin, Pos ePin);

#endif
