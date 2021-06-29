#ifndef RSMTAWARE_H
#define RSNTAWARE_H
#include "compress.h"
#include "flute.h"
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

struct EdgeS {
  int row;
  int col;
};

vector<vector<EdgeS>> rsmtAware(vector<Pos> pins, int mini, int maxi, int minj,
                                int maxj);
void print(vector<Pos> &poss);
#endif
