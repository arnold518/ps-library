// https://www.acmicpc.net/problem/9248

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

namespace SuffixAutomaton
{
    // 0-based, build : O(|S|)
    // v.link = suffix link of v
    // v.len = length of longest substring corresponding to v
    // v.pos = one element of endpos(v)
    // v.chd[c] = c-th children node of v in suffix automaton
    // v.flag = true if longest path to v corresponds exactly to prefix S[ ... v.pos]
    // call init() first, and build suffix automaton by calling build(S)
    // all paths start at node 0
    // maximum number of nodes are 2|S|
    // maximum number of edges are 3|S|
    const int CH = 26;
    struct Node
    {
        int link, len, pos;
        vector<int> chd;
        bool flag;
        Node()
        {
            link = len = pos = 0;
            flag = false;
            chd = vector<int>(CH, -1);
        }
    };
    vector<Node> NS;
    void init() { NS.clear(); }
    int newNode()
    {
        NS.push_back(Node());
        return NS.size()-1;
    }
    void build(string S)
    {
        int last=newNode();
        NS[last].link=-1;
        for(int i=0; i<S.size(); i++)
        {
            int c=S[i]-'a';
            int cur=newNode(), p;
            NS[cur].len=NS[last].len+1;
            NS[cur].pos=i;
            NS[cur].flag=true;
            for(p=last; p!=-1 && NS[p].chd[c]==-1; p=NS[p].link) NS[p].chd[c]=cur;
            if(p==-1) NS[cur].link=0;
            else
            {
                int q=NS[p].chd[c];
                if(NS[p].len+1==NS[q].len) NS[cur].link=q;
                else
                {
                    int clone=newNode();
                    NS[clone].len=NS[p].len+1;
                    NS[clone].pos=i;
                    NS[clone].link=NS[q].link;
                    NS[clone].chd=NS[q].chd;
                    for(; p!=-1 && NS[p].chd[c]==q; p=NS[p].link) NS[p].chd[c]=clone;
                    NS[q].link=NS[cur].link=clone;
                }
            }
            last=cur;
        }
    }

    // 0-based, suffix_tree : O(|S|)
    // e.x = child node of edge e in suffix tree
    // [e.l, e.r] = substring of S corresponding to edge e in suffix tree
    // v.flag = true if v corresponds exactly to suffix S[v.pos ... ]
    // call init() first, and build suffix tree by calling suffix_tree(S)
    // all nodes in NS are used in both suffix automaton and suffix tree
    // nodes in suffix tree CAN have ONE child
    struct Edge { int x, l, r; };
    vector<vector<Edge>> adj;
    void suffix_tree(string S)
    {
        reverse(S.begin(), S.end());
        build(S);
        reverse(S.begin(), S.end());
        int N=NS.size();
        adj=vector<vector<Edge>>(N);
        for(int i=0; i<N; i++) NS[i].pos=S.size()-1-NS[i].pos;
        for(int i=1; i<N; i++)
        {
            int x=NS[i].link;
            int l=NS[i].pos+NS[x].len;
            int r=NS[i].pos+NS[i].len-1;
            adj[x].push_back({i, l, r});
        }
        for(int i=0; i<N; i++) sort(adj[i].begin(), adj[i].end(), [&](Edge p, Edge q) { return S[p.l] < S[q.l]; });
    }

    // 0-based, suffix_array : O(|S|)
    // call init() first, and get suffix array of S by calling suffix_array(S, SA, R, LCP)
    void dfs(int now, vector<int> &SA)
    {
        if(NS[now].flag) SA.push_back(NS[now].pos);
        for(auto [nxt, l, r] : adj[now]) dfs(nxt, SA);
    }
    void suffix_array(string S, vector<int> &SA, vector<int> &R, vector<int> &LCP)
    {
        suffix_tree(S);
        dfs(0, SA);
        int N=S.size();
        R=LCP=vector<int>(N);
        for(int i=0; i<N; i++) R[SA[i]]=i;
        for(int i=0, k=0; i<N; i++) if(R[i])
        {
            int j=SA[R[i]-1];
            while(i+k<N && j+k<N && S[i+k]==S[j+k]) k++;
            LCP[R[i]]=k;
            k=max(k-1, 0);
        }
    }
}


int main()
{
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    string S;
    cin >> S;
    vector<int> SA, R, LCP;

    SuffixAutomaton::init();
    SuffixAutomaton::suffix_array(S, SA, R, LCP);
    for(int i=0; i<S.size(); i++) cout << SA[i]+1 << " ";
    cout << "\nx ";
    for(int i=1; i<S.size(); i++) cout << LCP[i] << " ";
    cout << "\n";
}