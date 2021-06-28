#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

bool debug = false;
int Totaln = 6;

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

/*int main (void) {
    int i, j;
    int k, l;
    int idx;
    int size;
    vector<Permu> permus;
    Permu permu;
    vector<PPtable> pptables;
    PPtable pptable;
    Graph graph;              // to be complete
    Graph graph1;

    for (i = 2; i <= Totaln; i++) {
        permu.n = i;
        permu.list = permutations(i);
        permus.push_back(permu);
    }

    for (i = 0; i <= Totaln - 2; i++) {    // for each amount of pins
        pptable.n = i + 1;
        size = permus[i].list.size();
        pptable.list.resize(size);
        pptable.result.resize(size);
//        cout << "size: " << size << endl;
        for (j = 0; j < size; j++) {   // for each permutation
            //cout << "PosSeq: " << permus[i].list[j] << endl;
            idx = idxOfPosSeq(permus[i].list[j]);
            pptable.list[idx] = permus[i].list[j];

            graph = genGraph(permus[i].list[j]);
            //cout << "genGraph complete" << endl;
            graph1 = genLUT(graph);
            cout << "permutation: " << j + 1 << endl;
            print(graph1);
            pptable.result[idx].powv = genPowv(graph1);
            pptable.result[idx].post = graph1.edges;
            cout << pptable.result[idx].powv << endl;
        }
        pptables.push_back(pptable);
        pptable.list.clear();
    }
    cout << "flute complete" << endl;
    return 0;
}*/

PPtable genLookupTable(int n) {
    int j;
    int idx;
    int size;
    Permu permu;
    PPtable pptable;
    Graph graph;
    Graph graph1;

    permu.n = n;
    permu.list = permutations(n);

    pptable.n = n;
    size = permu.list.size();
    pptable.list.resize(size);
    pptable.result.resize(size);
//        cout << "size: " << size << endl;
    for (j = 0; j < size; j++) {   // for each permutation
        //cout << "PosSeq: " << permus[i].list[j] << endl;
        idx = idxOfPosSeq(permu.list[j]);
        pptable.list[idx] = permu.list[j];

        graph = genGraph(permu.list[j]);
        //cout << "genGraph complete" << endl;
        graph1 = genLUT(graph);
//        cout << "permutation: " << j + 1 << endl;
//        print(graph1);
        pptable.result[idx].powv = genPowv(graph1);
        pptable.result[idx].post = graph1.edges;
//        cout << pptable.result[idx].powv << endl;
    }
    cout << "flute complete, size: " << n << endl;
    return pptable;
}

vector<vector<int>> permutations(int n) {
    vector<vector<int>> permN;
    vector<int> A;
    int i, j, k;
    int times = 0;
    int swap;
    int temp;
    int findj = 1, findk;
    int revend;

    for (i = 0; i < n; i++) A.push_back(n - i);

    while (findj == 1) {
//        cout << "permutatuion " << ++times << ": ";

//        for (j = 0; j < n; j++) cout << " " << A[j];
//        cout << endl;
        permN.push_back(A);

        findj = 0;
        for (j = n - 2; j >= 0 && findj == 0; j--) {
            if (A[j] > A[j + 1]) {
                findj = 1;
                findk = 0;
                for (k = n - 1; k > j && findk == 0; k--) {
                    if (A[j] > A[k]) {
                        findk = 1;
                        temp = A[j];
                        A[j] = A[k];
                        A[k] = temp;
                        revend = (n + j) / 2;
                        for (i = j + 1; i <= revend; i++) {
                            swap = n - (i - j);
                            temp = A[i];
                            A[i] = A[swap];
                            A[swap] = temp;
                        }
                    }
                }
            }
        }
    }
//    cout << "finish: " << n << endl;
    reverse(permN.begin(), permN.end());
    return permN;
}

int idxOfPosSeq(vector<int> posSeq) {
    int n = posSeq.size();
    int i, j, k;
    int count;
    int index = 0;
    int temp;

    for (i = 1; i <= n; i++) {
        count = 0;
        for (j = 1; j < i; j++)
            if (posSeq[j - 1] < posSeq[i - 1])
                count += 1;
//        cout << count << " ";
        index += count * factorial(i, n);
    }
//    cout << endl;
    return index;
}

