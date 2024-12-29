// https://www.acmicpc.net/problem/9250

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

namespace AhoCorasick
{
    // 0-based, calc : O(sum|T_i|), aho_corasick : O(|S| + sum|T_i|)
    // v.fail = failure link of node v
    // v.suf = longest suffix of node v corresponding to whole inserted string (if none, root)
    // v.chd[c] = c-th children of node v in trie
    // v.jmp[c] = c-th next match of node v in automaton
    // call init() first, and insert strings using insert(T_i)
    // then call calc() to calculate fail, suf, chd, jump, and then call aho_corasick(S)
    // aho_corasick(S) : Find occurences of TV (strings inserted in trie) in S (ending position)
    // aho_corasick(S = "mississippi", TV = ["ss", "sis", "ippi", "pp"]) = [3, 5, 6, 9, 10]
    const int CH = 26;
    struct Node
    {
        int par, fail, suf;
        vector<int> chd, jmp;
        Node()
        {
            par=fail=suf=-1;
            chd=vector<int>(CH, -1);
            jmp=vector<int>(CH, -1);
        }
    };
    int root;
    vector<Node> NS;
    int newNode()
    {
        NS.push_back(Node());
        return NS.size()-1;
    }
    void init()
    {
        NS.clear();
        root=newNode();
        NS[root].par=root; NS[root].fail=root; NS[root].suf=root;
    }
    void insert(const string &S)
    {
        int now=root;
        for(auto c : S)
        {
            if(NS[now].chd[c-'a']==-1)
            {
                int nxt=NS[now].chd[c-'a']=newNode();
                NS[nxt].par=now;
            }
            now=NS[now].chd[c-'a'];
        }
        NS[now].suf=now;
    }
    void calc()
    {
        queue<int> Q;
        Q.push(root);
        while(!Q.empty())
        {
            int now=Q.front(); Q.pop();
            for(int i=0; i<CH; i++) if(NS[now].chd[i]!=-1)
            {
                int nxt=NS[now].chd[i];
                NS[now].jmp[i]=nxt;
                if(now==root) NS[nxt].fail=root;
                else
                {
                    int p;
                    for(p=NS[now].fail; p!=root && NS[p].chd[i]==-1; p=NS[p].fail);
                    if(NS[p].chd[i]==-1) NS[nxt].fail=root;
                    else NS[nxt].fail=NS[p].chd[i];
                }
                if(NS[nxt].suf==-1) NS[nxt].suf=NS[NS[nxt].fail].suf;
                Q.push(nxt);
            }

            for(int i=0; i<CH; i++)
            {
                if(NS[now].chd[i]!=-1) NS[now].jmp[i]=NS[now].chd[i];
                else
                {
                    if(now==root) NS[now].jmp[i]=now;
                    else NS[now].jmp[i]=NS[NS[now].fail].jmp[i];
                }
            }
        }
    }
    vector<int> aho_corasick(string S)
    {
        vector<int> ans;
        int now=root;
        for(int i=0; i<S.size(); i++)
        {
            now=NS[now].jmp[S[i]-'a'];

            // now is matching node in trie with S[0...i]
            // your code goes here

            if(NS[now].suf!=root) ans.push_back(i);
        }
        return ans;
    }
}

int main()
{
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    int N, Q;
    cin >> N;

    AhoCorasick::init();
    for(int i=0; i<N; i++)
    {
        string T;
        cin >> T;
        AhoCorasick::insert(T);
    }
    AhoCorasick::calc();

    cin >> Q;
    while(Q--)
    {
        string S;
        cin >> S;
        vector<int> ans = AhoCorasick::aho_corasick(S);
        if(!ans.empty()) cout << "YES\n";
        else cout << "NO\n";
    }
}