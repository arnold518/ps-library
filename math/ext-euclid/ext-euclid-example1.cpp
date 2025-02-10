// https://www.acmicpc.net/problem/1571

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

const int MAXN = 50;

int N;
string S[MAXN+10], T;

int main()
{
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    cin >> N;
    for(int i=0; i<N; i++) cin >> S[i];
    cin >> T;

    ll x=-1, m=S[0].size();
    for(int i=0; i<S[0].size(); i++) if(S[0][i]==T[0]) x=i;
    
    if(x==-1) return !(cout << "-1\n");

    for(int i=1; i<N; i++)
    {
        ll x2=-1, m2=S[i].size();
        for(int j=0; j<S[i].size(); j++) if(S[i][j]==T[i]) x2=j;
        if(x2==-1) return !(cout << "-1\n");
        ll a, b;
        if(!crt(x, m, x2, m2, a, b)) return !(cout << "-1\n");
        x=a; m=b;
    }
    cout << x << "\n";
}