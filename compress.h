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

struct TwoPinNets {
    vector<TwoPinRoute2D> net;
    vector<vector<GridSupply>> usage;
};

struct TwoPinRoute2D {
    Pos sPin;
    Pos ePin;
    string name;
    float weight;
    vector<Route2D> route;
};

void PrintEdgeSupply(EdgeSupply a);
void PrintGridSupply(GridSupply a);
void PrintPos(Pos a);
void PrintRoute2D(Route2D a);
void PrintNet2D(Net2D a);
void PrintTwoPinNet(TwoPinRoute2D a);

vector<vector<GridSupply>> GenerateGridSupplyGraph(Problem *pro);
vector<vector<EdgeSupply>> Grid2EdgeSupply(vector<vector<GridSupply>>& graph);
void SupplyChange(vector<vector<GridSupply>>& graph, Net2D newNet, Net2D oldNet = {});

vector<Net2D> Three2Two(Problem *pro);
TwoPinRoute2D Multi2TwoPinRoute(Net2D& net, Pos sPin, Pos ePin);
void SortTaskQueue(vector<TwoPinRoute2D>& twoPinNets,int chosen, vector<vector<GridSupply>>& graph);
int RerouteNet(vector<TwoPinRoute2D>& twoPinNets);
#endif
