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
struct Net2D {
    vector<Pin> pin2Ds;
    string name;
    float weight;
};

vector<vector<EdgeSupply>> GenerateSupplyGraph(Problem *pro);
vector<Net2D> pinThree2Two(Problem *pro);

#endif
