#ifndef COMPRESS_H
#define COMPRESS_H

// #include <iostream>
#include "problem.h"
#include <string>
#include <vector>
//#include "rsmtAware.h"
#include "structures.h"
using namespace std;

void PrintEdgeSupply(EdgeSupply a);
void PrintGridSupply(GridSupply a);
void PrintPos(Pos a);
void PrintRoute2D(Route2D a);
void PrintNet2D(Net2D a);
void PrintTwoPinNet(TwoPinRoute2D a);

bool isPosSame(Pos &a, Pos &b);
vector<Net2D> Three2Two(Problem *pro);

void MergeNet(vector<Route2D> &routes); // all twoPinNet Route put into it
TwoPinRoute2D Multi2TwoPinRoute(Net2D &net, Pos sPin, Pos ePin);

TwoPinNets CreateNetSet(Problem *pro, string &name);

vector<vector<GridSupply>> GenerateGridSupplyGraph(Problem *pro);
vector<vector<EdgeSupply>> Grid2EdgeSupply(vector<vector<GridSupply>> &graph);
void SupplyChange(vector<vector<GridSupply>> &graph, Net2D newNet,
                  Net2D oldNet = {});
void afterRouting(vector<vector<GridSupply>> &graph, TwoPinRoute2D &net,
                  TwoPinNets &netSet);

void SortTaskQueue(vector<TwoPinRoute2D> &twoPinNets, int chosen,
                   vector<vector<GridSupply>> &graph);
int RerouteNet(vector<TwoPinRoute2D> &twoPinNets);
void BLMR(vector<vector<GridSupply>> &graph, TwoPinNets &netSet,
          TwoPinRoute2D &net, int &ite);
#endif
