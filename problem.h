// ploblem.h
#ifndef PROBLEM_H
#define PROBLEM_H

#include <bits/stdc++.h>
using std::vector;
using std::string;

// Layer
struct Layer {
    string name;       // layer name
    int Idx;           // layer index
    char direction;    // H or V
    int defaultsupply; // supply of one grid in this layer
    float powerfac;    // power factor
};

// Non default supply
struct NonDefaultSupply {
    int rowIdx;       // row index
    int colIdx;       // column index
    int LayIdx;       // layer index
    int relatedValue; // increase or decrease than default
};

// Pin
struct MasterCellPin {
    string name;
    string layer;
};

// Blkg
struct Blkg {
    string name;
    int layer;
    float demand;
};

// Master Cell
struct MasterCell {
    string name;
    int pinCount;
    int blockageCount;
    vector<MasterCellPin> pins;
    vector<Blkg> blkgs;
};

// CellInst
struct CellInst {
    string name;
    string MasterCellName;
    int rowIdx;
    int colIdx;
    bool movable; // 1: movable, 0: fixed
};

// Pin
struct NetPin {
    string instName;
    string masterPinName;
};

// Net
struct Net {
    string name;
    int NumPins;
    string minRouteLayConst;
    float weight;
    vector<NetPin> pins;
};

// Voltage-area-GGrids
struct GGrid {
    int rowIdx;
    int colIdx;
};

// Voltage-area-Instances
struct Instance {
    string name;
};

// Voltage-area
struct VoltageArea {
    string name;
    int NumGGrid;
    int NumInst;
    vector<GGrid> ggrids;
    vector<Instance> instances;
};

// Route
struct Route {
    string name;
    int srowIdx, scolIdx, slayIdx;
    int erowIdx, ecolIdx, elayIdx;
};

class Problem {
    public:
        int MaxCellMove; // maximum cells move
        int GGridBD[4]; // grid boundary index

        int NumLayer; // total number of layers
        vector<Layer> layers;

        int NumNonDefaultSupply; // # of nondefult supply grid
        vector<NonDefaultSupply> NonDefaultSupplies;

        int NumMasterCell;
        vector<MasterCell> masterCells;

        int NumCellInst;
        vector<CellInst> cellinsts;

        int NumNet;
        vector<Net> nets;

        int NumVoltageArea;
        vector<VoltageArea> voltageAreas;

        int NumRoute;
        vector<Route> routes;

        Problem(void);
        ~Problem(void);
        void readCase(char* inputfile);
};
#endif
