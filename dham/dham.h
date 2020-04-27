#include <bits/stdc++.h>
using namespace std;

extern vector<set<int>> E_adj;
extern vector<array<int, 2>> E_list;
int m_star;


/***************************      PHASE - 1      ******************************/

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef vector<int> vi;
struct Dinic {
    struct Edge {
        int to, rev;
        ll c, oc;
        ll flow() { return max(oc - c, 0LL); } // if you need flows
    };
    vi lvl, ptr, q;
    vector<vector<Edge>> adj;
    Dinic(int n) : lvl(n), ptr(n), q(n), adj(n) {}
    void addEdge(int a, int b, ll c, int rcap = 0) {
        adj[a].push_back({b, sz(adj[b]), c, c});
        adj[b].push_back({a, sz(adj[a]) - 1, rcap, rcap});
    }
    ll dfs(int v, int t, ll f) {
        if (v == t || !f) return f;
        for (int& i = ptr[v]; i < sz(adj[v]); i++) {
            Edge& e = adj[v][i];
            if (lvl[e.to] == lvl[v] + 1)
                if (ll p = dfs(e.to, t, min(f, e.c))) {
                    e.c -= p, adj[e.to][e.rev].c += p;
                    return p;
                }
        }
        return 0;
    }
    ll calc(int s, int t) {
        ll flow = 0; q[0] = s;
        rep(L,0,31) do { // 'int L=30' maybe faster for random data
            lvl = ptr = vi(sz(q));
            int qi = 0, qe = lvl[s] = 1;
            while (qi < qe && !lvl[t]) {
                int v = q[qi++];
                trav(e, adj[v])
                    if (!lvl[e.to] && e.c >> (30 - L))
                        q[qe++] = e.to, lvl[e.to] = lvl[v] + 1;
            }
            while (ll p = dfs(s, t, LLONG_MAX)) flow += p;
        } while (lvl[t]);
        return flow;
    }
};

vector<int> phase1(int n, int ms) {
    int m = E_list.size();
    m_star = ms;
    
    // random permutation and it's inverse
    vector<int> sigma(n), sigmai(n);
    for(int i=0; i<n; i++) sigma[i]=i;
    random_shuffle(sigma.begin(), sigma.end());
    for(int i=0; i<n; i++) sigmai[sigma[i]] = i;

    // Creating a edge set with avg in/out-degree 10
    set< array<int, 2> > Ep, Em, Ecap;
    vector<int> dEp(n), dEm(n);
    for(int i=0; i<ms; i++) {
        auto e = E_list[i];
        if(dEp[e[0]] < 10) {
            Ep.insert({e[0], sigma[e[1]]});
            dEp[e[0]]++;
        }
    }
    for(int i=0; i<ms; i++) {
        auto e = E_list[i];
        array<int, 2> ecap = {e[0], sigma[e[1]]};
        if( (dEm[sigma[e[1]]] < 10) && (Ep.find(ecap) != Ep.end()) ){
            Em.insert(ecap);
            dEm[ecap[1]]++;
        }
    }
    Ecap = Ep;
    Ecap.insert(Em.begin(), Em.end());
    
    // Create the bipartite graph, and find maximum matching
    Dinic F(2*n + 2);
    vector<int> psi(n, 2*n+1), phi(n, 2*n+1);
    for(auto it : Ecap) {
        F.addEdge(it[0], it[1] + n, 1);
        F.addEdge(it[1], it[0] + n, 1);
    }
    for(int i=0; i<n; i++) {
        F.addEdge(2*n, i, 1);
        F.addEdge(n + i, 2*n + 1, 1);
    }
    F.calc(2*n, 2*n + 1);
    for(int i=0; i<n; i++) {
        //cerr << i << " : ";
        //for(auto it : F.adj[i]) fprintf(stderr, "(%d, %d), ", it.to, it.flow()); cerr << endl;
        for(auto it : F.adj[i]) {
            if(it.flow() == 1) {
                psi[i] = it.to - n;
                break;
            }
        }
        //phi[i] = psi[sigmai[i]];
        phi[i] = sigmai[psi[i]];
        if(phi[i] == 2*n+1) {
            //time to break, it's all lost, no Hamiltonian cyale
            cerr << "At i = " << i << " phi[i] = " << phi[i] << endl;
            break;
        }
    }
    return phi;
}


/***************************      PHASE - 2      ******************************/

struct DSU {
    vector<int> parent, size;
    void initialize(int n) {
        parent.resize(n);
        size.resize(n);
    }
    void make_set(int x) {
        size[x] = 1;
        parent[x] = x;
    }
    int find(int x) {
        if(parent[x] == x)
            return x;
        return find(parent[x]);
    }
    void unoin(int a, int b) {
        a = find(a); b = find(b);
        if(a != b) {
            if(size[a] < size[b])
                swap(a, b);
            parent[b] = a;
            size[a] += size[b];
        }
    }
}cycle;

void phase2(vector<int> &phi) {
    int n = phi.size();
    int m2 = ceil(n*log2(n)*5.0/6.0);
    
    cycle.initialize(n);
    vector<int> phi_i(n);
    for(int i=0; i<n; ++i) cycle.make_set(i);
    for(int i=0; i<n; ++i) {
        cycle.unoin(i, phi[i]);
        phi_i[phi[i]] = i;
    }
    bool flag = true;
    while(flag) {
        flag = false;
        for(int i=0; i<m2; ++i) {
            int x = E_list[i][0], y = E_list[i][1], z = phi_i[E_list[i][1]], w = phi[E_list[i][0]];
            if(cycle.find(x) != cycle.find(y) && E_adj[z].find(w) != E_adj[z].end() ) {
                cycle.unoin(x, y);
                phi[x] = y; phi_i[y] = x;
                phi[z] = w; phi_i[w] = z;
                flag = true;
                break;
            }
        }
    }
}


