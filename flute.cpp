#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Permu {
    int n;
    vector<vector<int>> list;
};

vector<vector<int>> permutations(int n);
int idxOfPosSeq(vector<int> posSeq);
int factorial(int m, int n);

int main (void) {
    int i, j;
    vector<Permu> permus;
    struct Permu permu;

    for (i = 2; i <= 4; i++) {
        permu.n = i;
        permu.list = permutations(i);
        permus.push_back(permu);
    }

//    for (i = 0; i < 24; i++) {
//    cout << permus[2].list[i][0] << permus[2].list[i][1] <<
//        permus[2].list[i][2] << permus[2].list[i][3] << ": ";
//        cout << idxOfPosSeq(permus[2].list[i]) << endl;
//    }

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
