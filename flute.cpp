#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Permu {
    int n;
    vector<vector<int>> list;
};

//struct Pin {
//    int row;
//    int col;
//};

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

vector<vector<int>> permutations(int n);
int idxOfPosSeq(vector<int> posSeq);
int factorial(int m, int n);
Graph genGraph(vector<int> posSeq);
Graph genLUT(Graph graph);
Graph genPOST(Graph graph, int Case);
Graph CompactAndExpand(Graph graph, int bd);
//type Prune(sometype graph1, sometype graph2); // to be complete
//int Judge(Graph graph);
void print(vector<vector<int>> vec);

int main (void) {
    int i, j;
    int k, l;
    int idx;
    int size;
    vector<Permu> permus;
    Permu permu;
    vector<PPtable> pptables;
    PPtable pptable;
//    vector<Pin> Pins;            // to be decide
    Graph graph;              // to be complete
    Graph graph1;

    for (i = 2; i <= 4; i++) {
        permu.n = i;
        permu.list = permutations(i);
        permus.push_back(permu);
    }

    for (i = 0; i <= 0; i++) {    // for each amount of pins
        pptable.n = i + 1;
        size = permus[i].list.size();
        pptable.list.resize(size);
        pptable.result.resize(size);
        for (j = 0; j < size; j++) {   // for each permutation
            idx = idxOfPosSeq(permus[i].list[j]);
            pptable.list[idx] = permus[i].list[j];
            /*for (k = 0; k < i + 1; k++) {
                cout << permus[i].list[j][k];
            }
            cout << ": " << idxOfPosSeq(permus[i].list[j]) << "     ";
            */

            graph = genGraph(permus[i].list[j]);
            graph1 = genPOST(graph, 3);

            cout << "------------------" << endl;
            print(graph.pins);
            cout << "size: " << graph1.edges.size() << endl;
            cout << "edges: " << endl;
            for (k = 0; k < graph1.edges.size(); k++) {
                cout << graph1.edges[k].srow << " " <<
                        graph1.edges[k].erow << " " <<
                        graph1.edges[k].scol << " " <<
                        graph1.edges[k].ecol << endl;
            }
//            cout << graph.pins[2].col << endl;
//            pptable.result[idx] = GenLUT(graph);
        }
        pptables.push_back(pptable);
        pptable.list.clear();
    }

/*    cout << "permutation" << endl;
    for (i = 0; i < 24; i++) {
        cout << permus[2].list[i][0] << permus[2].list[i][1] <<
            permus[2].list[i][2] << permus[2].list[i][3] << ": ";
            cout << idxOfPosSeq(permus[2].list[i]) << endl;
    }
    cout << "in index" << endl;
    for (i = 0; i < 24; i++) {
        cout << pptables[2].list[i][0] << pptables[2].list[i][1] <<
            pptables[2].list[i][2] << pptables[2].list[i][3] << endl;
    }
*/

//    cout << permus.size() << endl;
//    cout << permus[0].list.size() << endl;
//    cout << permus[1].list.size() << endl;
//    cout << permus[2].list.size() << endl;

//    cout << permus[2].list[2][0] << permus[2].list[2][1] <<
//        permus[2].list[2][2] << permus[2].list[2][3] << endl;

    return 0;
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
    if (graph.minrow == graph.maxrow && graph.mincol != graph.maxcol)
        return genPOST(graph, 1);
    else if (graph.minrow != graph.maxrow && graph.mincol == graph.maxcol)
        return genPOST(graph, 2);
    else if ((graph.maxrow - graph.minrow) == 1 &&
             (graph.maxcol - graph.mincol) == 1)
        return genPOST(graph, 3);
    else if ("boundary b contains only 1 pin")
        return CompactAndExpand(graph, b);
    else if ("corner with one pin s.t. both adjacent b1, b2 have 1 other pin")
        return Prune(CompactAndExpand(graph, b1)),
                     CompactAndExpand(graph, b2));
    else if ("7pins-all-on-bs")
        cout << "not implement" << endl;
    else
        cout << "not implement" << endl;
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
            cout << "not implement" << endl;
    }
    return graph;
}

// bd 0:minrow, 1:maxrow, 2:mincol, 3:maxcol
Graph CompactAndExpand(Graph graph, int bd) {
    int i, j;
    Graph graph1 = graph;

    switch (bd) {
        case 0:
            for (j = graph.mincol; j <= graph.maxcol; j++)
                if (graph1.pins[graph.minrow][j]) {
                    graph1.pins[graph.minrow + 1][j] = 1;
                    graph1.minrow += 1;
                    graph.edges = GenLUT(graph1).edges;
                    graph.edges.push_back(Edge(
                        minrow, minrow + 1,
                        j     , j
                        ));
                }
            break;
        case 1:
            for (j = graph.mincol; j <= graph.maxcol; j++)
                if (graph1.pins[graph.maxrow][j]) {
                    graph1.pins[graph.maxrow - 1][j] = 1;
                    graph1.maxrow -= 1;
                    graph.edges = GenLUT(graph1).edges;
                    graph.edges.push_back(Edge(
                        maxrow - 1, maxrow,
                        j         , j
                        ));
                }
            break;
        case 2:
            for (i = graph.minrow; i <= graph.maxrow; i++)
                if (graph1.pins[i][graph.mincol]) {
                    graph1.pins[i][graph.mincol + 1] = 1;
                    graph1.mincol += 1;
                    graph.edges = GenLUT(graph1).edges;
                    graph.edges.push_back(Edge(
                        i     , i         ,
                        mincol, mincol + 1
                        ));
                }
            break;
        case 3:
            for (i = graph.minrow; i <= graph.maxrow; i++)
                if (graph1.pins[i][graph.maxcol]) {
                    graph1.pins[i][graph.maxcol - 1] = 1;
                    graph1.maxcol -= 1;
                    graph.edges = GenLUT(graph1).edges;
                    graph.edges.push_back(Edge(
                        i         , i     ,
                        maxcol - 1, maxcol
                        ));
                }
            break;
        default:
            cout << "something went wrong" << endl;
    }
    return graph;
}

void print(vector<vector<int>> vec) {
    int i, j;
    int row = vec.size();
    int col;

    for (i = 0; i < row; i++) {
        col = vec[i].size();
        for (j = 0; j < col; j++) {
            cout << vec[i][j] << " ";
        }
        cout << endl;
    }
}
