// https://judge.yosupo.jp/problem/bitwise_xor_convolution

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

// you should NOT put 'int' as left operand
// mint should be initialized with an integer in [0, MOD - 1].
// ex. mint x = (long long)1e18; << NOT GOOD
const int MOD = 998244353;
struct mint
{
    int x;
    mint(void) : x(0) {}
    mint(int _x) : x(_x) {}
    mint operator + (int ot) const { return x+ot>=MOD ? x+ot-MOD : x+ot; }
    mint operator - (void) const { return x ? MOD-x : 0; }
    mint operator - (int ot) const { return x < ot ? x+MOD-ot : x-ot; }
    mint operator * (int ot) const { return 1ll*x*ot%MOD; }
    mint operator += (int ot) { return *this = *this + ot; }
    mint operator -= (int ot) { return *this = *this - ot; }
    mint operator *= (int ot) { return *this = *this * ot; }
    operator int(void) const { return x; }
};

mint mpow(mint a, ll b)
{
    mint ret=1;
    while(b)
    {
        if(b&1) ret=ret*a;
        a=a*a; b>>=1;
    }
    return ret;
}
mint inv(mint x) { return mpow(x, MOD-2); }
mint I2 = inv(2);

void dft(vector<mint> &F, bool inv)
{
    int N=F.size();
    for(int d=1; d<N; d<<=1)
    {
        for(int i=0; i<N; i+=d+d)
        {
            for(int j=0; j<d; j++)
            {
                mint a=F[i|j], b=F[i|j|d];

                // XOR
                // F[i|j]=a+b, F[i|j|d]=a-b;
                // if(inv) F[i|j]*=I2, F[i|j|d]*=I2;
                
                // AND
                F[i|j]=b, F[i|j|d]=a+b;
                if(inv) F[i|j]=-a+b, F[i|j|d]=a;

                // OR
                // F[i|j]=a, F[i|j|d]=a+b;
                // if(inv) F[i|j]=a, F[i|j|d]=-a+b;
            }
        }
    }
}

int main()
{
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    
    int N;
    cin >> N;
    vector<mint> A(1<<N), B(1<<N);
    for(auto &it : A) cin >> it.x;
    for(auto &it : B) cin >> it.x;

    dft(A, false); dft(B, false);
    for(int i=0; i<(1<<N); i++) A[i]*=B[i];
    dft(A, true);

    for(auto it : A) cout << it << " ";
    cout << "\n";
}