// https://www.acmicpc.net/problem/1786

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

typedef unsigned int ui;
typedef unsigned long long ull;
const ull MOD = 0xffff'ffff'0000'0001;  // primitive root = 7
struct mint {
  ull x;
  mint() : x(0) {}
  mint(ull x) : x(x) {}
  mint operator + (ull ot) const { return x >= MOD - ot ? x - MOD + ot : x + ot; }
  mint operator - (ull ot) const { return x >= ot ? x - ot : x - ot + MOD; }
  mint operator - () const { return x ? MOD - x : 0; }
  mint operator * (ull ot) const {
    unsigned __int128 p = (unsigned __int128)x * ot;
    ull a = (ull)p;
    ull b = (ui)(p >> 64);
    ull c = p >> 96;
    return mint(a) - b - c + (b << 32);
  }
  mint operator += (ull ot) { return *this = *this + ot; }
  mint operator -= (ull ot) { return *this = *this - ot; }
  mint operator *= (ull ot) { return *this = *this * ot; }
  operator unsigned long long() const { return x; }
};
mint mpow(mint a, ull b)
{
    mint ret=1;
    while(b)
    {
        if(b&1) ret*=a;
        a*=a; b>>=1;
    }
    return ret;
}
mint inv(mint a) { return mpow(a, MOD-2); }

// include mint (Ultimate Mod)
struct Hash
{
    // 0-based, constructor : O(|S|), get_hash : O(1)
    // get(l, r) = hash for S[l ... r] (inclusive), 0 if l>r
    int N;
    mint B;
    vector<mint> P, IP, V;
    Hash(string S, mint _B)
    {
        B=_B;
        N=S.size();
        P=IP=V=vector<mint>(N+1);
        P[0]=IP[0]=1;
        P[1]=B; IP[1]=inv(B);
        for(int i=2; i<=N; i++) P[i]=P[i-1]*P[1], IP[i]=IP[i-1]*IP[1];
        for(int i=1; i<=N; i++) V[i]=V[i-1]+P[i]*S[i-1];
    }
    mint get_hash(int l, int r) { return (l<=r ? (V[r+1]-V[l])*IP[l] : mint(0)); } 
};

int main()
{
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    string S, T;
    getline(cin, S); getline(cin, T);
    Hash HS(S, 7), HT(T, 7);
    vector<int> ans;
    for(int i=0; i+T.size()-1<S.size(); i++) if(HT.get_hash(0, T.size()-1)==HS.get_hash(i, i+T.size()-1)) ans.push_back(i+1);

    cout << ans.size() << "\n";
    for(auto it : ans) cout << it << " ";
    cout << "\n";
}