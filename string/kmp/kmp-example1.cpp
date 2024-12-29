// https://www.acmicpc.net/problem/1786

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

namespace KMP
{
    // 0-based, O(|S|)
    // kmp(S = "ababacaca") = [ -, 0, 1, 2, 3, 0, 1, 0, 1 ]
    vector<int> kmp(string S)
    {
        int N=S.size();
        vector<int> A(N);
        for(int i=1; i<N; i++)
        {
            int j=A[i-1];
            while(j>0 && S[j]!=S[i]) j=A[j-1];
            if(S[j]==S[i]) A[i]=j+1;
        }
        return A;
    }
}

int main()
{
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    
    string S, T;
    getline(cin, S);
    getline(cin, T);

    S=T+"?"+S;
    auto V = KMP::kmp(S);
    vector<int> ans;
    for(int i=0; i<V.size(); i++) if(V[i]==T.size()) ans.push_back(i-T.size()-T.size()+1);

    cout << ans.size() << "\n";
    for(auto it : ans) cout << it << " ";
    cout << "\n";
}