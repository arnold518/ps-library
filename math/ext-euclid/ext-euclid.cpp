typedef __int128 dll;

namespace ExtEuclid
{
    // mfloor : O(1), mceil : O(1), mmod : O(1)
    // mfloor : computes floor(x / m)
    // mceil  : computes ceil(x / m)
    // mmod   : computes x mod abs(m), 0 <= result < abs(m)
    // REQUIRED : m != 0
    ll mfloor(ll x, ll m) { return x/m - ((x^m)<0 && x%m); }
    ll mceil(ll x, ll m) { return x/m + ((x^m)>0 && x%m); }
    ll mmod(dll x, ll m) { return x%m + (x<0 && x%m) * abs(m); }

    // get_inv : O(log N)
    // computes a^{-1} mod b, 0 <= result < b
    // REQUIRED : a, b > 0, gcd(a, b) = 1
    ll get_inv(ll a, ll b) { return (b==1) ? 0 : mmod((1-(dll)b*get_inv(b, a%b))/(a%b), b); }

    // crt : O(log N)
    // solves x = x1 (mod m1), x = x2 (mod m2)
    // if there is no solution, returns false
    // otherwise x = a mod b, returns true
    // REQUIRED : 0 <= x1 < m1 and 0 <= x2 < m2, numbers less than 2 * 10^12
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

    // diophantine : O(log N)
    // solves ax + by = c
    // if there is no solution, returns false
    // otherwise (x + sk, y - tk) describes all solutions
    // satisfies s >= 0, 0 <= x < s
    // REQUIRED : (a, b) != (0, 0), numbers less than 9 * 10^18
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
}