struct HopcroftKarp
{
    // 1-based, unweighted bipartite graph, init : O(V), add_edge : O(1), matching : O(E sqrt V)
    // call init(N, M) first, add_edge(u, v), then matching()
    // left vertices : 1 ~ N, right vertices : 1 ~ M
    // matching() returns size of maximum matching
    // A[u] = for left vertex u, 0 if unmatched, matched right vertex v otherwise
    // B[v] = for right vertex v, 0 if unmatched, matched left vertex u otherwise
    const static int MAXV = 1e5;
    int N, M;
    vector<int> adj[MAXV+10];
    int A[MAXV+10], B[MAXV+10];
    int lvl[MAXV+10], pos[MAXV+10];

    void init(int _N, int _M)
    {
        N=_N; M=_M;
        for(int i=1; i<=N; i++) adj[i]=vector<int>(), A[i]=0;
        for(int i=1; i<=M; i++) B[i]=0;
    }
    void add_edge(int u, int v) { adj[u].push_back(v); }
    void bfs()
    {
        queue<int> Q;
        for(int i=1; i<=N; i++)
        {
            if(A[i]) lvl[i]=0;
            else
            {
                lvl[i]=1;
                Q.push(i);
            }
        }
        while(!Q.empty())
        {
            int now=Q.front(); Q.pop();
            for(auto nxt : adj[now])
            {
                if(!B[nxt] || lvl[B[nxt]]) continue;
                lvl[B[nxt]]=lvl[now]+1;
                Q.push(B[nxt]);
            }
        }
    }
    bool dfs(int now)
    {
        for(; pos[now]<adj[now].size(); pos[now]++)
        {
            int nxt=adj[now][pos[now]];
            if(!B[nxt] || (lvl[B[nxt]]==lvl[now]+1 && dfs(B[nxt])))
            {
                B[nxt]=now;
                A[now]=nxt;
                return true;
            }
        }
        return false;
    }
    int matching()
    {
        int flow=0;
        while(1)
        {
            bfs();
            for(int i=1; i<=N; i++) pos[i]=0;
            int t=0;
            for(int i=1; i<=N; i++) if(!A[i]) t+=dfs(i);
            if(!t) break;
            flow+=t;
        }
        return flow;
    }
};
