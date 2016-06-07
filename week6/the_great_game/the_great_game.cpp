#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

#define S 50001

using namespace std;

int minstored[S];
int maxstored[S];

int max_(vector< vector<int> > &graph, int src, int tgt);

int min_(vector< vector<int> > &graph, int src, int tgt) {
  if (src == tgt) {
    minstored[src] = 0;
    return minstored[src];
  }

  vector<int> dist;
  for (int v: graph[src]) {
    if (maxstored[v] == -1)
      max_(graph, v, tgt);
    dist.push_back(maxstored[v]);
  }
  minstored[src] = *min_element(begin(dist), end(dist)) + 1;
  return minstored[src];
}

int max_(vector< vector<int> > &graph, int src, int tgt) {
  if (src == tgt) {
    maxstored[src] = 0;
    return maxstored[src];
  }

  vector<int> dist;
  for (int v: graph[src]) {
    if (minstored[v] == -1)
      min_(graph, v, tgt);
    dist.push_back(minstored[v]);
  }
  maxstored[src] = *max_element(begin(dist), end(dist)) + 1;
  return maxstored[src];
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while (T--) {
    int n, m, r, b;
    cin >> n >> m >> r >> b;
    vector< vector<int> > graph(n + 1);
    for (int i = 0; i < m; ++i) {
      int u, v;
      cin >> u >> v;
      graph[u].push_back(v);
    }
    memset(minstored, -1, S * sizeof(int));
    memset(maxstored, -1, S * sizeof(int));
    int red_moves = min_(graph, r, n);
    int black_moves = min_(graph, b, n);
    cout << (black_moves < red_moves
        || (black_moves == red_moves && black_moves%2 == 0)) << endl;
  }
  return 0;
}
