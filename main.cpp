#include <iostream>
//#include <fstream>
#include "compress.h"
#include "problem.h"
#include "rsmtAware.h"
#include <string>
#include <vector>
using namespace std;

int main(int argc, char **argv) {
  vector<vector<EdgeS>> edgeSkeleton;
  if (!argv[1]) {
    cout << "Please input case file" << endl;
    return -1;
  }
  Problem *pro = new Problem;
  pro->readCase(argv[1]);

  vector<Net2D> flattenNet = Three2Two(pro);
  vector<vector<GridSupply>> gSupGraph = GenerateGridSupplyGraph(pro);
  vector<vector<EdgeSupply>> eSupGraph = Grid2EdgeSupply(gSupGraph);

  cout << "multiPinNets" << endl;
  //  for (auto net : flattenNet) {
  //    PrintNet2D(net);
  //  }

  // print(flattenNet[1].pin2Ds);
  edgeSkeleton = rsmtAware(flattenNet[1].pin2Ds, pro->GGridBD[0],
                           pro->GGridBD[2], pro->GGridBD[1], pro->GGridBD[3]);

  delete pro;
  return 0;
}
