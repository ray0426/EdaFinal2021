#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

struct Pos {
    int row;
    int col;
};

int manh(Pos, Pos);

/******************************
 * function name: kruskal
 * input: Pin2D (position of pins in 2D plane
 * output: TBD (two-pin nets
 *****************************/

//RMST kruskal(input) {
    // create a graph with only node (without edge
    // 1.
    // calculate manhattan distance of nodes and sort (small to big)
    // connect two unconnected node in order of mangattan and add to graph
    // repeat untill no unconnected node
//}

int main(void) {
    int i, j;
    vector<Pos> pins = {
        {0, 0},
        {1, 1},
        {2, 2},
        {3, 3}
    };

    cout << manh(pins[1], pins[3]) << endl;

    return 0;
}

int manh(Pos p1, Pos p2) {
    return (abs(p1.row - p2.row) + abs(p1.col - p2.col));
}
