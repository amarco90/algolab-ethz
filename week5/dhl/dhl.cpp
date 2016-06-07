#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>

using namespace std;

vector<int> A;
vector<int> B;

int stored[1005][1005];

int rec_try(int i, int j) {
  int asum = 0;
  for (int a = 0; a < i; ++a)
    asum += A[a];
  int bsum = 0;
  for (int b = 0; b < j; ++b)
    bsum += B[b];

  if (i == 0)
    return A[i] * bsum;
  if (j == 0)
    return asum * B[j];
  int best = asum * bsum;

  asum = 0;
  for (int a = i - 1; a >= 0; --a) {
    asum += A[a];
    int cost = asum * B[j - 1];
    if (stored[a][j - 1] == -1)
      stored[a][j - 1] = rec_try(a, j - 1);
    best = min(best, cost + stored[a][j - 1]);
  }
  bsum = 0;
  for (int b = j - 1; b >= 0; --b) {
    bsum += B[b];
    int cost = A[i - 1] * bsum;
    if (stored[i - 1][b] == -1)
      stored[i - 1][b] = rec_try(i - 1, b);
    best = min(best, cost + stored[i - 1][b]);
  }
  return best;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while (T--) {
    memset(stored, -1, sizeof(int) * 1005 * 1005);
    A.clear();
    B.clear();
    int n;
    cin >> n;
    int tmp;
    for (int i = 0; i < n; ++i) {
      cin >> tmp;
      A.push_back(tmp - 1);
    }
    for (int i = 0; i < n; ++i) {
      cin >> tmp;
      B.push_back(tmp - 1);
    }
    cout << rec_try(n, n) << endl;
  }
  return 0;
}
