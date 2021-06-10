// #include <iostream>
#include <vector>
#include <string>
#include "problem.h"
#include "compress.h"
#include <algorithm>
using namespace std;

vector<Net2D> Three2Two (Problem* pro) {
    vector<Net2D> flatenNets;   //return

    CellInst currentCellinst;
    string instName;            
    Net CurrentNet;
    Route CurrentRoute;         //now using

    Pin pin;                    
    vector<Pin> pins;
    Route2D flatenRoute;
    Net2D flatenNet;            //buffer
    
    //pin of Net2D,this part of Net2D are loss of routing
    for (int i = 0; i < pro->NumNet; i++) {
        CurrentNet = pro->nets[i];

        //reseting
        flatenNet.pin2Ds.clear();
        flatenNet.route2Ds.clear();

        //seting name and weight
        flatenNet.name = CurrentNet.name;
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
    //now we put routing into Net2Ds
    for (int i = 0; i < pro->NumRoute; i++){
        CurrentRoute = pro->routes[i];
        
        if (CurrentRoute.elayIdx == CurrentRoute.slayIdx) continue;

        for (Net2D planedNet: flatenNets) {
            if (planedNet.name == CurrentRoute.name) {
                flatenRoute.erowIdx = CurrentRoute.erowIdx;
                flatenRoute.ecolIdx = CurrentRoute.ecolIdx;
                flatenRoute.srowIdx = CurrentRoute.srowIdx;
                flatenRoute.scolIdx = CurrentRoute.scolIdx;
                planedNet.route2Ds.push_back(flatenRoute);
                break;
            }
        }
    }
    
    return flatenNets;
}
vector<vector<EdgeSupply>> GenerateSupplyGraph (Problem* pro) {
    int hSupply;
    int vSupply;
    int rowBound = pro->GGridBD[2];
    int colBound = pro->GGridBD[3];
    vector<vector<EdgeSupply>> graph;
    vector<EdgeSupply> rowGraph;
    EdgeSupply grid;

    //counting defaultsupply
    for (Layer layer: pro->layers) {
        if (layer.direction == 'H')
            hSupply += layer.defaultsupply;
        else if (layer.direction == 'V')
            vSupply += layer.defaultsupply;
    }
    //assert defaultsupply to grids
    for (int i = 0; i < rowBound; i++) {
        rowGraph.clear();
        for (int j = 0; j < colBound; j++) {
            grid.col = (i == 0) ? 0 : vSupply;
            grid.row = (j == 0) ? 0 : hSupply;
            rowGraph.push_back(grid);
        }
        graph.push_back(rowGraph);
    }
    //change grids of nondefaultsupply
    for (NonDefaultSupply nd: pro->NonDefaultSupplies) {
        int rId = nd.rowIdx;
        int cId = nd.colIdx;
        int delta = nd.relatedValue;
        Layer layer = pro->layers[nd.LayIdx];

        if (layer.direction == 'H') graph[rId][cId].row += delta;
        else if (layer.direction == 'V') graph[rId][cId].col += delta;
    }
    //gridsupply to edgesupply
    for (int i = rowBound - 1; i >= 0; i--) {
        for (int j = colBound - 1; j >= 0; j--) {
            graph[i][j].row = (i == 0) ? 0 : min(graph[i][j].row, graph[i - 1][j].row);
            graph[i][j].col = (j == 0) ? 0 : min(graph[i][j].col, graph[i][j - 1].col);
        }
    }
    
    return graph;
}
