#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

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
    string layer;
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
    vector<GGrid> ggrids;
    vector<Instance> instances;
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

        int NumNets;
        vector<Net> nets;

        int NumVoltageAreas;
        vector<VoltageArea> voltageAreas;

        int NumRoutes;
        vector<Route> routes;

        Problem(void);
        ~Problem(void);
        void readCase(char* inputfile);
};

Problem::Problem(void) {
    cout << "construct Problem" << endl;
}

Problem::~Problem(void) {
    cout << "destruct Problem" << endl;
    // use delete[] arr; to free array of pointer
}

void Problem::readCase(char* inputfile) {
    string str; // for useless string
    ifstream file(inputfile); // read input file
    int i, j; // array index

    cout << "=====start loading input file: " << inputfile << "=====" << endl;
    //while (getline (ReadFile, line, ' ')) {
    //    cout << line << endl;
    //}
    file >> str >> this->MaxCellMove;
    file >> str;
    for (i = 0; i < 4; i++)
        file >> this->GGridBD[i];

    struct Layer layer;
    file >> str >> this->NumLayer;
    for (i = 0; i < this->NumLayer; i++) {
        file >> str >> layer.name >> layer.Idx >> layer.direction >>
            layer.defaultsupply >> layer.powerfac;
        this->layers.push_back(layer);
    }
    //cout << layers[0].name << endl;
    //cout << layers[0].Idx << endl;
    //cout << layers[2].powerfac << endl;

    struct NonDefaultSupply NonDefSP;
    file >> str >> this->NumNonDefaultSupply;
    for (i = 0; i < this->NumNonDefaultSupply; i++) {
        file >> NonDefSP.rowIdx >> NonDefSP.colIdx >>
            NonDefSP.LayIdx >> NonDefSP.relatedValue;
        this->NonDefaultSupplies.push_back(NonDefSP);
    }

    //cout << this->NonDefaultSupplies[0].relatedValue << endl;
    //cout << this->NonDefaultSupplies[1].relatedValue << endl;
    //cout << this->NonDefaultSupplies[2].colIdx << endl;

    struct MasterCell mc;
    struct MasterCellPin mcp;
    struct Blkg blkg;
    file >> str >> this->NumMasterCell;
    for (i = 0; i < this->NumMasterCell; i++) {
        file >> str >> mc.name >> mc.pinCount >> mc.blockageCount;
        for (j = 0; j < mc.pinCount; j++) {
            file >> str >> mcp.name >> mcp.layer;
            mc.pins.push_back(mcp);
        }
        for (j = 0; j < mc.blockageCount; j++) {
            file >> str >> blkg.name >> blkg.layer >> blkg.demand;
            mc.blkgs.push_back(blkg);
        }
        this->masterCells.push_back(mc);
        mc.pins.clear();
        mc.blkgs.clear();
    }
    //cout << this->masterCells[1].name << endl;
    //cout << this->masterCells[2].pins[0].name << endl;
    //cout << this->masterCells[2].pins[1].name << endl;
    //cout << this->masterCells[2].pins[2].name << endl;
    //cout << this->masterCells[2].pins[0].layer << endl;
    //cout << this->masterCells[2].pins[1].layer << endl;
    //cout << this->masterCells[2].pins[2].layer << endl;
    //cout << this->masterCells[0].blkgs[1].demand << endl;

    struct CellInst cell;
    file >> str >> this->NumCellInst;
    for (i = 0; i < this->NumCellInst; i++) {
        file >> str >> cell.name >> cell.MasterCellName >>
            cell.rowIdx >> cell.colIdx;
        file >> str;
        if (str == "Movable")
            cell.movable = 1;
        this->cellinsts.push_back(cell);
    }
    //cout << this->cellinsts[0].name << endl;
    //cout << this->cellinsts[2].MasterCellName << endl;
    //cout << this->cellinsts[4].colIdx << endl;
    //cout << this->cellinsts[7].movable << endl;

    struct NetPin netpin;
    struct Net net;
    file >> str >> this->NumNets;
    for (i = 0; i < this->NumNets; i++) {
        file >> str >> net.name >> net.NumPins >>
            net.minRouteLayConst >> net.weight;
        for (j = 0; j < net.NumPins; j++) {
            file >> str >> str;
            netpin.instName = str.substr(0, str.find('/'));
            str.erase(0, str.find('/') + 1);
            netpin.masterPinName = str;
            net.pins.push_back(netpin);
        }
        this->nets.push_back(net);
        net.pins.clear();
    }

    //cout << this->nets[0].name << endl;
    //cout << this->nets[1].NumPins << endl;
    //cout << this->nets[2].minRouteLayConst << endl;
    //cout << this->nets[3].weight << endl;
    //cout << this->nets[4].pins[0].instName << endl;
    //cout << this->nets[5].pins[0].instName << endl;
    //cout << this->nets[5].pins[1].masterPinName << endl;
    //cout << this->nets[5].pins[2].masterPinName << endl;


/*
NumNets 6
Net N1 3 M2 1.5
Pin C1/P2
Pin C5/P2
Pin C4/P2
Net N2 4 NoCstr 2.0
Pin C2/P2
Pin C5/P1
Pin C8/P2
Pin C7/P1
Net N3 3 NoCstr 1.5
Pin C2/P1
Pin C4/P1
Pin C6/P1
Net N4 2 NoCstr 1.0
Pin C1/P1
Pin C3/P2
Net N5 2 NoCstr 1.0
Pin C2/P3
Pin C3/P1
Net N6 3 NoCstr 1.5
Pin C6/P2
Pin C7/P2
Pin C8/P1
NumRoutes 42
4 1 1 4 1 3 N1
4 1 3 4 4 3 N1
4 4 3 4 4 1 N1
4 3 3 4 3 2 N1
4 3 2 1 3 2 N1
1 3 2 1 3 3 N1
1 3 3 1 2 3 N1
1 2 3 1 2 2 N1
1 2 2 2 2 2 N1
2 2 2 2 2 1 N1
5 2 1 5 3 1 N2
5 3 1 5 3 2 N2
5 3 2 4 3 2 N2
4 3 2 4 3 3 N2
4 3 3 4 4 3 N2
4 4 3 4 4 1 N2
4 4 1 4 5 1 N2
4 5 1 4 5 2 N2
4 5 2 3 5 2 N2
3 5 2 3 5 1 N2
3 5 1 3 4 1 N2
3 4 1 3 4 2 N2
3 4 2 2 4 2 N2
2 4 2 2 4 1 N2
2 2 1 2 2 3 N3
2 2 3 2 4 3 N3
2 4 3 2 4 1 N3
2 2 2 5 2 2 N3
4 1 1 4 1 2 N4
4 1 2 2 1 2 N4
2 1 2 2 1 1 N4
2 1 1 2 2 1 N4
2 2 1 2 2 2 N5
2 2 2 5 2 2 N5
5 2 2 5 2 1 N5
2 4 1 2 4 2 N6
2 4 2 1 4 2 N6
1 4 2 1 4 1 N6
1 4 1 1 5 1 N6
1 5 1 1 5 2 N6
1 5 2 3 5 2 N6
3 5 2 3 5 1 N6
NumVoltageAreas 1
Name V1
GGrids 9
2 1
2 2
2 3
3 1
3 2
3 3
4 1
4 2
4 3
Instances 2
C1
C3
*/

// ========================
//    cout << "|" << str << "|" << endl;
    file.close();
}

int main (int argc, char** argv) {
    Problem* pro = new Problem;
    pro->readCase(argv[1]);

//  for (int i = 0; i < argc; ++i)
//      cout << argv[i] << "\n";
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
