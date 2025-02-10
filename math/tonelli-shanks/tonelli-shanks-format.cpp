namespace TonelliShanks {

mt19937 rnd(1557);
ll rng(ll l, ll r) { return uniform_int_distribution<ll>(l, r)(rnd); }
ll mpow(ll a, ll b, ll mod) {
  ll ret = 1;
  for (; b; b >>= 1, a = a * a % mod) if (b & 1) ret = ret * a % mod;
  return ret;
}

// quadratic_residue : O(log^2 P)
// returns one x such that x^2 = n (mod p)
// returns -1 if such x does not exist
// REQUIRED : p is prime, 0 <= n < p
ll quadratic_residue(ll n, ll p) {
  if (n == 0) return 0;
  if (p == 2) return n;
  if (mpow(n, (p - 1) / 2, p) != 1) return -1;

  int q = p - 1, s = __builtin_ctz(q);
  q >>= s;

  ll x = mpow(n, (q + 1) / 2, p);
  ll t = mpow(n, q, p);
  ll z = rng(1, p - 1);
  while (mpow(z, (p - 1) / 2, p) != p - 1) z = rng(1, p - 1);

  while (t != 1) {
    ll i = 0, v = t;
    while (v != 1) v = v * v % p, i++;
    ll b = mpow(z, q * (1ll << (s - i - 1)), p);
    x = x * b % p;
    t = t * b % p * b % p;
  }
  return x;
}

}