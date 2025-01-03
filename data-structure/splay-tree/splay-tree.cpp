// the following must be implemented
// Data : type for data structure
// Data operator + (Data p, Data q), ID : identity element for Data
// Data2 : type for lazy update
// Data2 operator + (Data2 p, Data2 q), ID2 : identity element for Data2
// Data apply(Data p, Data2 q, int sz) : result of update q applied to value p in range [tl, tr]

namespace SplayTree
{
    Data apply(Data p, Data2 q, int sz) {}

    struct Node
    {
        int sz;
        int par, lc, rc;
        
        // val : value stored in node, sum : query value stored in node
        // lazy : lazy value to be applied to subtree of node (already applied to val, sum)
        Data val, sum;
        bool rev;
        Data2 lazy;

        Node(Data x)
        {
            sz=1;
            par=lc=rc=0;

            // your code goes here
            val=sum=x;
            rev=false;
            lazy=ID2;
        }
        Node()
        {
            sz=par=lc=rc=0;
            val=sum=ID;
            rev=false;
            lazy=ID2;
        }
    };

    int root;
    vector<Node> NS;
    void init() { NS=vector<Node>(1); root=0; }
    int newNode(Data x) { NS.push_back(Node(x)); return NS.size()-1; }

    // root must be initialized
    // NS[0] : NIL node

    // recalc : recalculate value of node from lc, rc
    // lazy must be empty
    // changes sz, sum
    void recalc(int node)
    {
        if(node==0) return;
        int l=NS[node].lc, r=NS[node].rc;
        NS[node].sz=NS[l].sz+NS[r].sz+1;

        NS[node].sum=NS[l].sum+NS[node].val+NS[r].sum;
    }

    // apply : apply update value upd to subtree of node
    // update to node itself (val, sum), and to lazy value (lazy)
    // changes lazy, val, sum
    void apply(int node, bool rev, Data2 lazy)
    {
        if(node==0) return;

        NS[node].rev^=rev;
        NS[node].lazy=NS[node].lazy+lazy;

        if(rev) swap(NS[node].lc, NS[node].rc);
        NS[node].val=apply(NS[node].val, lazy, 1);
        NS[node].sum=apply(NS[node].sum, lazy, NS[node].sz);
    }

    // push : propagate lazy value to lc, rc, and initialize lazy
    // should be called before going down to children
    // changes lazy
    void push(int node)
    {
        if(node==0) return;

        apply(NS[node].lc, NS[node].rev, NS[node].lazy);
        apply(NS[node].rc, NS[node].rev, NS[node].lazy);
        NS[node].rev=false;
        NS[node].lazy=ID2;
    }

    // push_anc : propagates all ancestors of x
    void push_anc(int x)
    {
        if(NS[x].par!=0) push_anc(NS[x].par);
        push(x);
    }
    
    // rotate : rotate x with its parent
    void rotate(int x)
    {
        assert(x!=0 && NS[x].par!=0);
        int p=NS[x].par, q;
        if(NS[p].lc==x)
        {
            NS[p].lc=q=NS[x].rc;
            NS[x].rc=p;
        }
        else
        {
            NS[p].rc=q=NS[x].lc;
            NS[x].lc=p;
        }
        NS[x].par=NS[p].par;
        NS[p].par=x;
        if(q) NS[q].par=p;
        if(NS[x].par!=0)
        {
            if(NS[NS[x].par].lc==p) NS[NS[x].par].lc=x;
            else NS[NS[x].par].rc=x;
        }

        recalc(p);
        recalc(x);
    }

    // splay : ammortized O(log N), make x the root of tree
    // should be called after consuming time to visit any internal node
    void splay(int x)
    {
        root=x;
        if(x==0) return;
        push_anc(x);
        while(NS[x].par)
        {
            int p=NS[x].par, q=NS[p].par;
            if(q) rotate((NS[p].lc==x)==(NS[q].lc==p) ? p : x);
            rotate(x);
        }
    }

    // find_kth : ammortized O(log N)
    // find kth node in subtree of node, and make it root
    int find_kth(int node, int k, int &root)
    {
        if(node==0) return 0;
        assert(1<=k && k<=NS[node].sz);
        push(node);
        if(k<=NS[NS[node].lc].sz) return find_kth(NS[node].lc, k, root);
        if(k==NS[NS[node].lc].sz+1) { splay(node); return root=node; }
        return find_kth(NS[node].rc, k-NS[NS[node].lc].sz-1, root);
    }

    // insert : ammortized O(log N)
    // insert node x after the kth node in subtree of node, and make it root
    void insert(int node, int k, int x, int &root)
    {
        assert(0<=k && k<=NS[node].sz);
        push(node);

        if(k<=NS[NS[node].lc].sz)
        {
            if(NS[node].lc==0)
            {
                NS[node].lc=x;
                NS[x].par=node;
                splay(x);
                root=x;
            }
            else insert(NS[node].lc, k, x, root);
        }
        else
        {
            if(NS[node].rc==0)
            {
                NS[node].rc=x;
                NS[x].par=node;
                splay(x);
                root=x;
            }
            else insert(NS[node].rc, k-NS[NS[node].lc].sz-1, x, root);
        }
    }

    // erase : ammortized O(log N)
    // erase root of tree
    void erase(int &root)
    {
        if(root==0) return;
        push(root);
        int p=NS[root].lc, q=NS[root].rc;
        if(p==0 || q==0)
        {
            root=p+q;
            NS[root].par=0;
            return;
        }
        root=p;
        NS[root].par=0;
        find_kth(root, NS[p].sz, root);
        NS[root].rc=q;
        NS[q].par=root;
        recalc(root);
    }

    // interval : ammortized O(log N)
    // merge [l, r]th nodes into subtree of NS[NS[root].lc].rc, and return it
    int interval(int l, int r, int &root)
    {
        assert(1<=l && r<=NS[root].sz);
        int sz=NS[root].sz, ret, x;

        if(r<sz)
        {
            find_kth(root, r+1, root);
            x=root;
            root=NS[x].lc;
            NS[root].par=0;
        }

        if(l>1)
        {
            find_kth(root, l-1, root);
            ret=NS[root].rc;
        }
        else ret=root;

        if(r<sz)
        {
            NS[root].par=x;
            NS[x].lc=root;
            root=x;
        }
        return ret;
    }

    // update : ammortized O(log N)
    // update val to range [l, r]
    void update(int l, int r, bool rev, Data2 lazy, int &root)
    {
        assert(1<=l && r<=NS[root].sz);
        int p=interval(l, r, root);
        apply(p, rev, lazy);
        recalc(NS[p].par);
        recalc(NS[NS[p].par].par);
    }
    
    // query : ammortized O(log N)
    // query range [l, r]
    Node query(int l, int r, int &root)
    {
        assert(1<=l && r<=NS[root].sz);
        int p=interval(l, r, root);
        return NS[p];
    }
}