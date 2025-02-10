// https://www.acmicpc.net/problem/11069

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

typedef __int128 dll;

ll mfloor(ll x, ll m) { return x/m - ((x^m)<0 && x%m); }
ll mceil(ll x, ll m) { return x/m + ((x^m)>0 && x%m); }
ll mmod(dll x, ll m) { return x%m + (x<0 && x%m) * abs(m); }

ll get_inv(ll a, ll b) { return (b==1) ? 0 : mmod((1-(dll)b*get_inv(b, a%b))/(a%b), b); }

bool crt(ll x1, ll m1, ll x2, ll m2, ll &a, ll &b)
{
    ll g=__gcd(m1, m2);
    if(x1%g != x2%g) return false;
    ll r=x1%g;
    m1/=g; m2/=g; x1/=g; x2/=g;
    dll x=(dll)x1*m2*get_inv(m2, m1) + (dll)x2*m1*get_inv(m1, m2);
    ll m=m1*m2;
    a=g*mmod(x, m)+r;
    b=g*m;
    return true;
}

bool diophantine(ll a, ll b, ll c, ll &x, ll &y, ll &s, ll &t)
{
    if(!a && !b) assert(false);
    ll g=__gcd(abs(a), abs(b));
    if(c%g!=0) return false;
    if(!a) { x=0; y=c/b; s=1; t=0; return true; }
    if(!b) { x=c/a; y=0; s=0; t=1; return true; }
    a/=g; b/=g; c/=g;
    x=mmod((dll)get_inv(abs(a), abs(b))*(a<0 ? -c : c), abs(b));
    y=(c-(dll)x*a)/b;
    s=b; t=a;
    if(s<0) s=-s, t=-t;
    return true;
}

int main() {
    for(int i=-10; i<10; i++) cout << mmod(i, -4) << "\n";


	cin.tie(0); ios_base::sync_with_stdio(0);
	auto solve = [&](int W, int H, int x0, int y0, int x1, int y1, int a, int b) {
		int g = __gcd(abs(a), abs(b));
		a /= g;
		b /= g;
		ll ret = 1e18;
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				ll p, q, r, s, u, v;
				bool f = diophantine(a, -2 * W, (i == 0 ? -x1 : x1) - x0, p, u, q, v);
				bool g = diophantine(b, -2 * H, (j == 0 ? -y1 : y1) - y0, r, u, s, v);
				if (f && g) {
					bool h = crt(p, q, r, s, u, v);
					if (h) ret = min(ret, u);
				}
			}
		}
		return ret;
	};
	int TC;
	cin >> TC;
	while (TC--) {
		int W, H, x0, y0, x1, y1, x2, y2, a, b;
		cin >> W >> H >> x0 >> y0 >> x1 >> y1 >> x2 >> y2 >> a >> b;
		ll t1 = solve(W, H, x0, y0, x1, y1, a, b);
		ll t2 = solve(W, H, x0, y0, x2, y2, a, b);
		if (t1 == 1e18 && t2 == 1e18) {
			cout << "O\n";
		}
		else cout << (t1 < t2 ? "A\n" : "B\n");
	}
}