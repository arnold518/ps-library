namespace LiChao
{
    // |f(x)| < INF, XL <= x <= XR must hold
    const ll INF = 4e18;
    const ll XL = -1e9, XR = 1e9;
    struct Line
    {
        ll a, b;
        Line() : a(0), b(INF) {} // min : INF , max : -INF
        Line(ll _a, ll _b) : a(_a), b(_b) {}
        ll operator () (ll x) { return a*x+b; }
    };

    struct Node
    {
        Line f;
        int lc, rc;
        Node() : f(), lc(-1), rc(-1) {}
    };
    
    struct LiChao
    {
        int root;
        vector<Node> NS;

        LiChao() { NS=vector<Node>(); root=newNode(); }
        int newNode() { NS.push_back(Node()); return NS.size()-1; }

        // update : O(log X), insert line p = ax+b
        void update(int node, ll tl, ll tr, Line p)
        {
            ll mid=tl+tr>>1;
            if(p(mid) < NS[node].f(mid)) swap(p, NS[node].f); // min : < , max : >
            if(tl==tr) return;
            if(p(tl) < NS[node].f(tl)) // min : < , max : >
            {
                if(NS[node].lc==-1) NS[node].lc=newNode();
                update(NS[node].lc, tl, mid, p);
            }
            if(p(tr) < NS[node].f(tr)) // min : < , max : >
            {
                if(NS[node].rc==-1) NS[node].rc=newNode();
                update(NS[node].rc, mid+1, tr, p);
            }
        }
        void update(Line p) { update(root, XL, XR, p); }

        // query : O(log X), get min (or max) value at x
        ll query(int node, ll tl, ll tr, ll x)
        {
            if(node==-1) return INF; // min : INF , max : -INF
            ll mid=tl+tr>>1;
            ll ret=NS[node].f(x);
            if(x<=mid) ret=min(ret, query(NS[node].lc, tl, mid, x)); // min : min() , max : max()
            else ret=min(ret, query(NS[node].rc, mid+1, tr, x)); // min : min() , max : max()
            return ret;
        }
        ll query(ll x) { return query(root, XL, XR, x); }
    };
}