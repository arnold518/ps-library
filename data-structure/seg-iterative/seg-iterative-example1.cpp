// https://judge.yosupo.jp/problem/range_affine_range_sum

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

const int MOD = 998244353;
struct mint
{
    int x;
    mint() : x(0) {}
    mint(int _x) : x(_x) {}
    mint operator + (int ot) const { return x+ot>=MOD ? x+ot-MOD : x+ot; }
    mint operator - (int ot) const { return x<ot ? x+MOD-ot : x-ot; }
    mint operator - () const { return x ? MOD-x : 0; }
    mint operator * (int ot) const { return 1ll*x*ot%MOD; }
    mint operator += (int ot) { return *this = *this + ot; }
    mint operator -= (int ot) { return *this = *this - ot; }
    mint operator *= (int ot) { return *this = *this * ot; }
    operator int() const { return x; }
};

const int MAXN = 5e5;

// the following must be implemented for normal segment tree
// Data : type for data structure
// Data operator + (Data p, Data q), ID : identity element for Data

// the following must be implemented for lazy segment tree
// Data2 : type for lazy update
// Data2 operator + (Data2 p, Data2 q), ID2 : identity element for Data2
// Data apply(Data p, Data2 q, int tl, int tr) : result of update q applied to value p in range [tl, tr]

typedef mint Data;
typedef pair<mint, mint> Data2;
Data2 operator + (const Data2 &p, const Data2 &q) { return Data2{p.first*q.first, p.second*q.first+q.second}; }
Data ID=0;
Data2 ID2={1, 0};

struct SEG
{
    Data apply(Data p, Data2 q, int tl, int tr) { return p*q.first+q.second*(tr-tl+1); }

    int N, lg;
    pii rng[MAXN*2+10];

    // tree[node] : query value stored in node
    // lazy[node] : lazy value to be applied to subtree of node (already applied to tree[node])
    Data tree[MAXN*2+10];
    Data2 lazy[MAXN+10];

    // apply : O(1), apply update value upd to subtree of node
    void apply(int node, Data2 upd)
    {
        tree[node]=apply(tree[node], upd, rng[node].first, rng[node].second);
        if(node<N) lazy[node]=lazy[node]+upd;
    }

    // push : O(1), propagate lazy value to child, and initialize lazy
    void push(int node)
    {
        if(rng[node].first!=rng[node].second)
        {
            apply(node<<1, lazy[node]);
            apply(node<<1|1, lazy[node]);
        }
        lazy[node]=ID2;
    }
    
    void pull(int node)
    {
        tree[node]=tree[node<<1]+tree[node<<1|1];
    }

    // init : O(N), initialize tree, lazy
    void init(int _N, vector<Data> &A)
    {
        N=_N; lg=__lg(N);
        for(int i=0; i<N; i++) rng[i+N]=pii{i, i}, tree[i+N]=A[i];
        for(int i=N-1; i>=1; i--) rng[i].first=rng[i<<1].first, rng[i].second=rng[i<<1|1].second;
        for(int i=N-1; i>=1; i--) pull(i), lazy[i]=ID2;
    }

    // update_point : O(log N), update value at position p to val
    void update_point(int p, Data val)
    {
        p+=N;
        for(int i=lg; i>=1; i--) if(p>>i) push(p>>i);
        tree[p]=val;
        for(int i=1; i<=lg; i++) if(p>>i) pull(p>>i);
    }

    // update_range : O(log N), update value at range [l, r] by upd
    void update_range(int l, int r, Data2 upd)
    {
        l+=N; r+=N;
        for(int i=lg; i>=1; i--)
        {
            if(l>>i<<i != l) push(l>>i);
            if(r>>i<<i != r) push(r>>i);
        }
        for(int l2=l, r2=r; l2<r2; l2>>=1, r2>>=1)
        {
            if(l2&1) apply(l2++, upd);
            if(r2&1) apply(--r2, upd);
        }
        for(int i=1; i<=lg; i++)
        {
            if(l>>i<<i != l) pull(l>>i);
            if(r>>i<<i != r) pull(r>>i);
        }
    }

    // query : O(log N), query value at range [l, r]
    Data query(int l, int r)
    {
        Data retl=ID, retr=ID;
        l+=N; r+=N;
        for(int i=lg; i>=1; i--)
        {
            if(l>>i<<i != l) push(l>>i);
            if(r>>i<<i != r) push(r>>i);
        }
        for(; l<r; l>>=1, r>>=1)
        {
            if(l&1) retl=retl+tree[l++];
            if(r&1) retr=tree[--r]+retr;
        }
        return retl+retr;
    }
}seg;

int main()
{
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    
    int N, Q;
    cin >> N >> Q;
    vector<mint> A(N);
    for(int i=0; i<N; i++) cin >> A[i].x;
    seg.init(N, A);
    while(Q--)
    {
        int t;
        cin >> t;
        if(t==0)
        {
            int l, r; Data2 x;
            cin >> l >> r >> x.first.x >> x.second.x;
            seg.update_range(l, r, x);
        }
        else
        {
            int l, r;
            cin >> l >> r;
            cout << seg.query(l, r) << "\n";
        }
    }
}