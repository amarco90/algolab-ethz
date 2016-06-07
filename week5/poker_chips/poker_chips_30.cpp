#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>

using namespace std;

const int SIZE = 1025;

vector<int> ms;
vector< vector<int> > stacks;

int dp[SIZE][SIZE];

int main() {
  std::ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while (T--) {
    memset(dp, 0, sizeof(short) * SIZE * SIZE);
    ms.clear();
    stacks.clear();
    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
      int m;
      cin >> m;
      ms.push_back(m);
      vector<int> newv;
      stacks.push_back(newv);
    }
    for (int i = 0; i < n; ++i) {
      stacks[i].push_back(-1); // insert unused element just for index convenience
      for (int j = 0; j < ms[i]; ++j) {
        int c;
        cin >> c;
        stacks[i].push_back(c);
      }
    }
    for (int i = 1; i <= ms[0]; ++i) {
      for (int j = 1; j <= ms[1]; ++j) {
        int match = stacks[0][i] == stacks[1][j];
        dp[i][j] = max(dp[i-1][j], max(dp[i][j-1], dp[i-1][j-1] + match));
      }
    }
    cout << dp[ms[0]][ms[1]] << endl;
  }
  return 0;
}
