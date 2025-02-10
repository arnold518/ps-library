// https://www.acmicpc.net/problem/11657

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

namespace SPFA
{
    // 1-based, directed graph, init : O(V), add_edge : O(1), shortest_path : average O(E)
    // call init() first, add_edge(u, v, w), then call shortest_path(source)
    // distance of shortest path is stored in dist[v]
    // returns true if negative cycle was not detected, false if negative cycle was detected
    // shortest path can be recovered by following par[v] until it returns to source
    // can NOT find negative cycles unreachable from source

    typedef long long T;
    const int MAXV = 1e5;
    const T INF = numeric_limits<T>::max()/2;

    int N;
    vector<pair<int, T>> adj[MAXV+10];
    T dist[MAXV+10];
    int par[MAXV+10], in[MAXV+10];
    bool inq[MAXV+10];

    void init(int _N)
    {
        N=_N;
        for(int i=1; i<=N; i++) adj[i].clear();
    }
    void add_edge(int u, int v, T w) { adj[u].push_back({v, w}); }
    bool is_cycle()
    {
        fill(in+1, in+N+1, 0);
        for(int i=1; i<=N; i++) if(par[i]!=0) in[par[i]]++;
        queue<int> Q;
        for(int i=1; i<=N; i++) if(!in[i]) Q.push(i);
        while(!Q.empty())
        {
            int now=Q.front(); Q.pop();
            if(par[now]!=0)
            {
                in[par[now]]--;
                if(!in[par[now]]) Q.push(par[now]);
            }
        }
        // in order to restore cycle, choose any node s.t. in[i] != 0, then i belongs to cycle
        // then, follow par[i] until it returns to i
        for(int i=1; i<=N; i++) if(in[i]) return true;
        return false;
    }
    bool shortest_path(int S)
    {
        // if S = -1, start from all vertices (for only negative cycle detection)
        fill(dist+1, dist+N+1, INF);
        fill(par+1, par+N+1, 0);
        fill(inq+1, inq+N+1, false);
        deque<int> Q;
        if(S==-1) for(int i=1; i<=N; i++) { dist[i]=0; Q.push_back(i); inq[i]=true; }
        else { dist[S]=0; Q.push_back(S); inq[S]=true; }
        int cnt=0;
        while(!Q.empty())
        {
            int now=Q.front(); Q.pop_front(); inq[now]=false;
            for(auto [nxt, w] : adj[now])
            {
                if(dist[nxt]>dist[now]+w)
                {
                    dist[nxt]=max(dist[now]+w, -INF);
                    par[nxt]=now;
                    if(!inq[nxt])
                    {
                        inq[nxt]=true;
                        if(Q.size() && dist[Q.front()]>dist[nxt]) Q.push_front(nxt);
                        else Q.push_back(nxt);
                    }
                }
            }
            // comment this line if negative cycle detection is not required 
            if(++cnt%N==0 && is_cycle()) return false;
        }
        return true;
    }
}

int main()
{
    ios_base::sync_with_stdio(0); cin.tie(0);
    int N, M;
    cin >> N >> M;
    SPFA::init(N);
    for(int i=1; i<=M; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        SPFA::add_edge(u, v, w);
    }
    if(!SPFA::shortest_path(1)) return !(cout << "-1\n");
    for(int i=2; i<=N; i++)
    {
        if(SPFA::dist[i]==SPFA::INF) SPFA::dist[i]=-1;
        cout << SPFA::dist[i] << "\n";
    }
}