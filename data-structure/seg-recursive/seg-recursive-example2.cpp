// https://codeforces.com/contest/2007/problem/F

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

const int MAXN = 4e5;

int N, A[MAXN+10];

// Data, ID
// Data + Data
// Data2, ID2
// Data2 + Data2
// bool f(Data)
// Data apply(Data, Data2, tl, tr)

struct SEG
{
    bool f(int p) { return __builtin_popcount(p)<=1; }
    // Data apply(Data p, Data2 q, int tl, int tr) {}

    int tree[MAXN*4+10];
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

    void init(int node, int tl, int tr, int *A)
    {
        // lazy[node]=ID2;
        if(tl==tr) { tree[node]=abs(A[tl]-A[tl+1]); return; }
        int mid=tl+tr>>1;
        init(node*2, tl, mid, A);
        init(node*2+1, mid+1, tr, A);
        tree[node]=__gcd(tree[node*2], tree[node*2+1]);
    }

    // void update_point(int node, int tl, int tr, int p, Data val)
    // {
    //     if(tl==tr) { tree[node]=val; return; }
    //     push(node, tl, tr);
    //     int mid=tl+tr>>1;
    //     if(p<=mid) update_point(node*2, tl, mid, p, val);
    //     else update_point(node*2+1, mid+1, tr, p, val);
    //     tree[node]=tree[node*2]+tree[node*2+1];
    // }

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

    // Data query(int node, int tl, int tr, int l, int r)
    // {
    //     if(r<tl || tr<l) return ID;
    //     if(l<=tl && tr<=r) return tree[node];
    //     push(node, tl, tr);
    //     int mid=tl+tr>>1;
    //     return query(node*2, tl, mid, l, r)+query(node*2+1, mid+1, tr, l, r);
    // }

    int query_bsl(int node, int tl, int tr, int l, int r, int &val)
    {
        if(r<tl || tr<l) return -1;
        if(l<=tl && tr<=r)
        {
            int val2=__gcd(val, tree[node]);
            if(!f(val2)) { val=val2; return -1; }
        }
        if(tl==tr) return tl;
        // push(node, tl, tr);
        int mid=tl+tr>>1;
        int ret=query_bsl(node*2, tl, mid, l, r, val);
        if(ret!=-1) return ret;
        return query_bsl(node*2+1, mid+1, tr, l, r, val);
    }

    int query_bsr(int node, int tl, int tr, int l, int r, int &val)
    {
        if(r<tl || tr<l) return -1;
        if(l<=tl && tr<=r)
        {
            int val2=__gcd(tree[node], val);
            if(!f(val2)) { val=val2; return -1; }
        }
        if(tl==tr) return tl;
        // push(node, tl, tr);
        int mid=tl+tr>>1;
        int ret=query_bsr(node*2+1, mid+1, tr, l, r, val);
        if(ret!=-1) return ret;
        return query_bsr(node*2, tl, mid, l, r, val);
    }
}seg;

int main2()
{
    cin >> N;
    for(int i=1; i<=N; i++) cin >> A[i];

    if(N==1) return !(cout << "1\n");

    ll ans=0;
    seg.init(1, 1, N-1, A);
    for(int l=1, r=1; l<=N; l=r)
    {
        for(; r<=N && A[r]==A[l]; r++);
        for(int i=l; i<r; i++)
        {
            int val=0;
            int p=seg.query_bsl(1, 1, N-1, r-1, N-1, val);
            ans+=r-i;
            if(p!=-1) ans+=N-p;
        }
    }

    cout << ans << "\n";

    return 0;
}

void reset()
{

}

int main()
{
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    
    int TC;
    cin >> TC;
    while(TC--)
    {
        main2();
        reset();
    }
}