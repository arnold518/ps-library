// you should NOT put 'int' as left operand
// mint should be initialized with an integer in [0, MOD - 1].
// ex. mint x = (long long)1e18; << NOT GOOD
const int MOD = (int)1e9 + 7;
struct mint {
  int x;
  mint(void) : x(0) {}
  mint(int _x) : x(_x) {}
  mint operator+(int ot) const { return x + ot >= MOD ? x + ot - MOD : x + ot; }
  mint operator-(void) const { return x ? MOD - x : 0; }
  mint operator-(int ot) const { return x < ot ? x + MOD - ot : x - ot; }
  mint operator*(int ot) const { return 1ll * x * ot % MOD; }
  mint operator+=(int ot) { return *this = *this + ot; }
  mint operator-=(int ot) { return *this = *this - ot; }
  mint operator*=(int ot) { return *this = *this * ot; }
  operator int(void) const { return x; }
};
mint mpow(mint a, ll b) {
  mint ret = 1;
  while (b) {
    if (b & 1) ret = ret * a;
    a = a * a; b >>= 1;
  }
  return ret;
}
mint inv(mint x) { return mpow(x, MOD - 2); }