// (n!)/(m!)
int factorial(int m, int n) {
    int i;
    int num = 1;

    for (i = m + 1; i <= n; i++) num *= i;
    return num;
}


//struct Graph {
//    vector<Pin> pins;
//    vector<Edge> edges;
//};

Graph genGraph(vector<int> posSeq) {  // posSeq contains 1~n
    int i, j;
    int n = posSeq.size();
    Graph graph;

    graph.minrow = 1;
    graph.maxrow = n;
    graph.mincol = 1;
    graph.maxcol = n;
    graph.pins.resize(n + 1);
    for (i = 0; i <= n; i++) {         // row, col from 1 to n
        graph.pins[i].resize(n + 1);
        fill(graph.pins[i].begin(), graph.pins[i].end(), 0);
    }
    for (i = 1; i <= n; i++ ) {
        graph.pins[i][posSeq[i - 1]] = 1;
    }
    return graph;
}

Graph genLUT(Graph graph) {
    if (debug) cout << "genLUT:" << endl;
    if (debug) print(graph);
    int Case;
    Graph graph1, graph2, graph3, graph4;
    int minimum;

    // single row
    if (graph.minrow == graph.maxrow && graph.mincol != graph.maxcol)
        return genPOST(graph, 1);
    // single column
    if (graph.minrow != graph.maxrow && graph.mincol == graph.maxcol)
        return genPOST(graph, 2);
    // 2x2
    if ((graph.maxrow - graph.minrow) == 1 &&
             (graph.maxcol - graph.mincol) == 1)
        return genPOST(graph, 3);
    // boundary contain only 1 pin
    Case = judgeCond1(graph);
//    cout << "Case: " << Case << endl;
    if (Case != -1)
        return CompactAndExpand(graph, Case);
    // corner with one pin s.t. both adjacent b1, b2 have 1 other pin
    Case = judgeCond2(graph);
    if (Case != -1)
        return CompactAndExpandAndPrune(graph, Case);
    /*
    else if ("7pins-all-on-bs")
        cout << "not implement" << endl;*/
    graph1 = CompactAndExpand(graph, 0);
    graph2 = CompactAndExpand(graph, 1);
    graph3 = CompactAndExpand(graph, 2);
    graph4 = CompactAndExpand(graph, 3);
    minimum = min(min(graph1.edges.size(), graph2.edges.size()),
              min(graph3.edges.size(), graph4.edges.size()));
    if (minimum == graph1.edges.size())
        return graph1;
    if (minimum == graph2.edges.size())
        return graph2;
    if (minimum == graph3.edges.size())
        return graph3;
    if (minimum == graph4.edges.size())
        return graph4;

    return CompactAndExpandAndPrune(graph, 0);
    cout << "not implement in genLUT" << endl;
}

// suppose each boundary contains at least a pin
// three situations:
// 1. compact to a single line(single row or single col)
// 2. A 2 by 2 square
// 3. At least 3 by 3 with 7 pins on boundary
// case1: if (graph.minrow == graph.maxrow && graph.mincol != graph.maxcol) ;
// case2: if (graph.minrow != graph.maxrow && graph.mincol == graph.maxcol) ;
// case3: if ((graph.maxrow - graph.minrow) == 1 &&
//            (graph.maxcol - graph.mincol) == 1) ;
//        at least on two diagonal
// case4: if ("7pins-all-on-bd")
// case5: if ("more-pins-7-on-bd")
Graph genPOST(Graph graph, int Case) {
    int i, j;
    int mincol = graph.mincol, maxcol = graph.maxcol;
    int minrow = graph.minrow, maxrow = graph.maxrow;

    switch (Case) {
        case 1:
            for (i = mincol; i < maxcol; i++)
                graph.edges.push_back(Edge(
                    minrow, minrow,
                    i     , i + 1
                    ));
            break;
        case 2:
            for (i = minrow; i < maxrow; i++)
                graph.edges.push_back(Edge(
                    i     , i + 1,
                    mincol, mincol
                    ));
            break;
        case 3:
//            cout << minrow << " " << maxrow << " " <<
//                mincol << " " << maxcol << endl;
            graph.edges.push_back(Edge(
                minrow, maxrow,
                mincol, mincol
                ));
            graph.edges.push_back(Edge(
                minrow, maxrow,
                maxcol, maxcol
                ));
            graph.edges.push_back(Edge(
                minrow, minrow,
                mincol, maxcol
                ));
            break;
//        case 4:
        default:
            cout << "not implement in genPOST, Case: " << Case << endl;
    }
    return graph;
}

