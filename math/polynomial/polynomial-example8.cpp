// https://judge.yosupo.jp/problem/polynomial_interpolation

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

namespace NTT
{
    const mint G = 3; // primitive root of MOD

    // dft : O(N log N), 0-based
    // get DFT (IDFT if inv is true) of polynomial F
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
    
    // multiply : O(N log N), 0-based
    // multiplies polynomial F, G
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

namespace TonelliShanks
{
    mt19937 rnd(1557);
    ll rng(ll l, ll r) { return uniform_int_distribution<ll>(l, r)(rnd); }
    ll mpow(ll a, ll b, ll mod)
    {
        ll ret=1;
        for(; b; b>>=1, a=a*a%mod) if(b&1) ret=ret*a%mod;
        return ret;
    }

    // quadratic_residue : O(log^2 P)
    // returns one x such that x^2 = n (mod p)
    // returns -1 if such x does not exist
    // REQUIRED : p is prime, 0 <= n < p
    ll quadratic_residue(ll n, ll p)
    {
        if(n==0) return 0;
        if(p==2) return n;
        if(mpow(n, (p-1)/2, p)!=1) return -1;

        int q=p-1, s=__builtin_ctz(q);
        q>>=s;

        ll x=mpow(n, (q+1)/2, p);
        ll t=mpow(n, q, p);
        ll z=rng(1, p-1);
        while(mpow(z, (p-1)/2, p)!=p-1) z=rng(1, p-1);

        while(t!=1)
        {
            ll i=0, v=t;
            while(v!=1) v=v*v%p, i++;
            ll b=mpow(z, q*(1ll<<(s-i-1)), p);
            x=x*b%p;
            t=t*b%p*b%p;
        }
        return x;
    }
}

// mint must be included
// ntt must be included
// tonelli-shanks must be included (for chksqrt, sqrt)

namespace Polynomial
{
    typedef vector<mint> poly;
    poly operator + (poly p, poly q)
    {
        poly ret(max(p.size(), q.size()));
        for(int i=0; i<p.size(); i++) ret[i]+=p[i];
        for(int i=0; i<q.size(); i++) ret[i]+=q[i];
        return ret;
    }
    poly operator - (poly p, poly q)
    {
        poly ret(max(p.size(), q.size()));
        for(int i=0; i<p.size(); i++) ret[i]+=p[i];
        for(int i=0; i<q.size(); i++) ret[i]-=q[i];
        return ret;
    }
    poly operator * (poly p, poly q)
    {
        return NTT::multiply(p, q);
    }
    poly resize(poly p, int sz)
    {
        poly ret=p;
        ret.resize(sz);
        return ret;
    }

    // inv : O(log N)
    // returns p(x)^{-1} (mod x^N) (same length as p, can have leading zeros)
    // REQUIRED : p[0] != 0
    // inv([5, 4, 3, 2, 1]) = [598946612, 718735934, 862483121, 635682004, 163871793]
    poly inv(poly p)
    {
        assert(p[0]!=0);
        poly ret=poly{inv(p[0])};
        for(int i=1; i<p.size(); i<<=1)
        {
            ret=ret*(poly{2}-resize(p, i+i)*ret);
            ret.resize(i+i);
        }
        ret.resize(p.size());
        return ret;
    }

    poly deriv(poly p)
    {
        poly ret(p.size()-1);
        for(int i=1; i<p.size(); i++) ret[i-1]=p[i]*i;
        return ret;
    }

    poly integr(poly p)
    {
        poly ret(p.size()+1);
        for(int i=0; i<p.size(); i++) ret[i+1]=p[i]*inv(mint(i+1));
        return ret;
    }

    // log : O(N log N)
    // returns log(p(x)) (mod x^N) (same length as p, can have leading zeros)
    // REQUIRED : p[0] = 1
    // log([1, 1, 499122179, 166374064, 291154613]) = [0, 1, 2, 3, 4]
    poly log(poly p)
    {
        assert(p[0]==1);
        poly ret=integr(deriv(p)*inv(p));
        ret.resize(p.size());
        return ret;
    }

    // exp : O(N log N)
    // returns exp(p(x)) (mod x^N) (same length as p, can have leading zeros)
    // REQUIRED : p[0] = 0
    // exp([0, 1, 2, 3, 4]) = [1, 1, 499122179, 166374064, 291154613]
    poly exp(poly p)
    {
        assert(p[0]==0);
        poly ret=poly{1};
        for(int i=1; i<p.size(); i<<=1)
        {
            ret=ret*(poly{1}+resize(p, i+i)-log(resize(ret, i+i)));
            ret.resize(i+i);
        }
        ret.resize(p.size());
        return ret;
    }

    // pow : O(N log N)
    // returns p(x)^k (mod x^N) (same length as p, can have leading zeros)
    // pow([1, 3, 0, 0, 0], 4) = [1, 12, 54, 108, 81]
    poly pow(poly p, ll k)
    {
        if(k==0) { poly ret(p.size()); ret[0]=1; return ret; }
        int t;
        for(t=0; t<p.size() && p[t]==0; t++);
        if((__int128)t*k>=p.size()) return poly(p.size());
        poly p2, ret(p.size());
        mint v=inv(p[t]);
        for(int i=t; i<p.size(); i++) p2.push_back(p[i]*v);
        p2.resize(p.size()-t*k);
        p2=log(p2);
        v=mint(k%MOD);
        for(int i=0; i<p2.size(); i++) p2[i]*=v;
        p2=exp(p2);
        v=mpow(p[t], k);
        for(int i=0; i<p2.size(); i++) ret[i+t*k]=v*p2[i];
        return ret;
    }

