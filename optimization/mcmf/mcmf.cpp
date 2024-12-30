struct MCMF
{
    // 0-based, directed graph, init : O(V), add_edge : O(1), flow : O(EF)
    // call init() first, add_edge(u, v, c = capcity, w = cost), then flow(source, sink)
    // flow(source, sink) returns (min cost when max flow, max flow)
    // adj = residual graph of current flow (v = node, c = remaining capacity, w = cost, r = index of reverse edge)
    // to get actual flow, you must save original capcity of all edges
    typedef long long T;
    const static int MAXV = 400;
    const T INF = numeric_limits<T>::max();

    struct Edge { int v; T c, w; int r; };
    int N, src, snk;
    vector<Edge> adj[MAXV+10];
    T dist[MAXV+10];
    int par[MAXV+10];
    bool inq[MAXV+10];

    void init(int _N)
    {
        N=_N;
        for(int i=0; i<N; i++) adj[i].clear();
    }
    void add_edge(int u, int v, T c, T w, bool dir = true) // directed edge : dir = true, undirected edge : dir = false
    {
        adj[u].push_back({v, c, w, adj[v].size()});
        adj[v].push_back({u, dir ? 0 : c, -w, adj[u].size()-1});
    }
    bool shortest_path()
    {
        fill(dist, dist+N, INF);
        fill(par, par+N, -1);
        fill(inq, inq+N, false);
        deque<int> Q;
        dist[src]=0; inq[src]=true; Q.push_back(src);
        int cnt=0;
        while(Q.size())
        {
            int x=Q.front(); Q.pop_front();
            inq[x]=false;
            for(auto [y, c, w, r] : adj[x])
            {
                if(c==0) continue;
                if(dist[y]>dist[x]+w)
                {
                    dist[y]=max(dist[x]+w, -INF);
                    par[y]=r;
                    if(!inq[y])
                    {
                        inq[y]=true;
                        if(Q.size() && dist[Q[0]]>dist[y]) Q.push_front(y);
                        else Q.push_back(y);
                    }
                }
            }
        }
        return dist[snk]!=INF;
    }
    pair<T, T> flow(int _src, int _snk)
    {
        src=_src; snk=_snk;
        T cost=0, flow=0;
        while(shortest_path())
        {
            T val=INF;
            for(int x=snk; x!=src;)
            {
                auto [y, c, w, r] = adj[x][par[x]];
                val=min(val, adj[y][r].c);
                x=y;
            }
            for(int x=snk; x!=src;)
            {
                auto [y, c, w, r] = adj[x][par[x]];
                adj[y][r].c-=val;
                adj[x][par[x]].c+=val;
                x=y;
            }
            // added flow = val
            // added cost = dist[snk] * val (dist[snk] cost per flow)
            flow+=val;
            cost+=dist[snk]*val;
        }
        return {cost, flow};
    }
};