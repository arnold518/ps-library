// https://judge.yosupo.jp/problem/line_add_get_min

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

namespace CHT
{
    typedef __int128 dll;
    struct Line { ll a, b; };
    struct Frac
    {
        ll u, d;
        Frac(ll _u, ll _d) : u(_u), d(_d) { if(d<0) u=-u, d=-d; }
        bool operator < (const Frac &ot) const { return (dll)u*ot.d < (dll)ot.u*d; }
        bool operator > (const Frac &ot) const { return (dll)u*ot.d > (dll)ot.u*d; }
    };
    ll divfloor(ll u, ll d) { return u/d - ((u^d)<0 && u%d); }
    ll divceil(ll u, ll d) { return u/d + ((u^d)>0 && u%d); }

    // Get cross point of line p, q
    // If all queries are integer, change 'Frac' to 'div'
    Frac cross(const Line &p, const Line &q) { return Frac(p.b-q.b, q.a-p.a); } // dir + : divfloor, dir - : divceil

    // min/max :   ?, slope : ?, dir : ?
    // min/max : min, slope : +, dir : -
    // min/max : min, slope : -, dir : +
    // min/max : max, slope : +, dir : +
    // min/max : max, slope : -, dir : -
    struct CHT
    {
        deque<Line> V;

        // push : ammortized O(1)
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
            while(V.size()>1 && !(cross(V[V.size()-2], V[V.size()-1]) > cross(V[V.size()-1], p))) V.pop_back(); // dir + : <, dir - : >
            V.push_back(p);
        }

        // query : O(log N)
        // Get min (or max) value at x
        ll query(ll x)
        {
            assert(!V.empty());
            int lo=0, hi=V.size();
            while(lo+1<hi)
            {
                int mid=lo+hi>>1;
                if(cross(V[mid-1], V[mid]) > Frac(x, 1)) lo=mid; // dir + : <, dir - : >
                else hi=mid;
            }
            return V[lo].a*x+V[lo].b;
        }

        // query2 : ammortized O(1)
        // Get min (or max) value at x
        // x must be increasing (or decreasing) ('dir')
        ll query2(ll x)
        {
            assert(!V.empty());
            while(V.size()>1 && cross(V[0], V[1]) > Frac(x, 1)) V.pop_front(); // dir + : <, dir - : >
            return V[0].a*x+V[0].b;
        }
    };
}

const int MAXN = 2e5;

ll ans[MAXN+10];

int main()
{
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    vector<pll> A;
    int N, Q;
    cin >> N >> Q;
    for(int i=1; i<=N; i++)
    {
        ll a, b;
        cin >> a >> b;
        A.push_back({a, b});
    }

    // min/max : min, slope : +, dir : -

    sort(A.begin(), A.end());

    vector<CHT::CHT> cht(1);
    vector<pll> V;
    for(auto [a, b] : A) cht[0].push({a, b});

    int SQ = 500;
    for(int i=1; i<=Q; i++)
    {
        int t;
        cin >> t;
        if(t==0)
        {
            ll a, b;
            cin >> a >> b;
            V.push_back({a, b});
            if(V.size()>SQ)
            {
                sort(V.begin(), V.end());
                cht.push_back(CHT::CHT());
                for(auto [a, b] : V) cht.back().push({a, b});
                V.clear();
            }
        }
        else
        {
            ll x;
            cin >> x;
            ll ans=cht[0].query(x);
            for(auto &it : cht) ans=min(ans, it.query(x));
            for(auto [a, b] : V) ans=min(ans, a*x+b);
            cout << ans << "\n";
        }
    }
}