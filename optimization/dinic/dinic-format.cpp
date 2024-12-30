struct Dinic {
  // 0-based, directed graph, init : O(V), add_edge : O(1), flow : O(min(V^2 E, EF))
  // call init(V) first, add_edge(u, v, c), then flow(source, sink)
  // adj = residual graph of current flow (v = node, c = remaining capacity, r = index of reverse edge)
  // to get actual flow, you must save original capcity of all edges
  typedef long long T;
  const static int MAXV = 1200;
  const T INF = numeric_limits<T>::max();
  
  struct Edge { int v; T c; int r; };
  int N, src, snk;
  vector<Edge> adj[MAXV + 10];
  int lvl[MAXV + 10], pos[MAXV + 10];
  T lim;

  void init(int _N) {
    N = _N;
    for (int i = 0; i < N; i++) adj[i] = vector<Edge>();
  }
  void add_edge(int u, int v, T c, bool dir = true) {
    // directed edge : dir = true, undirected edge : dir = false
    adj[u].push_back({v, c, adj[v].size()});
    adj[v].push_back({u, dir ? 0 : c, adj[u].size() - 1});
  }
  bool bfs() {
    fill(lvl, lvl + N, 0);
    queue<int> Q;
    Q.push(src);
    lvl[src] = 1;
    while (!Q.empty()) {
      int now = Q.front(); Q.pop();
      for (auto [nxt, c, r] : adj[now]) {
        if (lvl[nxt] || c < lim) continue;
        Q.push(nxt);
        lvl[nxt] = lvl[now] + 1;
      }
    }
    return lvl[snk];
  }
  T dfs(int now, T flow) {
    if (now == snk) return flow;
    for (; pos[now] < adj[now].size(); pos[now]++) {
      auto &[nxt, c, r] = adj[now][pos[now]];
      if (lvl[nxt] != lvl[now] + 1 || !c) continue;
      T f = dfs(nxt, min(flow, c));
      if (f) {
        c -= f;
        adj[nxt][r].c += f;
        return f;
      }
    }
    return 0;
  }
  T flow(int _src, int _snk) {
    src = _src; snk = _snk;
    T ans = 0;
    for (lim = INF; lim > 0; lim >>= 1) {
      while (bfs()) {
        fill(pos, pos + N, 0);
        while (1) {
          T t = dfs(src, INF);
          if (!t) break;
          ans += t;
        }
      }
    }
    return ans;
  }
};