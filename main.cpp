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

    cout << "\033[1;32mstart loading input file: " << inputfile << "\033[0m\n";
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
    file >> str >> this->NumNet;
    for (i = 0; i < this->NumNet; i++) {
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

    struct Route route;
    file >> str >> this->NumRoute;
    for (i = 0; i < this->NumRoute; i++) {
        file >> route.srowIdx >> route.scolIdx >> route.slayIdx
            >> route.erowIdx >> route.ecolIdx >> route.elayIdx >> route.name;
        this->routes.push_back(route);
    }
    //cout << this->routes[0].name << endl;
    //cout << this->routes[1].srowIdx << endl;
    //cout << this->routes[41].ecolIdx << endl;

    struct GGrid ggrid;
    struct Instance inst;
    struct VoltageArea va;
    file >> str >> this->NumVoltageArea;
    for (i = 0; i < this->NumVoltageArea; i++) {
        file >> str >> va.name;
        file >> str >> va.NumGGrid;
        for (j = 0; j < va.NumGGrid; j++) {
            file >> ggrid.rowIdx >> ggrid.colIdx;
            va.ggrids.push_back(ggrid);
        }
        file >> str >> va.NumInst;
        for (j = 0; j < va.NumInst; j++) {
            file >> inst.name;
            va.instances.push_back(inst);
        }
        this->voltageAreas.push_back(va);
        va.ggrids.clear();
        va.instances.clear();
    }
    //cout << this->voltageAreas[0].name << endl;
    //cout << this->voltageAreas[0].ggrids[8].colIdx << endl;
    //cout << this->voltageAreas[0].instances[1].name << endl;
    //
    file.close();
    cout << "\033[1;32mloading input file complete\033[0m" << endl;

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
