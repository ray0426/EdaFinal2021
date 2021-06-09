// #include <iostream>
#include <vector>
#include <string>
#include "problem.h"
#include "compress.h"
using namespace std;

vector<Net2D> pinThree2Two (Problem* pro) {
    vector<Net2D> flatenNets;
    CellInst currentCellinst;
    string instName;
    Net CurrentNet;
    Pin pin;
    Net2D flatenNet;
    vector<Pin> pins;
    //making ele of Net2D
    for (int i = 0; i < pro->NumNet; i++) {
        CurrentNet = pro->nets[i];

        flatenNet.pin2Ds.clear();
        flatenNet.name = CurrentNet.name;
        // flatenNet.minRouteLayConst = CurrentNet.minRouteLayConst;
        flatenNet.weight = CurrentNet.weight;

        pins.clear();

        //finding flatenNet.pin2Ds
        for (int j = 0; j < CurrentNet.NumPins; j++) {
            instName = CurrentNet.pins[j].instName;
            // string masterPinName = CurrentNet.pins[j].masterPinName;
            for (auto currentCellinst : pro->cellinsts) {
                if (instName == currentCellinst.name) {
                    pin.col = currentCellinst.colIdx;
                    pin.row = currentCellinst.rowIdx;
                    pins.push_back(pin);
                    break;
                }
            }
        }
        flatenNet.pin2Ds = pins;
        flatenNets.push_back(flatenNet);
    }
    return flatenNets;
}
vector<vector<EdgeSupply>> GenerateSupplyGraph (Problem* pro) {
    int hSupply;
    int vSupply;
    vector<vector<EdgeSupply>> graph;
    vector<EdgeSupply> rowGraph;
    EdgeSupply grid;

    for (Layer layer: pro->layers) {
        if (layer.direction == 'H')
            hSupply += layer.defaultsupply;
        else if (layer.direction == 'V')
            vSupply += layer.defaultsupply;
    }

    for (int i = 0; i < pro->GGridBD[2]; i++) {
        rowGraph.clear();
        for (int j = 0; j < pro->GGridBD[3]; j++) {
            grid.col = (i == 0) ? 0 : vSupply;
            grid.row = (j == 0) ? 0 : hSupply;
            rowGraph.push_back(grid);
        }
        graph.push_back(rowGraph);
    }

    for (NonDefaultSupply nd: pro->NonDefaultSupplies) {
        for (Layer layer: pro->layers) {
            if (layer.direction == 'H')
                graph[nd.rowIdx][nd.colIdx].row += nd.relatedValue;
            else if (layer.direction == 'V')
                graph[nd.rowIdx][nd.colIdx].col += nd.relatedValue;
            break;
        }
    }
    return graph;
}
