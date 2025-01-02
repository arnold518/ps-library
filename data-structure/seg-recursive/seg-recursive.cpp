// the following must be implemented for normal segment tree
// Data : type for data structure
// Data operator + (Data p, Data q), ID : identity element for Data
// bool f(Data p) : boolean function for binary search (query_bsl, query_bsr)

// the following must be implemented for lazy segment tree
// Data2 : type for lazy update
// Data2 operator + (Data2 p, Data2 q), ID2 : identity element for Data2
// Data apply(Data p, Data2 q, int tl, int tr) : result of update q applied to value p in range [tl, tr]

struct SEG
{
    bool f(Data p) {}
    Data apply(Data p, Data2 q, int tl, int tr) {}

    // tree[node] : query value stored in node
    // lazy[node] : lazy value to be applied to subtree of node (already applied to tree[node])
    Data tree[MAXN*4+10];
    Data2 lazy[MAXN*4+10];

    // apply : O(1), apply update value upd to subtree of node
    void apply(int node, int tl, int tr, Data2 upd)
    {
        tree[node]=apply(tree[node], upd, tl, tr);
        lazy[node]=lazy[node]+upd;
    }

    // push : O(1), propagate lazy value to child, and initialize lazy
    void push(int node, int tl, int tr)
    {
        if(tl!=tr)
        {
            int mid=tl+tr>>1;
            apply(node*2, tl, mid, lazy[node]);
            apply(node*2+1, mid+1, tr, lazy[node]);
        }
        lazy[node]=ID2;
    }

    // init : O(N), initialize tree, lazy
    void init(int node, int tl, int tr, Data *A)
    {
        lazy[node]=ID2;
        if(tl==tr) { tree[node]=A[tl]; return; }
        int mid=tl+tr>>1;
        init(node*2, tl, mid, A);
        init(node*2+1, mid+1, tr, A);
        tree[node]=tree[node*2]+tree[node*2+1];
    }

    // update_point : O(log N), update value at position p to val
    void update_point(int node, int tl, int tr, int p, Data val)
    {
        if(tl==tr) { tree[node]=val; return; }
        push(node, tl, tr);
        int mid=tl+tr>>1;
        if(p<=mid) update_point(node*2, tl, mid, p, val);
        else update_point(node*2+1, mid+1, tr, p, val);
        tree[node]=tree[node*2]+tree[node*2+1];
    }

    // update_range : O(log N), update value at range [l, r] by upd
    void update_range(int node, int tl, int tr, int l, int r, Data2 upd)
    {
        if(r<tl || tr<l) return;
        if(l<=tl && tr<=r)
        {
            apply(node, tl, tr, upd);
            return;
        }
        push(node, tl, tr);
        int mid=tl+tr>>1;
        update_range(node*2, tl, mid, l, r, upd);
        update_range(node*2+1, mid+1, tr, l, r, upd);
        tree[node]=tree[node*2]+tree[node*2+1];
    }

    // query : O(log N), query value at range [l, r]
    Data query(int node, int tl, int tr, int l, int r)
    {
        if(r<tl || tr<l) return ID;
        if(l<=tl && tr<=r) return tree[node];
        push(node, tl, tr);
        int mid=tl+tr>>1;
        return query(node*2, tl, mid, l, r)+query(node*2+1, mid+1, tr, l, r);
    }

    // query_bsl : O(log N), binary search with fixed left point
    // returns leftmost p (l<=p<=r) such that f(query(l, p)) is true
    // f(query(l, p)) must be in the form 00...0011...11 for (l<=p<=r)
    // returns -1 if f(query(l, p)) is false for all p
    int query_bsl(int node, int tl, int tr, int l, int r, Data &val)
    {
        if(r<tl || tr<l) return -1;
        if(l<=tl && tr<=r)
        {
            Data val2=val+tree[node];
            if(!f(val2)) { val=val2; return -1; }
        }
        if(tl==tr) return tl;
        push(node, tl, tr);
        int mid=tl+tr>>1;
        int ret=query_bsl(node*2, tl, mid, l, r, val);
        if(ret!=-1) return ret;
        return query_bsl(node*2+1, mid+1, tr, l, r, val);
    }

    // query_bsr : O(log N), binary search with fixed right point
    // returns rightmost p (l<=p<=r) such that f(query(p, r)) is true
    // f(query(p, r)) must be in the form 11...1100...00 for (l<=p<=r)
    // returns -1 if f(query(p, r)) is false for all p
    int query_bsr(int node, int tl, int tr, int l, int r, Data &val)
    {
        if(r<tl || tr<l) return -1;
        if(l<=tl && tr<=r)
        {
            Data val2=tree[node]+val;
            if(!f(val2)) { val=val2; return -1; }
        }
        if(tl==tr) return tl;
        push(node, tl, tr);
        int mid=tl+tr>>1;
        int ret=query_bsr(node*2+1, mid+1, tr, l, r, val);
        if(ret!=-1) return ret;
        return query_bsr(node*2, tl, mid, l, r, val);
    }
};