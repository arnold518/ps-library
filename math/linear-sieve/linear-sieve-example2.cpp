// https://codeforces.com/contest/1717/problem/E

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

namespace LinearSieve
{
    const int MAXN = 1e5;

    // P[x] = true if x is prime number
    // VP = vector of prime numbers
    // SP[x] = smallest prime divisor of x
    // SP2[x] = maximum power of smallest prime divisor of x
    // mul[x] = multiplicative function value of x
    bool P[MAXN+10];
    vector<int> VP;
    int SP[MAXN+10], SP2[MAXN+10];
    ll mul[MAXN+10];

    // sieve : O(N)
    // finds primes, and smallest prime divisor for x less or equal than N
    // calculates values for multiplicative function
    void sieve(int N)
    {
        VP.clear();
        fill(P, P+N+1, true);
        P[0]=P[1]=false; mul[1]=1;

        for(int i=2; i<=N; i++)
        {
            if(P[i])
            {
                VP.push_back(i);
                SP[i]=i;
                SP2[i]=i;
                mul[i]=i-1; // mul[p] = ??? ( phi[p] = p-1 )
            }
            for(int p : VP)
            {
                ll x=(ll)i*p;
                if(x>N) break;
                P[x]=false;
                SP[x]=p;

                if(i%p==0)
                {
                    SP2[x]=SP2[i]*p;
                    if(x==SP2[x]) mul[x]=mul[i]*p; // mul[p^(k+1)] = ??? ( phi[p^(k+1)] = phi[p^k]*p )
                    else mul[x]=mul[i]/mul[SP2[i]]*mul[SP2[x]];
                    break;
                }
                SP2[x]=p;
                mul[x]=mul[i]*mul[p];
            }
        }
    }
};

ll lcm(ll a, ll b) { return a*b/__gcd(a, b); }

int main()
{
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    int M=1e5;
    LinearSieve::sieve(M);
    ll N;
    cin >> N;

    int MOD = 1e9+7;
    ll ans=0;
    for(int i=1; i<=N; i++)
    {
        for(int j=i+i; j<N; j+=i)
        {
            int c=N-j;
            if(1<=c && c<=N-2)
            {
                (ans+=lcm(c, i)*LinearSieve::mul[j/i])%=MOD;
            }
        }
    }
    cout << ans << "\n";
}