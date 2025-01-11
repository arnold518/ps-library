// https://www.acmicpc.net/problem/32121

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

const int MOD = 998244353;

struct mint
{
    int x;
    mint() : x(0) {}
    mint(int _x) : x(_x) {}
    mint operator + (int ot) const { return x+ot>=MOD ? x+ot-MOD : x+ot; }    
    mint operator - (int ot) const { return x<ot ? x+MOD-ot : x-ot; }
    mint operator * (int ot) const { return 1ll*x*ot%MOD; }
    mint operator += (int ot) { return *this = *this + ot; }
    mint operator -= (int ot) { return *this = *this - ot; }
    mint operator *= (int ot) { return *this = *this * ot; }
    operator int() const { return x; }
};
mint mpow(mint a, ll b)
{
    mint ret=1;
    while(b)
    {
        if(b&1) ret*=a;
        a*=a; b>>=1;
    }
    return ret;
}

// mint must be included

namespace NTT
{
    const mint G = 3; // primitive root of MOD

    // Get DFT (IDFT if inv is true) of polynomial F
    // length of A must be power of 2
    // dft([1, 2, 3, 4], false) = [10, 173167434, 998244351, 825076915]
    // dft([10, 173167434, 998244351, 825076915], true) = [1, 2, 3, 4]
    void dft(vector<mint> &F, bool inv)
    {
        int N=F.size(), B=__lg(N);
        vector<int> rev(N);
        for(int i=0; i<N; i++)
        {
            rev[i]=(rev[i>>1]>>1)|((i&1)<<(B-1));
            if(i<rev[i]) swap(F[i], F[rev[i]]);
        }
        vector<mint> W(N);
        W[0]=W[1]=1;
        for(int k=2; k<N; k<<=1)
        {
            mint Z[2]={1, mpow(G, inv ? MOD-1-(MOD-1)/k/2 : (MOD-1)/k/2)};
            for(int i=k; i<(k<<1); i++) W[i]=W[i>>1]*Z[i&1];
        }
        for(int d=1; d<N; d<<=1)
        {
            for(int i=0; i<N; i+=d+d)
            {
                for(int j=0; j<d; j++)
                {
                    mint b=F[i|j|d]*W[j|d];
                    F[i|j|d]=F[i|j]-b;
                    F[i|j]+=b;
                }
            }
        }
        if(inv)
        {
            mint val=mpow(N, MOD-2);
            for(int i=0; i<N; i++) F[i]*=val;
        }
    }
    
    // Multiplies polynomial F, G
    // multiply([1, 2, 3], [4, 5, 6, 7, 8]) = [4, 13, 28, 34, 40, 37, 24]
    vector<mint> multiply(vector<mint> F, vector<mint> G)
    {
        int N=1;
        vector<mint> ret(F.size()+G.size()-1);
        while(N<ret.size()) N<<=1;
        F.resize(N); G.resize(N);
        dft(F, false); dft(G, false);
        for(int i=0; i<N; i++) F[i]*=G[i];
        dft(F, true);
        for(int i=0; i<ret.size(); i++) ret[i]=F[i];
        return ret;
    }
};

const int MAXN = 2e5;

int N;
mint fac[MAXN+10];

vector<mint> dnc(int l, int r)
{
    if(l==r) return vector<mint>{l*2, 1};
    int mid=l+r>>1;
    return NTT::multiply(dnc(l, mid), dnc(mid+1, r));
}

int main()
{
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    
    cin >> N;
    fac[0]=1;
    for(int i=1; i<=N; i++) fac[i]=fac[i-1]*i;

    auto A=dnc(0, N-1);
    cout << N << "\n";
    for(int i=1; i<=N; i++) cout << i << " " << A[i]*fac[N] << "\n";
}