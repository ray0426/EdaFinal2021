#ifndef COMPRESS_H
#define COMPRESS_H

// #include <iostream>
#include <vector>
#include <string>
#include "problem.h"
#include "rsmtAware.h"
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
};

struct BLMRgrid {
    Pos further;
    int len;
    int cost;
    bool isBLC;
};

void PrintEdgeSupply(EdgeSupply a);
void PrintGridSupply(GridSupply a);
void PrintPos(Pos a);
void PrintRoute2D(Route2D a);
void PrintNet2D(Net2D a);
void PrintTwoPinNet(TwoPinRoute2D a);

vector<Net2D> Three2Two(Problem *pro);

void MergeNet (vector<Route2D>& routes);    //all twoPinNet Route put into it
TwoPinRoute2D Multi2TwoPinRoute(Net2D& net, Pos sPin, Pos ePin);

TwoPinNets CreateNetSet(Problem* pro, string& name);

vector<vector<GridSupply>> GenerateGridSupplyGraph(Problem *pro);
vector<vector<EdgeSupply>> Grid2EdgeSupply(vector<vector<GridSupply>>& graph);
void SupplyChange(vector<vector<GridSupply>>& graph, Net2D newNet, Net2D oldNet = {});
void afterRouting (vector<vector<GridSupply>>& graph, TwoPinRoute2D& net, TwoPinNets& netSet);

void SortTaskQueue(vector<TwoPinRoute2D>& twoPinNets,int chosen, vector<vector<GridSupply>>& graph);
int RerouteNet(vector<TwoPinRoute2D>& twoPinNets);
void BLMR(vector<vector<GridSupply>>& graph, TwoPinNets& netSet, TwoPinRoute2D& net, int& ite, vector<vector<EdgeS>>& skeleton);
#endif
