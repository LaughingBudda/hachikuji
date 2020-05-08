#include <bits/stdc++.h>
#include "jngen.h"
using namespace std;

int main(int argc, char **argv) {
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    // A k regular graph with n vertices
    int n = 100, k = 10;
    if (argc > 1) {
        n = atoi( argv[1] );
    }
    if (argc > 2) {
        k = atoi( argv[2] );
    }
    if (k > n) {
        cerr << "k cannot be greater than n\n";
        return 0;
    }
    
    set<array<int, 2>> E;
    for(int i=0; i<n; ++i) {
        for(int j=1; j<=k; ++j) {
            E.insert({i, (i+j)%n});
        }
    } 

    vector<int> perm(n);
    for(int i=0; i<n; ++i) perm[i]=i;
    shuffle(perm.begin(), perm.end(), rng);
    cout << n << " " << E.size() << endl;
    for(auto it : E) cout << perm[it[0]] << " " << perm[it[1]] << endl;
}