// bd 0:minrow, 1:maxrow, 2:mincol, 3:maxcol
int judgeCond1(Graph& graph) {
    if (debug) cout << "judgeCond1" << endl;
    int i, j;
    int count = 0;

    // minrow
    for (j = graph.mincol; j <= graph.maxcol; j++) {
        count += graph.pins[graph.minrow][j];
    }
    if (count == 1) return 0;

    // maxrow
    count = 0;
    for (j = graph.mincol; j <= graph.maxcol; j++) {
        count += graph.pins[graph.maxrow][j];
    }
    if (count == 1) return 1;

    // mincol
    count = 0;
    for (i = graph.minrow; i <= graph.maxrow; i++) {
        count += graph.pins[i][graph.mincol];
    }
    if (count == 1) return 2;

    // maxcol
    count = 0;
    for (i = graph.minrow; i <= graph.maxrow; i++) {
        count += graph.pins[i][graph.maxcol];
    }
    if (count == 1) return 3;
    return -1;
}

// bd 0:minrow, 1:maxrow, 2:mincol, 3:maxcol
Graph CompactAndExpand(Graph graph, int bd) {
    if (debug) cout << "CAE  bd: " << bd << endl;
    int i, j;
    Graph graph1 = graph;

    switch (bd) {
        case 0:
            for (j = graph.mincol; j <= graph.maxcol; j++) {
                if (graph1.pins[graph.minrow][j]) {
                    graph1.pins[graph.minrow + 1][j] = 1;
                }
            }
            graph1.minrow += 1;
            graph.edges = genLUT(graph1).edges;
            for (j = graph.mincol; j <= graph.maxcol; j++) {
                if (graph1.pins[graph.minrow][j]) {
                    graph.edges.push_back(Edge(
                        graph.minrow    , graph.minrow + 1,
                        j               , j
                        ));
                }
            }
            break;
        case 1:
            for (j = graph.mincol; j <= graph.maxcol; j++) {
                if (graph1.pins[graph.maxrow][j]) {
                    graph1.pins[graph.maxrow - 1][j] = 1;
                }
            }
            graph1.maxrow -= 1;
            graph.edges = genLUT(graph1).edges;
            for (j = graph.mincol; j <= graph.maxcol; j++) {
                if (graph1.pins[graph.maxrow][j]) {
                    graph.edges.push_back(Edge(
                        graph.maxrow - 1, graph.maxrow,
                        j               , j
                        ));
                }
            }
            break;
        case 2:
            for (i = graph.minrow; i <= graph.maxrow; i++) {
                if (graph1.pins[i][graph.mincol]) {
                    graph1.pins[i][graph.mincol + 1] = 1;
                }
            }
            graph1.mincol += 1;
            graph.edges = genLUT(graph1).edges;
            for (i = graph.minrow; i <= graph.maxrow; i++) {
                if (graph1.pins[i][graph.mincol]) {
                    graph.edges.push_back(Edge(
                        i               , i               ,
                        graph.mincol    , graph.mincol + 1
                        ));
                }
            }
            break;
        case 3:
            for (i = graph.minrow; i <= graph.maxrow; i++) {
                if (graph1.pins[i][graph.maxcol]) {
                    graph1.pins[i][graph.maxcol - 1] = 1;
                }
            }
            graph1.maxcol -= 1;
            graph.edges = genLUT(graph1).edges;
            for (i = graph.minrow; i <= graph.maxrow; i++) {
                if (graph1.pins[i][graph.maxcol]) {
                    graph.edges.push_back(Edge(
                        i               , i           ,
                        graph.maxcol - 1, graph.maxcol
                        ));
                }
            }
            break;
        default:
            cout << "something went wrong in CAE " <<
                "with bd: " << bd << endl;
    }
    return graph;
}

