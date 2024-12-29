namespace SuffixArray {

// 0-based, O(|S|log|S|)
// SA[i] : starting position of i-th suffix when sorted
// R[i] : lexicographic order of i-th suffix, R[SA[i]]=i
// LCP[i] : LCP(S[SA[i-1] ...], S[SA[i] ...])
// S   = "abcababc"
// SA  = [ 3, 5, 0, 4, 6, 1, 7, 2 ]
// R   = [ 2, 5, 7, 0, 3, 1, 4, 6 ]
// LCP = [ -, 2, 3, 0, 1, 2, 0, 1 ]
void suffix_array(string S, vector<int> &SA, vector<int> &R, vector<int> &LCP) {
  int N = S.size();
  vector<int> F(N), T(N);
  SA = R = LCP = vector<int>(N);

  iota(SA.begin(), SA.end(), 0);
  sort(SA.begin(), SA.end(), [&](int p, int q) { return S[p] < S[q]; });

  R[SA[0]] = 1;
  for (int i = 1; i < N; i++) R[SA[i]] = R[SA[i - 1]] + (S[SA[i]] != S[SA[i - 1]]);

  for (int d = 1; d <= N; d <<= 1) {
    if (R[SA[N - 1]] == N) break;

    F[0] = d;
    for (int i = d; i < N; i++) F[R[i]]++;
    for (int i = 1; i < N; i++) F[i] += F[i - 1];
    for (int i = 0; i < N; i++) SA[--F[(i + d < N ? R[i + d] : 0)]] = i;
    fill(F.begin(), F.end(), 0);

    T = SA;
    for (int i = 0; i < N; i++) F[R[i]]++;
    for (int i = 1; i < N; i++) F[i] += F[i - 1];
    for (int i = N - 1; i >= 0; i--) SA[--F[R[T[i]]]] = T[i];
    fill(F.begin(), F.end(), 0);

    T = R;
    R[SA[0]] = 1;
    for (int i = 1; i < N; i++) {
      R[SA[i]] = R[SA[i - 1]];
      if (T[SA[i]] != T[SA[i - 1]]) R[SA[i]]++;
      else {
        int p = (SA[i - 1] + d < N ? T[SA[i - 1] + d] : 0);
        int q = (SA[i] + d < N ? T[SA[i] + d] : 0);
        if (p != q) R[SA[i]]++;
      }
    }
  }
  for (int i = 0; i < N; i++) R[i]--;
  for (int i = 0, k = 0; i < N; i++)
    if (R[i]) {
      int j = SA[R[i] - 1];
      while (i + k < N && j + k < N && S[i + k] == S[j + k]) k++;
      LCP[R[i]] = k;
      k = max(k - 1, 0);
    }
}

// 0-based, init : O(|S|log|S|), get_lcp : O(1), compare : O(1)
// call init(S) first
// get_lcp(a, b) = LCP(S[a ...], S[b ...])
// compare(l1, r1, l2, r2) =
//  1 if S[l1 ... r1] > S[l2 ... r2]
//  0 if S[l1 ... r1] = S[l2 ... r2]
// -1 if S[l1 ... r1] < S[l2 ... r2]
// l1 <= r1, l2 <= r2 are NOT necessary
int N;
string S;
vector<int> SA, LCP, R;
vector<vector<int>> SP;
void init(string _S) {
  S = _S; N = S.size();
  suffix_array(S, SA, R, LCP);
  SP = vector<vector<int>>(__lg(N) + 1, vector<int>(N)); SP[0] = LCP;
  for (int i = 1; i < SP.size(); i++) for (int j = 0; j < N; j++) SP[i][j] = min(SP[i - 1][j], SP[i - 1][min(N - 1, j + (1 << (i - 1)))]);
}
int get_lcp(int a, int b) {
  if (a == b) return N - a;
  a = R[a]; b = R[b];
  if (a > b) swap(a, b);
  int len = b - a, k = __lg(len);
  return min(SP[k][a + 1], SP[k][b - (1 << k) + 1]);
}
int compare(int l1, int r1, int l2, int r2) {
  r1 = max(r1, l1 - 1);
  r2 = max(r2, l2 - 1);
  int len = min(r1 - l1 + 1, r2 - l2 + 1), x = get_lcp(l1, l2);
  if (x < len) return (S[l1 + x] > S[l2 + x]) - (S[l1 + x] < S[l2 + x]);
  else return (r1 - l1 > r2 - l2) - (r1 - l1 < r2 - l2);
}

}