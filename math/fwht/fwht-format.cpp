namespace FWHT {

// dft : O(N log N), 0-based
// get DFT (IDFT if inv is true) of F
// length of A must be power of 2
void dft(vector<ll> &F, bool inv) {
  int N = F.size();
  for (int d = 1; d < N; d <<= 1) {
    for (int i = 0; i < N; i += d + d) {
      for (int j = 0; j < d; j++) {
        ll a = F[i | j], b = F[i | j | d];

        // XOR
        F[i | j] = a + b, F[i | j | d] = a - b;
        if (inv) F[i | j] /= 2, F[i | j | d] /= 2;

        // AND
        F[i | j] = b, F[i | j | d] = a + b;
        if (inv) F[i | j] = -a + b, F[i | j | d] = a;

        // OR
        F[i | j] = a, F[i | j | d] = a + b;
        if (inv) F[i | j] = a, F[i | j | d] = -a + b;
      }
    }
  }
}

};