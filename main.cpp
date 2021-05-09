#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Layer
struct Layer {
    string name;       // layer name
    int Idx;           // layer index
    int direction;     // 0 for horizontal, 1 for vertical
    int defaultsupply; // supply of one grid
    float powerfac;    // power factor
};

// Non default supply
struct NonDefaultSupply {
    public:
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
    string layer;
    float demand;
};

// Master Cell
struct MasterCell {
    string name;
    int pinCount;
    int blockageCount;
    struct MasterCellPin *pins[1000];
    struct Blkg *blkgs[1000];
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
    string minRoutingLayConstraint;
    float weight;
    struct NetPin *pins[1000];
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
    struct GGrid *ggrids[1000];
    struct Instance *instances[1000];
};

// Route
struct Route {
    string name;
    int srowIdx, scolIdx, slayIdx;
    int erowIdx, ecolIdx, elayIdx;
};

// Input Problem
class Problem {
    public:
        int MaxCellMove;
        int GGridBoundaryIdx[4]; // grid boundary index

        int NumLayer;
        struct Layer *layers[1000];

        int NumNonDefaultSupplyGGrid; // # of nondefult supply grid
        struct NonDefaultSupply *NonDefaultSupplyGGrids[1000];

        int NumMasterCell;
        struct MasterCell masterCells[1000];

        int NumCellInst;
        struct CellInst *cellinsts[1000];

        int NumNets;
        struct Net *nets[100];

        int NumVoltageAreas;
        struct VoltageArea *voltageAreas;

        int NumRoutes;
        struct Route *routes[1000];

        Problem(void);
        ~Problem(void);
};

Problem::Problem(void) {
    cout << "construct Problem" << endl;
}

Problem::~Problem(void) {
    cout << "destruct Problem" << endl;
    // use delete[] arr; to free array of pointer
}

int main () {
    Problem* pro = new Problem;
//  string line;
//  ifstream myfile ("case1.txt");
//  if (myfile.is_open())
//  {
//    while ( getline (myfile,line) )
//    {
//      cout << line << '\n';
//    }
//    myfile.close();
//  }

//  else cout << "Unable to open file";
    delete pro;
    return 0;
}
