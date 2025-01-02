// https://judge.yosupo.jp/problem/point_set_range_composite

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

const int MAXN = 4e5;

// Data, ID
// Data + Data
// Data2, ID2
// Data2 + Data2
// bool f(Data)
// Data apply(Data, Data2, tl, tr)

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

typedef pair<mint, mint> Data;
Data operator + (const Data &p, const Data &q) { return Data{p.first*q.first, p.second*q.first+q.second}; }
Data ID={1, 0};

struct SEG
{
    // bool f(int p) { return __builtin_popcount(p)<=1; }
    // Data apply(Data p, Data2 q, int tl, int tr) {}

    Data tree[MAXN*4+10];
    // Data2 lazy[MAXN*4+10];

    // void apply(int node, int tl, int tr, Data2 upd)
    // {
    //     tree[node]=apply(tree[node], upd, tl, tr);
    //     lazy[node]=lazy[node]+upd;
    // }

    // void push(int node, int tl, int tr)
    // {
    //     if(tl!=tr)
    //     {
    //         int mid=tl+tr>>1;
    //         apply(node*2, tl, mid, lazy[node]);
    //         apply(node*2+1, mid+1, tr, lazy[node]);
    //     }
    //     lazy[node]=ID2;
    // }

    void init(int node, int tl, int tr, vector<Data> &A)
    {
        // lazy[node]=ID2;
        if(tl==tr) { tree[node]=A[tl]; return; }
        int mid=tl+tr>>1;
        init(node*2, tl, mid, A);
        init(node*2+1, mid+1, tr, A);
        tree[node]=tree[node*2]+tree[node*2+1];
    }

    void update_point(int node, int tl, int tr, int p, Data val)
    {
        if(tl==tr) { tree[node]=val; return; }
        // push(node, tl, tr);
        int mid=tl+tr>>1;
        if(p<=mid) update_point(node*2, tl, mid, p, val);
        else update_point(node*2+1, mid+1, tr, p, val);
        tree[node]=tree[node*2]+tree[node*2+1];
    }

    // void update_range(int node, int tl, int tr, int l, int r, Data2 val)
    // {
    //     if(r<tl || tr<l) return;
    //     if(l<=tl && tr<=r)
    //     {
    //         apply(node, tl, tr, val);
    //         return;
    //     }
    //     push(node, tl, tr);
    //     int mid=tl+tr>>1;
    //     update_range(node*2, tl, mid, l, r, val);
    //     update_range(node*2+1, mid+1, tr, l, r, val);
    //     tree[node]=tree[node*2]+tree[node*2+1];
    // }

    Data query(int node, int tl, int tr, int l, int r)
    {
        if(r<tl || tr<l) return ID;
        if(l<=tl && tr<=r) return tree[node];
        // push(node, tl, tr);
        int mid=tl+tr>>1;
        return query(node*2, tl, mid, l, r)+query(node*2+1, mid+1, tr, l, r);
    }

    // int query_bsl(int node, int tl, int tr, int l, int r, int &val)
    // {
    //     if(r<tl || tr<l) return -1;
    //     if(l<=tl && tr<=r)
    //     {
    //         int val2=__gcd(val, tree[node]);
    //         if(!f(val2)) { val=val2; return -1; }
    //     }
    //     if(tl==tr) return tl;
    //     // push(node, tl, tr);
    //     int mid=tl+tr>>1;
    //     int ret=query_bsl(node*2, tl, mid, l, r, val);
    //     if(ret!=-1) return ret;
    //     return query_bsl(node*2+1, mid+1, tr, l, r, val);
    // }

    // int query_bsr(int node, int tl, int tr, int l, int r, int &val)
    // {
    //     if(r<tl || tr<l) return -1;
    //     if(l<=tl && tr<=r)
    //     {
    //         int val2=__gcd(tree[node], val);
    //         if(!f(val2)) { val=val2; return -1; }
    //     }
    //     if(tl==tr) return tl;
    //     // push(node, tl, tr);
    //     int mid=tl+tr>>1;
    //     int ret=query_bsr(node*2+1, mid+1, tr, l, r, val);
    //     if(ret!=-1) return ret;
    //     return query_bsr(node*2, tl, mid, l, r, val);
    // }
}seg;

int main()
{
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    int N, Q;
    cin >> N >> Q;
    vector<Data> A(N);
    for(int i=0; i<N; i++) cin >> A[i].first.x >> A[i].second.x;
    seg.init(1, 0, N-1, A);
    while(Q--)
    {
        int t;
        cin >> t;
        if(t==0)
        {
            int p; Data x;
            cin >> p >> x.first.x >> x.second.x;
            seg.update_point(1, 0, N-1, p, x);
        }
        else
        {
            int l, r, x;
            cin >> l >> r >> x;
            r--;
            Data ret=seg.query(1, 0, N-1, l, r);
            cout << ret.first*mint(x)+ret.second << "\n";
        }
    }
}