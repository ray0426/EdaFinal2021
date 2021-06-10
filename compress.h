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
struct Pin {
    int row;
    int col;
};

struct Route2D {
    int srowIdx, scolIdx;
    int erowIdx, ecolIdx;
};
struct Net2D {
    vector<Pin> pin2Ds;
    string name;
    float weight;
    vector<Route2D> route2Ds;
};

vector<vector<EdgeSupply>> GenerateSupplyGraph(Problem *pro);
vector<Net2D> Three2Two(Problem *pro);


#endif
