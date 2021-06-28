#include <iostream>
//#include <fstream>
#include <vector>
#include <string>
#include "problem.h"
#include "compress.h"
using namespace std;

int main (int argc, char** argv) {
    if (!argv[1]) {
        cout << "Please input case file"<< endl;
        return -1;
    }
    Problem* pro = new Problem;
    pro->readCase(argv[1]);

    vector<Net2D> flattenNet = Three2Two(pro);
    vector<vector<GridSupply>> gSupGraph = GenerateGridSupplyGraph(pro);
    vector<vector<EdgeSupply>> eSupGraph = Grid2EdgeSupply(gSupGraph);

    cout << "multiPinNets" << endl;
    for(auto net: flattenNet){
        PrintNet2D(net);
    }

    //cout << "gridSupply(v,h)" << endl;
    //for (auto r: gSupGraph) {
    //    for (auto g: r) {
    //        PrintGridSupply(g);
    //    }
    //    cout << endl;
    //}
    //SupplyChange(gSupGraph,flattenNet[0]);
    //cout << "supplychange" << endl;
    //for (auto r: gSupGraph) {
    //    for (auto g: r) {
    //        PrintGridSupply(g);
    //    }
    //    cout << endl;
    //}
//
    //cout << "edgeSupply(v,h)" << endl;
    //for (auto r: eSupGraph) {
    //    for (auto e: r) {
    //        PrintEdgeSupply(e);
    //    }
    //    cout << endl;
    //}
//
    //TwoPinRoute2D TwoPinNet = Multi2TwoPinRoute(flattenNet[0], flattenNet[0].pin2Ds[0], flattenNet[0].pin2Ds[2]);
    //PrintTwoPinNet(TwoPinNet);
//
    //TwoPinRoute2D Net1 = Multi2TwoPinRoute(flattenNet[1], flattenNet[1].pin2Ds[1], flattenNet[1].pin2Ds[2]);
    //TwoPinRoute2D Net2 = Multi2TwoPinRoute(flattenNet[1], flattenNet[1].pin2Ds[0], flattenNet[1].pin2Ds[3]);
    //TwoPinRoute2D Net3 = Multi2TwoPinRoute(flattenNet[1], flattenNet[1].pin2Ds[0], flattenNet[1].pin2Ds[1]);
    //vector<TwoPinRoute2D> Nets{Net1, Net2, Net3};
//
    //for (auto n: Nets) {
    //    PrintTwoPinNet(n);
    //}
    //
    //int i = RerouteNet(Nets);
    //PrintTwoPinNet(Nets[i]);
// //
    //SortTaskQueue(Nets,i,gSupGraph);
    //for (auto n: Nets) {
    //    PrintTwoPinNet(n);
    //}

    delete pro;
    return 0;
}



