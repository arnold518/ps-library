// https://www.acmicpc.net/problem/10067

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

namespace CHT
{
    typedef __int128 dll;
    struct Line { ll a, b; int p; };
    struct Frac
    {
        ll u, d;
        Frac(ll _u, ll _d) : u(_u), d(_d) { if(d<0) u=-u, d=-d; }
        bool operator < (const Frac &ot) const { return (dll)u*ot.d < (dll)ot.u*d; }
        bool operator > (const Frac &ot) const { return (dll)u*ot.d > (dll)ot.u*d; }
    };
    ll divfloor(ll u, ll d) { return u/d - ((u^d)<0 && u%d); }
    ll divceil(ll u, ll d) { return u/d + ((u^d)>0 && u%d); }

    // min/max : ?, slope : ?, dir : ?
    struct CHT
    {
        // Get cross point of line p, q
        // If all queries are integer, change 'Frac' to 'div'
        Frac cross(const Line &p, const Line &q) { return Frac(p.b-q.b, q.a-p.a); } // dir + : divfloor, dir - : divceil
        deque<Line> V;

        // Insert line p = ax+b
        // b must be increasing (or decreasing) ('slope')
        // cross(V[i-1], V[i]) < (or >) cross(V[i], V[i+1]) ('dir')
        void push(Line p)
        {
            if(!V.empty() && V.back().a==p.a)
            {
                if(V.back().b <= p.b) return; // min : <= , max : >=
                V.pop_back();
            }
            while(V.size()>1 && !(cross(V[V.size()-2], V[V.size()-1]) < cross(V[V.size()-1], p))) V.pop_back(); // dir + : <, dir - : >
            V.push_back(p);
        }

        // Get min (or max) value at x in O(logN)
        int query(ll x)
        {
            assert(!V.empty());
            int lo=0, hi=V.size();
            while(lo+1<hi)
            {
                int mid=lo+hi>>1;
                if(cross(V[mid-1], V[mid]) < Frac(x, 1)) lo=mid; // dir + : <, dir - : >
                else hi=mid;
            }
            // return V[lo].a*x+V[lo].b;
            return V[lo].p;
        }

        // Get min (or max) value at x in ammortized O(1)
        // x must be increasing (or decreasing) ('dir')
        int query2(ll x)
        {
            assert(!V.empty());
            while(V.size()>1 && cross(V[0], V[1]) < Frac(x, 1)) V.pop_front(); // dir + : <, dir - : >
            // return V[0].a*x+V[0].b;
            return V[0].p;
        }
    };
}

const int MAXN = 1e5;
int N, K;
ll A[MAXN+10];

namespace Alien
{
    // MAXN must be defined
    const int MAXN = 1e5;
    int N;
    
    // dp[i] : optimal dp value of i
    // cnt[i] : how much transitions were used in optimal dp[i]
    // memo[i] : previous transition position from i
    // V : restored optimal solution
    ll dp[MAXN+10];
    int cnt[MAXN+10], memo[MAXN+10];
    vector<int> V;

    void init(int _N)
    {
        N=_N;
    }

    // For given lambda, calculate dp, cnt, memo, V
    // dp[i] = min(or max)_{j<i} (dp[j] + cost(j, i)*2 - lambda) 
    void solve(ll lambda)
    {
        // initialize dp, cnt, memo, V, (and other data structures)
        for(int i=0; i<=N; i++) dp[i]=cnt[i]=memo[i]=0;
        V.clear();

        CHT::CHT cht;
        cht.push({0, 0, 0});

        for(int i=1; i<=N; i++)
        {
            // get_opt(i), cost(p, q) must be implemented
            // opt = argmin(or max)_{j<i} (dp[j] + cost(j, i)*2)
            int opt = cht.query2(A[i]);
            dp[i] = dp[opt] + (A[i]-A[opt])*(A[i]-A[opt])*2 - lambda; // Don't forget *2
            cnt[i] = cnt[opt]+1;
            memo[i] = opt;
            cht.push({-4*A[i], 2*A[i]*A[i]+dp[i], i});
        }

        for(int i=N; i>0;)
        {
            V.push_back(i);
            i=memo[i];
        }
        V.push_back(0);
        reverse(V.begin(), V.end());
    }

    // Get optimal dp[N][K] for given K
    // Returns (answer, restored solution)
    // dp[i][k] = min(or max)_{j<i} (dp[j][k-1] + cost(j, i))
    pair<ll, vector<int>> alien(int K)
    {
        // lambda equals slope
        // minimum : K increase, lambda increase
        // maximum : k increase, lambda decrease

        ll lo=-1e18, hi=1e18; // range for lambda is [2*lo+1, 2*hi+1]
        while(lo+1<hi)
        {
            ll mid=lo+hi>>1;
            solve(2*mid+1);
            if(K <= cnt[N]) hi=mid; // min : <= , max : >=
            else lo=mid;
        }
        
        vector<int> P1, P2, ansV;
        solve(2*lo+1); P1=V;
        solve(2*hi+1); P2=V;
        if(P1.size()>P2.size()) swap(P1, P2);

        if(P1.size()-1==K) ansV=P1;
        else if(P2.size()-1==K) ansV=P2;
        else
        {
            assert(P1.size()-1<K && K<P2.size()-1);
            int x=K+1-P1.size();
            for(int i=0; i+1<P1.size() && i+x+1<P2.size(); i++)
            {
                if(P1[i]<=P2[i+x] && P2[i+x+1]<=P1[i+1])
                {
                    for(int j=0; j<=i+x; j++) ansV.push_back(P2[j]);
                    for(int j=i+1; j<P1.size(); j++) ansV.push_back(P1[j]);
                    break;
                }
            }
        }
        assert(ansV.size()==K+1);

        solve(2*hi);
        ll ans=dp[N]/2+hi*K;
        return {ans, ansV};
    }
}

int main()
{
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    cin >> N >> K; K++;
    for(int i=1; i<=N; i++) cin >> A[i];
    for(int i=1; i<=N; i++) A[i]+=A[i-1];
    Alien::init(N);

    auto [ans, ansV] = Alien::alien(K);
    ans=(A[N]*A[N]-ans)/2;
    cout << ans << "\n";
    for(int i=1; i+1<ansV.size(); i++) cout << ansV[i] << " ";
    cout << "\n";
}