namespace FFT {

// double (double_range : 2^52) <=> long double (double_range : 2^63)
// ll (long_range : 2^63) <=> dll (long_range : 2^128)

typedef complex<double> cpx;
const double PI = acos((double)-1);

int M, B;
vector<cpx> W;
vector<int> rev;

// call init(B) first
// N <= 2^B must hold for all N given in dft
void init(int _B) {
  B = _B; M = 1 << B;
  W = vector<cpx>(M);
  rev = vector<int>(M);
  for (int i = 0; i < M; i++) W[i] = cpx(cos(PI / M * i), sin(PI / M * i));
  for (int i = 0; i < M; i++) rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (B - 1));
}

// dft : O(N log N), 0-based
// get DFT (IDFT if inv is true) of polynomial F
// length of A must be power of 2
void dft(vector<cpx> &F, bool inv) {
  int N = F.size(), C = __lg(N);
  for (int i = 0; i < N; i++) {
    int p = rev[i << (B - C)];
    if (i < p) swap(F[i], F[p]);
  }
  for (int d = 1; d < N; d <<= 1) {
    for (int i = 0; i < N; i += d + d) {
      for (int j = 0; j < d; j++) {
        cpx r = W[M / d * j];
        if (inv) r = conj(r);
        cpx b = F[i | j | d] * r;
        F[i | j | d] = F[i | j] - b;
        F[i | j] += b;
      }
    }
  }
  if (inv) for (int i = 0; i < N; i++) F[i] /= N;
}

// multiply : O(N log N), 0-based
// multiplies polynomial F, G (MAXV is small)
// MAXV * MAXV * N <= long_range
// MAXV * MAXV * N <= double_range
// stronger condition might need to hold, due to precision
vector<dll> multiply(vector<int> F, vector<int> G) {
  int N = 1;
  vector<dll> ret(F.size() + G.size() - 1);
  while (N < ret.size()) N <<= 1;
  vector<cpx> CF(N), CG(N);
  for (int i = 0; i < F.size(); i++) CF[i] = cpx(F[i], 0);
  for (int i = 0; i < G.size(); i++) CG[i] = cpx(G[i], 0);
  dft(CF, false);
  dft(CG, false);
  for (int i = 0; i < N; i++) CF[i] *= CG[i];
  dft(CF, true);
  for (int i = 0; i < ret.size(); i++) ret[i] = (dll)round(CF[i].real());
  return ret;
}

const int D = 32000;  // sqrt(MAXV)
// multiply2 : O(N log N), 0-based
// multiplies polynomial F, G (MAXV is big)
// MAXV * MAXV * N <= long_range
// D * D * N = MAXV * N <= double_range
// stronger condition might need to hold, due to precision
vector<dll> multiply2(vector<int> F, vector<int> G) {
  int N = 1;
  vector<dll> ret(F.size() + G.size() - 1);
  while (N < ret.size()) N <<= 1;
  vector<cpx> P(N), Q(N), R(N), S(N);
  for (int i = 0; i < F.size(); i++) P[i] = cpx(F[i] / D, F[i] % D);
  for (int i = 0; i < G.size(); i++) Q[i] = cpx(G[i] / D, G[i] % D);
  dft(P, false);
  dft(Q, false);
  for (int i = 0; i < N; i++) {
    int j = (N - i) % N;
    R[i] = (P[i] + conj(P[j])) * Q[i] * cpx(0.5, 0);
    S[i] = (P[i] - conj(P[j])) * conj(Q[j]) * cpx(0.5, 0);
  }
  dft(R, true);
  dft(S, true);
  for (int i = 0; i < ret.size(); i++) ret[i] = (dll)round(R[i].real()) * D * D + (dll)round(R[i].imag() + S[i].imag()) * D + (dll)round(S[i].real());
  return ret;
}

};