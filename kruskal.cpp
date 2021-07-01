#include "structures.h"
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

struct EdgeRMST {
  int p1, p2;
  int dist;
};

vector<int> G;

vector<Route2D> decomposition(vector<Pos> pins);
int manh(Pos, Pos);
bool compare(EdgeRMST, EdgeRMST);
int find(int p);
void unionV(int p1, int p2);

/******************************
 * function name: kruskal
 * input: Pin2D (position of pins in 2D plane
 * output: TBD (two-pin nets
 *****************************/

// RMST kruskal(input) {
// create a graph with only node (without edge
// calculate manhattan distance of nodes and sort (small to big)
// connect two unconnected node in order of manhattan and add to graph
// repeat untill no unconnected node
//}

/*int main(void) {
  vector<Pos> pins = {{0, 0}, {1, 1}, {2, 2}, {3, 3}, {1, 3}};
  decomposition(pins);
  int i, j;
  vector<int> distRow;
  vector<EdgeRMST> Esort;
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
    cout << Esort[i].p1 << " " << Esort[i].p2 << " " << Esort[i].dist << endl;
  }

  for (i = 0, j = 0; (i < V - 1) && (j < E); i++) {
    while (find(Esort[j].p1) == find(Esort[j].p2))
      j++;
    unionV(Esort[j].p1, Esort[j].p2);
    cout << "起點：p" << Esort[j].p1 << ", 終點：p" << Esort[j].p2 << ", 長度："
         << Esort[j].dist << endl;
    j++;
  }
  // cout << manh(pins[1], pins[3]) << endl;

  return 0;
}*/

vector<Route2D> decomposition(vector<Pos> pins) {
  int i, j;
  vector<int> distRow;
  vector<EdgeRMST> Esort;
  int V, E;
  vector<Route2D> route;
  Route2D route2d;
  Pos pos;
  G.clear();

  // cout << "test1" << endl;
  V = pins.size();
  E = (V * (V - 1)) / 2;
  for (i = 0; i < V; i++) {
    G.push_back(i);
  }

  // cout << "test2" << endl;
  for (i = 0; i < V; i++)
    for (j = i + 1; j < V; j++)
      Esort.push_back({i, j, manh(pins[i], pins[j])});
  sort(Esort.begin(), Esort.end(), compare);

  // for (i = 0; i < E; i++) {
  //  cout << Esort[i].p1 << " " << Esort[i].p2 << " " << Esort[i].dist << endl;
  //}

  // for (i = 0; i < V; i++) {
  //  cout << "G(" << i << "): " << G[i] << endl;
  //}
  // for (i = 0; i < V; i++) {
  //  cout << "find(" << i << "): " << find(i) << endl;
  //}

  // cout << "test3" << endl;
  // cout << Esort.size() << endl;
  for (i = 0, j = 0; (i < V - 1) && (j < E); i++) {
    // cout << "0" << endl;
    // cout << "p1: " << Esort[j].p1 << ", p2:" << Esort[j].p2 << endl;
    // cout << "1: " << find(Esort[j].p1) << ", 2:" << find(Esort[j].p2) <<
    // endl;
    while (find(Esort[j].p1) == find(Esort[j].p2))
      j++;
    // cout << "1" << endl;
    unionV(Esort[j].p1, Esort[j].p2);
    pos.row = pins[Esort[j].p1].row;
    pos.col = pins[Esort[j].p1].col;
    route2d.sIdx = pos;
    // cout << "2" << endl;
    pos.row = pins[Esort[j].p2].row;
    pos.col = pins[Esort[j].p2].col;
    route2d.eIdx = pos;
    // cout << "3" << endl;
    route.push_back(route2d);
    // cout << "4" << endl;
    // cout << "起點：p" << Esort[j].p1 << ", 終點：p" << Esort[j].p2 << ",
    // 長度："
    //      << Esort[j].dist << endl;
    j++;
  }
  // cout << "test4" << endl;
  // cout << manh(pins[1], pins[3]) << endl;
  return route;
}

int manh(Pos p1, Pos p2) {
  return (abs(p1.row - p2.row) + abs(p1.col - p2.col));
}

bool compare(EdgeRMST e1, EdgeRMST e2) { return e1.dist <= e2.dist; }

int find(int p) { return (p == G[p]) ? p : (G[p] = find(G[p])); }
void unionV(int p1, int p2) { G[find(p1)] = find(p2); }
