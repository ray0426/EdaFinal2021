#include <iostream>
//#include <fstream>
//#include <vector>
//#include <string>
#include "problem.h"
using namespace std;


int main (int argc, char** argv) {
    Problem* pro = new Problem;
    pro->readCase(argv[1]);

    delete pro;
    return 0;
}
