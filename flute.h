#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Permu {
    int n;
    vector<vector<int>> list;
};

struct Edge { // eq. Route2D in compress.h and Route in problem.h
    int srow, scol;
    int erow, ecol;

    Edge(int sr, int er, int sc, int ec) {
        srow = sr;
        erow = er;
        scol = sc;
        ecol = ec;
    }
};

struct PP {
    vector<int> powv; // e.g. seq=3142 (1,2,1,1,1,1) size: 2(n - 1)
    vector<Edge> post;
};

struct PPtable {
    int n;
    vector<vector<int>> list;
    vector<PP> result;
};

struct Graph {
    int minrow, maxrow;
    int mincol, maxcol;
    vector<vector<int>> pins;
    vector<Edge> edges;
};

PPtable genLookupTable(int n);
vector<vector<int>> permutations(int n);
int idxOfPosSeq(vector<int> posSeq);
int factorial(int m, int n);
Graph genGraph(vector<int> posSeq);
Graph genLUT(Graph graph);
Graph genPOST(Graph graph, int Case);
int judgeCond1(Graph& graph);
Graph CompactAndExpand(Graph graph, int bd);
int judgeCond2(Graph& graph);
Graph CompactAndExpandAndPrune(Graph graph, int Case);
vector<int> genPowv(Graph& graph);
void print(vector<vector<int>>& vec);
void print(vector<vector<bool>>& vec);
void print(Graph& graph);

ostream& operator<<(ostream& os, Edge& edge);
ostream& operator<<(ostream& os, vector<int>& vec);
