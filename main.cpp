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

    vector<Net2D> flattenNet = Three2Two(pro);  //this is not pointer version
    vector<vector<GridSupply>>* gSupGraph = GenerateGridSupplyGraph(pro);
    vector<vector<EdgeSupply>>* eSupGraph = Grid2EdgeSupply(gSupGraph);
    


    delete pro;
    return 0;
}



