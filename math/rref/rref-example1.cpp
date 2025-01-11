// https://judge.yosupo.jp/problem/inverse_matrix

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

typedef vector<vector<mint>> mat;

// rref : O(NM min(N, M)), 0-based
// gets rref of matrix A
void rref(mat &A)
{
    int N=A.size(), M=A[0].size();
    for(int y=0, x=0; x<M && y<N; x++)
    {
        int row=-1;
        for(int i=y; i<N; i++) if(A[i][x]!=0) row=i;
        if(row==-1) continue;

        for(int i=x; i<M; i++) swap(A[y][i], A[row][i]);
        mint t=inv(A[y][x]);
        for(int i=x; i<M; i++) A[y][i]*=t;
        for(int i=0; i<N; i++) if(i!=y) for(int j=M-1; j>=x; j--) A[i][j]-=A[i][x]*A[y][j];
        y++;
    }
}

int main()
{
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    int N;
    cin >> N;
    mat A=mat(N, vector<mint>(N+N));
    for(int i=0; i<N; i++) for(int j=0; j<N; j++)
    {
        int t;
        cin >> t;
        A[i][j]=t;
    }
    for(int i=0; i<N; i++) A[i][i+N]=1;
    rref(A);
    for(int i=0; i<N; i++) if(A[i][i]!=1) return !(cout << "-1\n");
    for(int i=0; i<N; i++) { for(int j=0; j<N; j++) cout << A[i][j+N] << " "; cout << "\n"; }
}