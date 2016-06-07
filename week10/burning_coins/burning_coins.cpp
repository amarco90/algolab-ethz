#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

#define S 1000

using namespace std;

vector<int> coins;
int stored[S][S];

int max_(int f, int l);

int min_(int f, int l) {
  if (f > l)
    return 0;

  int v1 = (stored[f + 1][l] == -1) ? max_(f + 1, l) : stored[f + 1][l];
  int v2 = (stored[f][l - 1] == -1) ? max_(f, l - 1) : stored[f][l - 1];
  stored[f][l] = min(v1, v2);
  return stored[f][l];
}

int max_(int f, int l) {
  if (f > l)
    return 0;

  int v1 = (stored[f + 1][l] == -1) ? min_(f + 1, l) : stored[f + 1][l];
  int v2 = (stored[f][l - 1] == -1) ? min_(f, l - 1) : stored[f][l - 1];
  stored[f][l] = max(coins[f] + v1, coins[l] + v2);
  return stored[f][l];
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while (T--) {
    int n;
    cin >> n;
    coins.clear();
    memset(stored, -1, S * S * sizeof(int));
    for (int i = 0; i < n; ++i) {
      int v;
      cin >> v;
      coins.push_back(v);
    }
    cout << max_(0, coins.size() - 1) << endl;
  }
  return 0;
}
