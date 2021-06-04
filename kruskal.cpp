#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>

using namespace std;

struct Pos {
    int row;
    int col;
};

struct Edge {
    int p1, p2;
    int dist;
};

vector<int> G;

int manh(Pos, Pos);
bool compare(Edge, Edge);
int find(int p);
void unionV(int p1, int p2);

/******************************
 * function name: kruskal
 * input: Pin2D (position of pins in 2D plane
 * output: TBD (two-pin nets
 *****************************/

//RMST kruskal(input) {
    // create a graph with only node (without edge
    // calculate manhattan distance of nodes and sort (small to big)
    // connect two unconnected node in order of manhattan and add to graph
    // repeat untill no unconnected node
//}

int main(void) {
    int i, j;
    vector<Pos> pins = {
        {0, 0},
        {1, 1},
        {2, 2},
        {3, 3},
        {1, 3}
    };
    vector<int> distRow;
    vector<Edge> Esort;
    int V, E;

    V = pins.size();
    E = (V * (V - 1)) / 2;
    for (i = 0; i < V; i++) {
        G.push_back(i);
    }

    for (i = 0; i < V; i++)
        for (j = i + 1; j < V; j++)
            Esort.push_back({i, j, manh(pins[i], pins[j])});
    sort(Esort.begin(), Esort.end(), compare);

    for (i = 0; i < E; i++) {
        cout << Esort[i].p1 << " " << Esort[i].p2
             << " " << Esort[i].dist << endl;
    }

    for (i = 0, j = 0; (i < V - 1) && (j < E); i++) {
        while (find(Esort[j].p1) == find(Esort[j].p2)) j++;
        unionV(Esort[j].p1, Esort[j].p2);
        cout << "起點：" << Esort[j].p1
             << "終點：" << Esort[j].p2
             << "長度：" << Esort[j].dist << endl;
        j++;
    }
    // cout << manh(pins[1], pins[3]) << endl;

    return 0;
}

int manh(Pos p1, Pos p2) {
    return (abs(p1.row - p2.row) + abs(p1.col - p2.col));
}


bool compare(Edge e1, Edge e2) {
    return e1.dist <= e2.dist;
}

int find(int p) {
    return (p == G[p]) ? p : (G[p] = find(G[p]));
}
void unionV(int p1, int p2) {
    G[find(p1)] = find(p2);
}
