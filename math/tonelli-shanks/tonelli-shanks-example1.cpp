// https://www.acmicpc.net/problem/17603

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

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

int main()
{
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    
    int TC;
    cin >> TC;
    while(TC--)
    {
        ll P, A, B;
        cin >> P >> B >> A;

        if(P==2)
        {
            if(A==0 && B==0) cout << "0 0\n";
            else if(B==0) cout << "0 1\n";
            else if(A==0) cout << "1 1\n";
            else cout << "-1\n";
        }
        else
        {
            ll x=TonelliShanks::quadratic_residue(((A*A-4*B)%P+P)%P, P);
            if(x==-1) cout << "-1\n";
            else
            {
                ll a=(A+x)*TonelliShanks::mpow(2, P-2, P)%P;
                ll b=(((A-x)%P+P)%P)*TonelliShanks::mpow(2, P-2, P)%P;
                if(a>b) swap(a, b);
                cout << a << " " << b << "\n";
            }
        }
    }
}