// Case (row, col) 0:(min,min) 1:(min,max) 2:(max,min) 3:(max,max)
int judgeCond2(Graph& graph) {
    if (debug) cout << "judgeCond2" << endl;
    int i, j;
    int countrow, countcol;
    int minrow = graph.minrow, mincol = graph.mincol;
    int maxrow = graph.maxrow, maxcol = graph.maxcol;

    // (min, min)
    if (graph.pins[minrow][mincol]) {
        countrow = 0;
        countcol = 0;
        // minrow
        for (j = graph.mincol + 1; j <= graph.maxcol; j++) {
            countcol += graph.pins[minrow][j];
        }
        // mincol
        for (i = graph.minrow + 1; i <= graph.maxrow; i++) {
            countrow += graph.pins[i][mincol];
        }
        if (countrow == 1 && countcol == 1) return 0;
    }

    // (min, max)
    if (graph.pins[minrow][maxcol]) {
        countrow = 0;
        countcol = 0;
        // minrow
        for (j = graph.mincol; j <= graph.maxcol - 1; j++) {
            countcol += graph.pins[minrow][j];
        }
        // mincol
        for (i = graph.minrow + 1; i <= graph.maxrow; i++) {
            countrow += graph.pins[i][maxcol];
        }
        if (countrow == 1 && countcol == 1) return 1;
    }

    // (max, min)
    if (graph.pins[maxrow][mincol]) {
        countrow = 0;
        countcol = 0;
        // minrow
        for (j = graph.mincol + 1; j <= graph.maxcol; j++) {
            countcol += graph.pins[maxrow][j];
        }
        // mincol
        for (i = graph.minrow; i <= graph.maxrow - 1; i++) {
            countrow += graph.pins[i][mincol];
        }
        if (countrow == 1 && countcol == 1) return 2;
    }

    // (max, max)
    if (graph.pins[maxrow][maxcol]) {
        countrow = 0;
        countcol = 0;
        // minrow
        for (j = graph.mincol; j <= graph.maxcol - 1; j++) {
            countcol += graph.pins[maxrow][j];
        }
        // mincol
        for (i = graph.minrow; i <= graph.maxrow - 1; i++) {
            countrow += graph.pins[i][maxcol];
        }
        if (countrow == 1 && countcol == 1) return 3;
    }

    return -1;
}

Graph CompactAndExpandAndPrune(Graph graph, int Case) {
    if (debug) cout << "CAEAP  Case: " << Case << endl;
    int i, j;
    Graph graph1, graph2;
    int minimum;

    switch (Case) {
        case 0: // (min, min)
            graph1 = CompactAndExpand(graph, 0);
            graph2 = CompactAndExpand(graph, 2);
            break;
        case 1: // (min, max)
            graph1 = CompactAndExpand(graph, 0);
            graph2 = CompactAndExpand(graph, 3);
            break;
        case 2: // (max, min)
            graph1 = CompactAndExpand(graph, 1);
            graph2 = CompactAndExpand(graph, 2);
            break;
        case 3: // (max, max)
            graph1 = CompactAndExpand(graph, 1);
            graph2 = CompactAndExpand(graph, 3);
            break;
        default:
            graph1 = CompactAndExpand(graph, 0);
            graph2 = CompactAndExpand(graph, 2);
            cout << "something went wrong in CAEAP " <<
                "with case: " << Case << endl;
    }
//    return Prune(graph1, graph2);
//    return Union(graph1, graph2);
    minimum = min(graph1.edges.size(), graph2.edges.size());
    if (minimum == graph1.edges.size())
        return graph1;
    if (minimum == graph2.edges.size())
        return graph2;
    return graph1;
}

