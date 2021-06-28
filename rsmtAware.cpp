#include <iostream>
#include <vector>
#include "flute.h"
using namespace std;

void rsmtAware(void);
int wirelength(vector<int> powv, vector<int> hv);

int main(void) {
    rsmtAware();
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
