#include <iostream>
//#include <fstream>
#include <vector>
#include <string>
#include "problem.h"
using namespace std;


int main (int argc, char** argv) {
    Problem* pro = new Problem;
    pro->readCase(argv[1]);

    delete pro;
    return 0;
}

struct coordinate {
    int row;
    int col;
    string layer;
};
struct Net2D {
    vector<coordinate> pin2Ds;
    string name;
    string minRouteLayConst;
    float weight;
};

vector<Net2D> pinThree2Two (Problem* pro) {  
    vector<Net2D> flatenNets;

    for (int i = 0; i < pro->NumNet; i++) {
        //ctrlCV
        Net CurrentNet = pro->nets[i];
        Net2D flatenNet;
        flatenNet.name = CurrentNet.name;
        flatenNet.minRouteLayConst = CurrentNet.minRouteLayConst;
        flatenNet.weight = CurrentNet.weight;

        //
        for (int j = 0; j < CurrentNet.NumPins; j++) {
            string instName = CurrentNet.pins[j].instName;
            string masterPinName = CurrentNet.pins[j].masterPinName;

            for (int k = 0; k < pro->NumCellInst; k++){
                CellInst currentCellinst = pro->cellinsts[k];
                if (instName == currentCellinst.name) {
                    coordinate c;
                    c.col = currentCellinst.colIdx;
                    c.row = currentCellinst.rowIdx;
                    for (int m = 0; m < pro->NumMasterCell; m++){
                        MasterCell currentMasterCell = pro->masterCells[m];
                        if (currentCellinst.MasterCellName == currentMasterCell.name) {
                            for (int n = 0; n < currentMasterCell.pinCount; n++){
                                MasterCellPin currentMasterCellPin = currentMasterCell.pins[n];
                                if (currentMasterCellPin.name == masterPinName){
                                    c.layer = currentMasterCellPin.layer;
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    flatenNet.pin2Ds.push_back(c);
                    break;
                }
            }
        }
        flatenNets.push_back(flatenNet);
    }
} 