    // chksqrt : O(N + log^2 MOD)
    // returns true iff there exists q(x) such that q(x)^2 = p(x) (mod x^N)
    // chksqrt([0, 0, 9, 12]) = true
    // chksqrt([0, 0, 10, 12]) = false
    bool chksqrt(poly p)
    {
        int t;
        for(t=0; t<p.size() && p[t]==0; t++);
        if(t>=p.size()) return true;
        if(t%2) return false;
        if(TonelliShanks::quadratic_residue(p[t], MOD)==-1) return false;
        return true;
    }

    // sqrt : O(N log N)
    // returns q(x) such that q(x)^2 = p(x) (mod x^N) (same length as p, can have leading zeros)
    // REQUIRED : chksqrt(p) must be true
    // sqrt([0, 0, 9, 12]) = [0, 3, 2, 332748117]
    poly sqrt(poly p)
    {
        int t;
        for(t=0; t<p.size() && p[t]==0; t++);
        if(t>=p.size()) return p;
        assert(chksqrt(p));
        poly p2, ret(p.size());
        mint v=inv(p[t]);
        for(int i=t; i<p.size(); i++) p2.push_back(p[i]*v);
        p2.resize(p.size()-t/2);
        p2=log(p2);
        v=inv(mint(2));
        for(int i=0; i<p2.size(); i++) p2[i]*=v;
        p2=exp(p2);
        v=TonelliShanks::quadratic_residue(p[t], MOD);
        for(int i=0; i<p2.size(); i++) ret[i+t/2]=v*p2[i];
        return ret;
    }

    // div : O(N log N)
    // returns {q(x), r(x)} such that f(x) = g(x)q(x) + r(x) (mod x^N)
    // q.size() = f.size()-g.size()+1, r.size() = g.size()-1, can have leading zeros
    // REQUIRED : g.size() > 0
    // div([1, 2, 3, 4], [5, 6, 7]) = {[916755018, 427819009], [407446676, 346329673]}
    pair<poly, poly> div(poly f, poly g)
    {
        assert(g.size()>0);
        if(f.size()<g.size()) return {poly(0), f};
        poly q, r;
        poly fr=f, gr=g;
        reverse(fr.begin(), fr.end());
        reverse(gr.begin(), gr.end());
        int sz=(int)f.size()-(int)g.size()+1;
        q=resize(fr*inv(resize(gr, sz)), sz);
        reverse(q.begin(), q.end());
        r=resize(f-g*q, g.size()-1);
        return {q, r};
    }

    // multi_eval : O(N log^2 N)
    // returns {f(X[0]), f(X[1]), ..., f(X[N-1])}
    // multi_eval([1, 2, 3, 4], [5, 6, 7, 8, 9]) = [586, 985, 1534, 2257, 3178]
    vector<mint> multi_eval(poly f, vector<mint> X)
    {
        int N=X.size();
        vector<poly> tree(N+N);
        for(int i=0; i<N; i++) tree[N+i]=poly{-X[i], 1};
        for(int i=N-1; i>0; i--) tree[i]=tree[i<<1]*tree[i<<1|1];
        tree[1]=div(f, tree[1]).second;
        for(int i=2; i<N+N; i++) tree[i]=div(tree[i>>1], tree[i]).second;
        vector<mint> ret(N);
        for(int i=0; i<N; i++) ret[i]=tree[N+i][0];
        return ret;
    }

    // interpolation : O(N log^2 N)
    // returns p(x) such that p(X[i]) = Y[i] for all i, p.size() = N
    // interpolation([5, 6, 7, 8, 9], [586, 985, 1534, 2257, 3178]) = [1, 2, 3, 4, 0]
    poly interpolation(vector<mint> X, vector<mint> Y)
    {
        int N=X.size();
        vector<poly> tree(N+N), tree2(N+N);
        for(int i=0; i<N; i++) tree[N+i]=poly{-X[i], 1};
        for(int i=N-1; i>0; i--) tree[i]=tree[i<<1]*tree[i<<1|1];
        vector<mint> P=multi_eval(deriv(tree[1]), X);
        for(int i=0; i<N; i++) tree2[i+N]=poly{Y[i]*inv(P[i])};
        for(int i=N-1; i>0; i--) tree2[i]=tree[i<<1]*tree2[i<<1|1]+tree2[i<<1]*tree[i<<1|1];
        return tree2[1];
    }
}

int main()
{
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    
    int N;
    cin >> N;
    vector<mint> A(N), B(N);
    for(int i=0; i<N; i++) cin >> A[i].x;
    for(int i=0; i<N; i++) cin >> B[i].x;
    auto C = Polynomial::interpolation(A, B);
    for(int i=0; i<C.size(); i++) cout << C[i].x << " "; cout << "\n";
}