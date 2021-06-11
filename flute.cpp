#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Permu {
    int n;
    vector<vector<int>> list;
};

struct Pin {
    int row;
    int col;
};

struct Edge { // eq. Route2D in compress.h and Route in problem.h
    int srow, scol;
    int erow, ecol;
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
    vector<Pin> pins;
    vector<Edge> edges;
};

vector<vector<int>> permutations(int n);
int idxOfPosSeq(vector<int> posSeq);
int factorial(int m, int n);
Graph genGraph(vector<int> posSeq);
//type genLUT(sometype graph);                  // to be complete
//Graph genPOST(sometype graph);
//type Expand(sometype graph, sometype bd);     // to be complete
//type Compact(sometype graph, sometype bd);    // to be complete
//type Prune(sometype graph1, sometype graph2); // to be complete

int main (void) {
    int i, j, k;
    int idx;
    int size;
    vector<Permu> permus;
    Permu permu;
    vector<PPtable> pptables;
    PPtable pptable;
//    vector<Pin> Pins;            // to be decide
    Graph graph;              // to be complete

    for (i = 2; i <= 4; i++) {
        permu.n = i;
        permu.list = permutations(i);
        permus.push_back(permu);
    }

    for (i = 0; i <= 2; i++) {    // for each amount of pins
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

    graph.pins.resize(n);
    for (i = 0; i < n; i++) {         // row, col from 1 to n
        graph.pins[i] = (Pin){.row = i + 1, .col = posSeq[i]};
    }
    return graph;
}

