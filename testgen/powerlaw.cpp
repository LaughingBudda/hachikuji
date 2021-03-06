#include <bits/stdc++.h>
using namespace std;

int main(int argc, char **argv) {
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    //Number of vertices and edges
    double a = 3, b = 0.5;
    if(argc > 1) {
        a = atof( argv[1] );
    }
    if(argc > 2) {
        b = atof( argv[2] );
    }

    vector<int> degseq, vcopy[2];
    set<array<int, 2>> E;
    long maxdeg = exp(a/b), degsum=0;
    for(long x=maxdeg; x>5; --x) {
        int y = exp(a)/pow(x, b);
        for(int i=0; i<y; ++i) {
            degseq.push_back(x);
            degsum += x;
            //cerr << vcopy.size() << " " << degsum << endl;
        }
    }

    double avgdeg = (double)degsum/degseq.size();
    long mindeg = 10*sqrt(avgdeg);
    degsum=0;
    for(int i=0; i<degseq.size(); ++i){
        if (degseq[i] < mindeg) {
            degseq.resize(i);
            break;
        }
        degsum += degseq[i];
        for(int j=0; j<degseq[i]; ++j)
            vcopy[0].push_back(i);
    }
    avgdeg = degsum/degseq.size();
    if (avgdeg > pow(degseq.size(), 0.666666)) {
        cerr << "cond fail" << avgdeg << " " << pow(degseq.size(), 0.666666) << endl;;
    }
    cerr << degseq.size() << endl;
    for(auto it : degseq) cerr << it << " "; cerr << endl;

    /*
    if (degsum%2) {
        //degseq.push_back(3);
        degsum += 3;
        for(int i=0; i<3; ++i)
            vcopy[0].push_back(n);
        n++;
    }
    int m = degsum/2;


    int iter = 0;
    while(iter < 2) {
        //creating the edge set
        int c = iter%2, n = (iter+1)%2;
        shuffle(vcopy[c].begin(), vcopy[c].end(), rng);
        vcopy[n].clear();
        for(int i=0; i<vcopy[c].size()-1; ++i) {
            array<int, 2> edge = {vcopy[c][i], vcopy[c][i+1]};
            if (E.find(edge) == E.end() && vcopy[c][i] != vcopy[c][i+1]) {
               E.insert(edge);
               //E.insert({vcopy[c][i+1], vcopy[c][i]});
               i++;
            }
            else {
                vcopy[n].push_back(vcopy[c][i]);
                if (i == vcopy[c].size()-2)
                    vcopy[n].push_back(vcopy[c][i+1]);
            }
        }
        cerr << iter << " " << vcopy[c].size() << " " << vcopy[n].size() << endl;
        iter++;
        if (vcopy[n].size() < 2) break;
    }

    cerr << a << " " << b << " " << a/b << " " << maxdeg << endl;
    cerr << n << " " << m << endl;
    cerr << E.size() <<  " " << iter << endl;

    vector<int> perm(n);
    for(int i=0; i<n; ++i) perm[i]=i;
    shuffle(perm.begin(), perm.end(), rng);
    cout << n << " " << E.size() << endl;
    //for(int i=0; i<n; ++i) cout << i << endl;
    for(auto it : E) cout << perm[it[0]] << " " << perm[it[1]] << endl;
    */
}