vector<int> genPowv(Graph& graph) {
    int i, j;
    vector<int> powv;

    powv.resize(graph.maxrow + graph.maxcol - graph.minrow - graph.mincol);
    fill(powv.begin(), powv.end(), 0);
    for (i = 0; i < graph.edges.size(); i++) {
        if (graph.edges[i].srow == graph.edges[i].erow) {
            powv[graph.edges[i].scol - 1] += 1;
        }
        if (graph.edges[i].scol == graph.edges[i].ecol) {
            powv[graph.maxcol - 1 + graph.edges[i].srow - 1] += 1;
        }
    }
    return powv;
}

void print(vector<vector<int>>& vec) {
    int i, j;
    int row = vec.size();
    int col;

    for (i = 1; i < row; i++) {
        col = vec[i].size();
        for (j = 1; j < col; j++) {
            cout << vec[i][j] << " ";
        }
        cout << endl;
    }
}

void print(vector<vector<bool>>& vec) {
    int i, j;
    int row = vec.size();
    int col;

    for (i = 0; i < row; i++) {
        col = vec[i].size();
        for (j = 0; j < col; j++) {
            cout << (vec[i][j] == true) << " ";
        }
        cout << endl;
    }
}

//  ------
//  |·   |
//  | ·  |
//  |    |
//  |    |
//  ------
void print(Graph& graph) {
    int i, j;
    int n = graph.pins.size() - 1;
    vector<vector<bool>> h;
    vector<vector<bool>> v;

    h.resize(n);
    v.resize(n - 1);
    for (i = 0; i < n; i++) {
        h[i].resize(n - 1);
        fill(h[i].begin(), h[i].end(), false);
    }
    for (i = 0; i < n - 1; i++) {
        v[i].resize(n);
        fill(v[i].begin(), v[i].end(), false);
    }
    for (i = 0; i < graph.edges.size(); i++) {
//        cout << graph.edges[i] << endl;
        if (graph.edges[i].srow == graph.edges[i].erow)
            h[graph.edges[i].srow - 1][graph.edges[i].scol - 1] = true;
        if (graph.edges[i].scol == graph.edges[i].ecol)
            v[graph.edges[i].srow - 1][graph.edges[i].scol - 1] = true;
    }

//    print(h);
//    print(v);

    // for testing
    // h[0][0] = true;
    // v[0][1] = true;

    // cout << "print graph========================" << endl;
    if (graph.pins.size() == 0) {
        cout << "graph is empty" << endl;
        return;
    }

    for (j = 0; j < graph.pins[0].size() * 2 - 1; j++) cout << "-";
    cout << endl;

    for (i = 1; i < graph.pins.size(); i++) {
        // each row
        cout << "|";
        for (j = 1; j < graph.pins[0].size(); j++) {
            if (graph.pins[i][j]) cout << "·";
            else cout << " ";
            if (j + 1 != graph.pins[0].size()) {
                if (h[i - 1][j - 1])
                    cout << "-";
                else
                    cout << " ";
            }
        }
        cout << "|" << endl;

        // between row
        if (i + 1 != graph.pins.size()) {
            cout << "|";
            for (j = 1; j < graph.pins[0].size(); j++) {
                if (v[i - 1][j - 1])
                    cout << "|";
                else
                    cout << " ";
                if (j + 1 != graph.pins[0].size()) cout << " ";
            }
            cout << "|" << endl;
        }
    }

    for (j = 0; j < graph.pins[0].size() * 2 - 1; j++) cout << "-";
    cout << endl;
    // cout << "end print graph====================" << endl;
}

ostream& operator<<(ostream& os, Edge& edge) {
    os << "srow: " << edge.srow << ", " <<
          "erow: " << edge.erow << ", " <<
          "scol: " << edge.scol << ", " <<
          "ecol: " << edge.ecol;
    return os;
}

ostream& operator<<(ostream& os, vector<int>& vec) {
    int i;

    for (i = 0; i < vec.size(); i++) {
        os << vec[i] << " ";
    }
    return os;
}