/***************************      PHASE - 3      ******************************/

struct Node {
    Node *l = 0, *r = 0, *p = 0;
    int val, y, c = 1;
    Node(int val) : val(val), y(rand()) {}
    void recalc();
};

int cnt(Node* n) { return n ? n->c : 0; }
void Node::recalc() { c = cnt(l) + cnt(r) + 1; }
vector<Node*> nodeat;

pair<Node*, Node*> split(Node* n, int k) {
    if (!n) return {};
    if (cnt(n->l) >= k) { // "n->val >= k" for lower_bound(k)
        auto pa = split(n->l, k);
        n->l = pa.second;
        pa.second->p = n;
        n->recalc();
        return {pa.first, n};
    } else {
        auto pa = split(n->r, k - cnt(n->l) - 1); // and just "k"
        n->r = pa.first;
        pa.first->p = n;
        n->recalc();
        return {n, pa.second};
    }
}

Node* merge(Node* l, Node* r) {
    if (!l) return r;
    if (!r) return l;
    if (l->y > r->y) {
        r->p = l;
        l->r = merge(l->r, r);
        l->recalc();
        return l;
    } else {
        l->p = r;
        r->l = merge(l, r->l);
        r->recalc();
        return r;
    }
}

Node* ins(Node* t, Node* n, int pos) {
    auto pa = split(t, pos);
    return merge(merge(pa.first, n), pa.second);
}

int key(Node* t, Node* x) {
    int ans = cnt(x->l);
    if (!t) return -1;
    while(x != t) {
        auto par = x->p;
        if (par->r == x)
            ans += 1 + cnt(par->l);
        x = par;
    }
    return ans;
}

int value(Node *n, int key) {
    if (!n) return -1;
    if (cnt(n->l) == key) return n->val;
    else if (cnt(n->l) > key) return value(n->l, key);
    else return value(n->r, key - cnt(n->l) - 1);
}

void move(Node*& t, int l, int r, int k) {
    Node *a, *b, *c;
    tie(a,b) = split(t, l); tie(b,c) = split(b, r - l);
    if (k <= l) t = merge(ins(a, b, k), c);
    else t = merge(a, ins(c, b, k - r));
}


vector<bool> used;
bool dfs(Node* t, int depth) {
    static vector<set<int>> E_adj (::E_adj.begin(), E_adj.begin()+m_star);

    if (depth < 1) return false;

    auto x = t;
    while(x->r) x = x->r;
    int last = x->val, k = cnt(t);
    bool ret = false;
    used[last] = true;

    for(auto a : E_adj[last]) {
        int ia = key(t, nodeat[a]);
        if(ia == 0) continue;
        int a1 = value(t, ia-1);
        for(auto b : E_adj[a1]) {
            int ib = key(t, nodeat[b]);
            if(ib > ia) {
                //here we finally have a valid operation
                int b1 = value(t, ib-1);
                if(!used[b1]) {
                    move(t, ia, ib, k-ib+ia);
                    if (E_adj[value(t, k-1)].find(value(t, 0)) != E_adj[value(t, k-1)].end()) {
                        return true;
                    }
                    ret = ret || dfs(t, depth -1);
                    if (ret) return ret;
                    move(t, k-ib+ia, k, a);
                }
            }
        }
    }
    return ret;
}

bool findcycle(int C1, int Ci, int i, vector<int> &phi) {
    int xj = i, xj1 = phi[i], n = phi.size();
    int T = ceil((2.0*log2(n))/(3.0*log2(log2(n))));
    static vector<set<int>> E_adj (::E_adj.begin(), E_adj.begin()+m_star);
    used.resize(n);
    nodeat.resize(n);
    
    //create each path in rho0, and explore them (depth limited)
    for(auto it : E_adj[xj]) {
        if(cycle.find(it) == cycle.find(C1) ) {
            Node* temp = new Node(xj1);
            nodeat[xj1] = temp;
            used.assign(n, false);
            int k=1;
        	for(int i=phi[xj1]; phi[i] != it; i = phi[i]) {
                Node* t = new Node(i);
                nodeat[i] = t;
        		temp = ins(temp, t, k++);
        		if( i == xj ) {
                    Node* t = new Node(it);
                    nodeat[it] = t;
                    temp = ins(temp, t, k++);
        			i = it;
        		}
        	}
        	dfs(temp, T);
        }
    }
    return true;
}

void phase3(vector<int> &phi) {
    int n = phi.size(), max=0;
    map<int, int> C;
    for(int i=0; i<n; ++i) {
        C[cycle.find(i)]++;
        if( C[max] > C[cycle.find(i)] )
           max = cycle.find(i);
    }
    C.erase(max); 

    for(auto it : C) {
        bool outcome = false;
        for(int i=it.first; phi[i] != it.first; i = phi[i]) {
            outcome = findcycle(max, it.first, i, phi);
            if(outcome) {
            	cycle.unoin(max, it.first);
                break;
            }
        }
        if(!outcome) {
            //terminate!! no solution
            return;
        }
    }
}
