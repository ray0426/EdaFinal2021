#include <iostream>
//#include <fstream>
#include <vector>
#include <string>
#include "problem.h"
using namespace std;



struct Pin {
    int row;
    int col;
};
struct Net2D {
    vector<Pin> pin2Ds;
    string name;
    // string minRouteLayConst;
    float weight;
};

vector<Net2D> pinThree2Two(Problem *pro);

int main (int argc, char** argv) {
    Problem* pro = new Problem;
    pro->readCase(argv[1]);
    vector<Net2D> flattenNet = pinThree2Two(pro);
    delete pro;
    return 0;
}


vector<Net2D> pinThree2Two (Problem* pro) {  
    vector<Net2D> flatenNets;
    CellInst currentCellinst;
    string instName;
    Net CurrentNet;
    Pin pin;

    //making ele of Net2D
    for (int i = 0; i < pro->NumNet; i++) {
        CurrentNet = pro->nets[i];

        Net2D flatenNet;
        flatenNet.name = CurrentNet.name;
        // flatenNet.minRouteLayConst = CurrentNet.minRouteLayConst;
        flatenNet.weight = CurrentNet.weight;

        vector<Pin> pins;
        
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
} 
