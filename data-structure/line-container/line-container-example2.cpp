// https://judge.yosupo.jp/problem/line_add_get_min

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

namespace LineContainer
{
    const ll INF = 9e18;
    struct Line
    {
        mutable ll a, b, p;
        bool operator < (const Line& o) const { return a!=o.a ? o.a>a : b>o.b; } // min : < , max : > (both)
        bool operator < (ll x) const { return p < x; }
    };

    // LineContainer::LineContainer cht;
    // multiset of Line, sorted in increasing order of cross points
    struct LineContainer : multiset<Line, less<>>
    {
        // for doubles, use INF = 1/.0, div(a,b) = a/b
        ll div(ll u, ll d) { return u/d - ((u^d)<0 && u%d); }
        bool isect(iterator x, iterator y)
        {
            if (y == end()) { x->p = INF; return 0; }
            if (x->a == y->a) x->p = x->b > y->b ? INF : -INF; // min : < , max : >
            else x->p = div(y->b - x->b, x->a - y->a);
            return x->p >= y->p;
        }

        // push : O(log N), insert line p = ax+b
        void push(ll a, ll b) {
            auto z = insert({a, b, 0}), y = z++, x = y;
            while (isect(y, z)) z = erase(z);
            if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
            while ((y = x) != begin() && (--x)->p >= y->p) isect(x, erase(y));
        }
        
        // query : O(log N), get min (or max) value at x
        ll query(ll x) {
            assert(!empty());
            auto l = *lower_bound(x);
            return l.a * x + l.b;
        }
    };
}

int main()
{
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    int N, Q;
    cin >> N >> Q;
    LineContainer::LineContainer lc;

    for(int i=1; i<=N; i++)
    {
        ll a, b;
        cin >> a >> b;
        lc.push(-a, -b);
    }
    while(Q--)
    {
        int t;
        cin >> t;
        if(t==0)
        {
            ll a, b;
            cin >> a >> b;
            lc.push(-a, -b);
        }
        else
        {
            ll x;
            cin >> x;
            cout << -lc.query(x) << "\n";
        }
    }
}