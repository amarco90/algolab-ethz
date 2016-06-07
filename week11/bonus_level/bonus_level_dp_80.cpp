#include <iostream>
#include <algorithm>
#include <map>
#include <vector>

using namespace std;

int n; // size of the board
int D; // number of diagonals in the board
int board[100][100];
map<tuple< int, tuple<int, int> >, int> stored;

int mario(int d, pair<int, int> pos1, pair<int, int> pos2) {
  //cout << d << " [" << pos1.first << ", " << pos1.second << "] [" << pos2.first << ", " <<pos2.second<<"]" << endl;
  if (d == D-1)
    return board[n - 1][n - 2] + board[n - 2][n - 1];

  int max_element = numeric_limits<int>::min();
  auto np1 = make_pair(pos1.first + 1, pos1.second);
  auto np2 = make_pair(pos2.first + 1, pos2.second);
  if (np1.first < n && np2.first < n && np1 < np2) {
    auto key = make_pair(d + 1, make_pair(np1.first, np2.first));
    auto it = stored.find(key);
    int res = it->second;
    if (it == stored.end())
      res = mario(d + 1, np1, np2);
    max_element = max(max_element, res);
  }
  np1 = make_pair(pos1.first + 1, pos1.second);
  np2 = make_pair(pos2.first, pos2.second + 1);
  if (np1.first < n && np2.second < n && np1 < np2) {
    auto key = make_pair(d + 1, make_pair(np1.first, np2.first));
    auto it = stored.find(key);
    int res = it->second;
    if (it == stored.end())
      res = mario(d + 1, np1, np2);
    max_element = max(max_element, res);
  }
  np1 = make_pair(pos1.first, pos1.second + 1);
  np2 = make_pair(pos2.first + 1, pos2.second);
  if (np1.second < n && np2.first < n && np1 < np2) {

    auto key = make_pair(d + 1, make_pair(np1.first, np2.first));
    auto it = stored.find(key);
    int res = it->second;
    if (it == stored.end())
      res = mario(d + 1, np1, np2);
    max_element = max(max_element, res);
  }
  np1 = make_pair(pos1.first, pos1.second + 1);
  np2 = make_pair(pos2.first, pos2.second + 1);
  if (np1.second < n && np2.second < n && np1 < np2) {
    auto np1 = make_pair(pos1.first, pos1.second + 1);
    auto np2 = make_pair(pos2.first, pos2.second + 1);
    auto key = make_pair(d + 1, make_pair(np1.first, np2.first));
    auto it = stored.find(key);
    int res = it->second;
    if (it == stored.end())
      res = mario(d + 1, np1, np2);
    max_element = max(max_element, res);
  }

  int vp1 = board[pos1.first][pos1.second];
  int vp2 = 0;
  if (pos1 != pos2)
    vp2 = board[pos2.first][pos2.second];

  auto key = make_pair(d, make_pair(pos1.first, pos2.first));
  auto value = max_element + vp1 + vp2;
  auto it = stored.find(key);
  if (it == stored.end())
    stored.insert(make_pair(key, value));
  return value;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while (T--) {
    cin >> n;
    stored.clear();
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        int a;
        cin >> a;
        board[i][j] = a;
      }
    }
    D = 2*n - 2;
    cout << mario(1, make_pair(0, 1), make_pair(1, 0)) + board[0][0] + board[n - 1][n - 1] << endl;
  }
  return 0;
}
