typedef vector<vector<mint>> mat;

namespace RREF {

// rref : O(NM min(N, M)), 0-based
// gets rref of matrix A
void rref(mat &A) {
  int N = A.size(), M = A[0].size();
  for (int y = 0, x = 0; x < M && y < N; x++) {
    int row = -1;
    for (int i = y; i < N; i++) if (A[i][x] != 0) row = i;
    if (row == -1) continue;

    for (int i = x; i < M; i++) swap(A[y][i], A[row][i]);
    mint t = inv(A[y][x]);
    for (int i = x; i < M; i++) A[y][i] *= t;
    for (int i = 0; i < N; i++) if (i != y) for (int j = M - 1; j >= x; j--) A[i][j] -= A[i][x] * A[y][j];
    y++;
  }
}

};