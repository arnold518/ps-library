// https://www.acmicpc.net/problem/16563

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

namespace LinearSieve
{
    const int MAXN = 5e6;

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
        P[0]=P[1]=false;

        for(int i=2; i<=N; i++)
        {
            if(P[i])
            {
                VP.push_back(i);
                SP[i]=i;
                // SP2[i]=i;
                // mul[i]=i-1; // mul[p] = ??? ( phi[p] = p-1 )
            }
            for(int p : VP)
            {
                ll x=(ll)i*p;
                if(x>N) break;
                P[x]=false;
                SP[x]=p;

                if(i%p==0)
                {
                    // SP2[x]=SP2[i]*p;
                    // if(x==SP2[x]) mul[x]=mul[i]*p; // mul[p^(k+1)] = ??? ( phi[p^(k+1)] = phi[p^k]*p )
                    // else mul[x]=mul[i]/mul[SP2[i]]*mul[SP2[x]];
                    break;
                }
                // SP2[x]=p;
                // mul[x]=mul[i]*mul[p];
            }
        }
    }
};

int main()
{
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    int M=5e6;
    LinearSieve::sieve(M);
    int N;
    cin >> N;
    while(N--)
    {
        int t;
        cin >> t;
        while(t>1)
        {
            int p=LinearSieve::SP[t];
            cout << p << " ";
            t/=p;
        }
        cout << "\n";
    }
}