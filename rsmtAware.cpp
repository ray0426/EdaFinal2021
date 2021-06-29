#include <iostream>
#include <vector>
#include "flute.h"
#include <algorithm>
using namespace std;

struct Pos {
    int row;
    int col;
};

struct Route2D {
    Pos sIdx;
    Pos eIdx;
};

struct Net2D {
    vector<Pos> pin2Ds;
    string name;
    float weight;
    vector<Route2D> route2Ds;
};

void rsmtAware(void);
int wirelength(vector<int> powv, vector<int> hv);
bool samePos(Pos pos1, Pos pos2);
bool sortRow(Pos pos1, Pos pos2);
void print(vector<Pos>& poss);

int main(void) {
    int i, j;
    vector<Pos> pins = {
        {2, 0},
        {1, 4},
        {0, 2},
        {4, 3},
        {1, 4},
        {0, 2}
    };
    vector<Pos>::iterator it;
    vector<int> h;
    vector<int> v;

    print(pins);
    sort(pins.begin(), pins.end(), sortRow);
    it = unique(pins.begin(), pins.end(), samePos);
    pins.resize(distance(pins.begin(), it));
    sort(pins.begin(), pins.end(), sortRow);
    print(pins);

    for (i = 0; i < pins.size(); i++) {

    //rsmtAware();
    return 0;
}

void rsmtAware(void) {
    PPtable pptable = genLookupTable(6);
    cout << pptable.result.size() << endl;
}

int wirelength(vector<int> powv, vector<int> hv) {
    int i;
    int result = 0;

    if (powv.size() != hv.size()) {
        cout << "wrong size!!" << endl;
        return -1;
    }

    for (i = 0; i < powv.size(); i++) {
        result += powv[i] * hv[i];
    }
    return result;
}


bool samePos(Pos pos1, Pos pos2) {
    return (pos1.row == pos2.row) && (pos1.col == pos2.col);
}

bool sortRow(Pos pos1, Pos pos2) {
    return pos1.row <= pos2.row;
}

void print(vector<Pos>& poss) {
    int i;

    cout << "pins:" << endl;
    for (i = 0; i < poss.size(); i++) {
        cout << "(" << poss[i].row << ", " <<
            poss[i].col << ")" << endl;
    }
    cout << endl;
}
