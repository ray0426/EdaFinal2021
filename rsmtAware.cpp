#include "compress.h"
#include "flute.h"
#include "structures.h"
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

vector<vector<EdgeS>> rsmtAware(vector<Pos> pins, int mini, int maxi, int minj,
                                int maxj);
int wirelength(vector<int> powv, vector<int> hv);
bool samePos(Pos pos1, Pos pos2);
bool sortRow(Pos pos1, Pos pos2);
bool sortCol(Pos pos1, Pos pos2);
void print(vector<Pos> &poss);

/*
int main(void) {
  vector<vector<EdgeS>> edgeSke;
  vector<Pos> pins = {{2, 0}, {1, 4}, {0, 2}, {4, 3}, {1, 4}, {0, 2}};
  int mini = 1, maxi = 4, minj = 1, maxj = 4;

  edgeSke = rsmtAware(pins, mini, maxi, minj, maxj);
  return 0;
}
*/

vector<vector<EdgeS>> rsmtAware(vector<Pos> pins, int mini, int maxi, int minj,
                                int maxj) {
  int key1 = 1, key2 = 1;
  int i, j, k, l;
  vector<Pos> pins2;
  vector<Pos>::iterator it;
  vector<int> h;
  vector<int> v;
  vector<int> posSeq;
  int idx;
  PPtable pptable;
  vector<Edge> post;
  EdgeS edges = {0, 0};
  vector<int> hp;
  vector<int> vp;
  vector<vector<EdgeS>> edgeSke;

  sort(pins.begin(), pins.end(), sortRow);
  it = unique(pins.begin(), pins.end(), samePos);
  pins.resize(distance(pins.begin(), it));
  sort(pins.begin(), pins.end(), sortRow);
  // print(pins);

  edgeSke.resize(maxi - mini + 1);
  // cout << edgeSke.size() << endl;
  for (i = 0; i < edgeSke.size(); i++) {
    edgeSke[i].resize(maxj - minj + 1);
    for (j = 0; j < edgeSke.size(); j++) {
      edgeSke[i][j] = edges;
    }
  }

  while (key1) {
    // cout << "test pinsize: " << pins.size() << endl;
    key1 = 0;
    key2 = 1;
    for (i = 0; i < pins.size() && key2; i++) {
      for (j = i + 1; j < pins.size() && key2; j++) {
        // cout << "i: " << i << ", j: " << j << endl;
        if (pins[i].row == pins[j].row) {
          edges.row = 1;
          edges.col = 0;
          if (pins[i].col < pins[j].col) {
            for (k = pins[i].col; k < pins[j].col; k++) {
              // cout << "1" << endl;
              // cout << "row: " << pins[i].row - 1 << ", col: " << k - 1 <<
              // endl;
              edgeSke[pins[i].row - 1][k - 1] = edges;
            }
            pins.erase(pins.begin() + j);
          } else {
            for (k = pins[j].col; k < pins[i].col; k++) {
              // cout << "1" << endl;
              // cout << "row: " << pins[i].row - 1 << ", col: " << k - 1 <<
              // endl;
              edgeSke[pins[i].row - 1][k - 1] = edges;
            }
            pins.erase(pins.begin() + i);
          }
          key1 = 1;
          key2 = 0;
        } else if (pins[i].col == pins[j].col) {
          edges.row = 0;
          edges.col = 1;
          if (pins[i].row < pins[j].row) {
            for (k = pins[i].row; k < pins[j].row; k++) {
              // cout << "2" << endl;
              // cout << "row: " << k - 1 << ", col: " << pins[i].col - 1 <<
              // endl;
              edgeSke[k - 1][pins[i].col - 1] = edges;
            }
            pins.erase(pins.begin() + j);
          } else {
            for (k = pins[j].row; k < pins[i].row; k++) {
              // cout << "2" << endl;
              // cout << "row: " << k - 1 << ", col: " << pins[i].col - 1 <<
              // endl;
              edgeSke[k - 1][pins[i].col - 1] = edges;
            }
            pins.erase(pins.begin() + i);
          }
          key1 = 1;
          key2 = 0;
        }
        // cout << "i: " << i << ", j: " << j << ", finish" << endl;
        // cout << "key1: " << key1 << ", key2: " << key2 << endl;
      }
    }
  }
  // cout << "finish fix" << endl;
  // print(pins);

  for (i = 1; i < pins.size(); i++) {
    v.push_back(pins[i].row - pins[i - 1].row);
  }
  sort(pins.begin(), pins.end(), sortCol);
  for (i = 1; i < pins.size(); i++) {
    h.push_back(pins[i].col - pins[i - 1].col);
  }
  //  cout << h << endl;
  //  cout << v << endl;

  pins2 = pins;
  for (i = 0; i < pins.size(); i++) {
    pins2[i].col = i + 1;
  }
  sort(pins2.begin(), pins2.end(), sortRow);
  for (i = 0; i < pins.size(); i++) {
    posSeq.push_back(pins2[i].col);
  }
  idx = idxOfPosSeq(posSeq);
  // cout << idx << endl;

  pptable = genLookupTable(pins2.size());
  post = pptable.result[idx].post;
  // cout << post.size() << endl;
  // for (i = 0; i < post.size(); i++) {
  //  cout << post[i].srow << " " << post[i].scol << " " << post[i].erow << " "
  //       << post[i].ecol << endl;
  //}

  sort(pins.begin(), pins.end(), sortRow);
  for (i = 0; i < pins.size(); i++) {
    vp.push_back(pins[i].row);
  }
  sort(pins.begin(), pins.end(), sortCol);
  for (i = 0; i < pins.size(); i++) {
    hp.push_back(pins[i].col);
  }
  // cout << hp << endl;
  // cout << vp << endl;

  for (i = 0; i < post.size(); i++) {
    if (post[i].srow == post[i].erow) {
      edges.row = 1;
      edges.col = 0;
      for (j = hp[post[i].scol - 1]; j < hp[post[i].ecol - 1]; j++) {
        // cout << "1" << endl;
        // cout << "row: " << vp[post[i].srow - 1] - 1 << ", col: " << j - 1
        //     << endl;
        edgeSke[vp[post[i].srow - 1] - 1][j - 1] = edges;
      }
    } else if (post[i].scol == post[i].ecol) {
      edges.row = 0;
      edges.col = 1;
      for (j = vp[post[i].srow - 1]; j < vp[post[i].erow - 1]; j++) {
        // cout << "2" << endl;
        // cout << "row: " << j - 1 << ", col: " << hp[post[i].scol - 1] - 1
        //     << endl;
        edgeSke[j - 1][hp[post[i].scol - 1] - 1] = edges;
      }
    }
  }

  return edgeSke;
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

bool sortRow(Pos pos1, Pos pos2) { return pos1.row <= pos2.row; }
bool sortCol(Pos pos1, Pos pos2) { return pos1.col <= pos2.col; }

void print(vector<Pos> &poss) {
  int i;

  cout << "pins:" << endl;
  for (i = 0; i < poss.size(); i++) {
    cout << "(" << poss[i].row << ", " << poss[i].col << ")" << endl;
  }
  cout << endl;
}
