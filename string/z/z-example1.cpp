// https://judge.yosupo.jp/problem/zalgorithm

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

namespace Z
{
    // Z[i] = LCP(S[i...], S)
    // get_z(S = "ababacaca") = [ -, 0, 3, 0, 1, 0, 1, 0, 1 ]
    vector<int> get_z(string S)
    {
        int N=S.size();
        vector<int> A(N);
        for(int i=1, j=0; i<N; i++)
        {
            if(j+A[j]>i) A[i]=min(j+A[j]-i, A[i-j]);
            while(i+A[i]<N && S[A[i]]==S[i+A[i]]) A[i]++;
            if(i+A[i]>j+A[j]) j=i;
        }
        return A;
    }
}

int main()
{
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    string S;
    cin >> S;
    auto V = Z::get_z(S);
    V[0]=S.size();
    for(auto it : V) cout << it << " ";
    cout << "\n";
}
