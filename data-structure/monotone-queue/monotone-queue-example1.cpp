// https://www.acmicpc.net/problem/17439

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

const int MAXN = 5e4;
int N, K;
ll A[MAXN+10];

namespace Alien_Monotone_Queue
{
    // MAXN must be defined
    const int MAXN = 5e5;
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

    // cost(p, q) must be monge
    ll cost(int p, int q) { return (q-p)*(A[q]-A[p]); }

    // Get maximum x that dp[p]+cost(p, x) < dp[q]+cost(q, x) (p is optimal than q)
    // p < q must hold
    int cross(int p, int q)
    {
        int lo=q, hi=N+1;
        while(lo+1<hi)
        {
            int mid=lo+hi>>1;
            if(dp[p]+cost(p, mid)*2 <= dp[q]+cost(q, mid)*2) lo=mid;
            else hi=mid;
        }
        return lo;
    }

    // For given lambda, calculate dp, cnt, memo, V
    // dp[i] = min(or max)_{j<i} (dp[j] + cost(j, i)*2 - lambda)
    // changes dp, cnt, memo, V 
    void solve(ll lambda)
    {
        // initialize dp, cnt, memo, V, (other data structures)
        for(int i=0; i<=N; i++) dp[i]=cnt[i]=memo[i]=0;
        V.clear();

        deque<int> DQ;
        DQ.push_back(0);

        for(int i=1; i<=N; i++)
        {
            while(DQ.size()>1 && dp[DQ[0]]+cost(DQ[0], i)*2 >= dp[DQ[1]]+cost(DQ[1], i)*2) DQ.pop_front();
            
            // opt = argmin(or max)_{j<i} (dp[j] + cost(j, i))
            int opt = DQ.front();
            dp[i]=dp[opt]+cost(opt, i)*2-lambda;
            cnt[i]=cnt[opt]+1;
            memo[i]=opt;

            while(DQ.size()>1 && !(cross(DQ[DQ.size()-2], DQ[DQ.size()-1]) < cross(DQ[DQ.size()-1], i))) DQ.pop_back();
            DQ.push_back(i);
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

    cin >> N >> K;
    K=min(N, K);
    for(int i=1; i<=N; i++) cin >> A[i], A[i]+=A[i-1];

    Alien_Monotone_Queue::init(N);
    auto [ans, ansV] = Alien_Monotone_Queue::alien(K);
    cout << ans << "\n";
}