#include <iostream>
#include <algorithm>
#include <limits>
#include <queue>
#include <vector>

using namespace std;

vector<int> times;
vector< vector<int> > positions;

class CompareDist {
public:
  bool operator()(pair<int, int> n1, pair<int, int> n2) {
    return n1.first > n2.first;
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while (T--) {
    int n, m, p;
    times.clear();
    positions.clear();
    cin >> n;
    for (int i = 0; i < n; ++i) {
      cin >> m;
      times.push_back(m);
    }
    for (int i = 0; i < n; ++i) {
      vector<int> newv;
      positions.push_back(newv);
      for (int j = 0; j < times[i]; ++j) {
        cin >> p;
        positions[i].push_back(p);
      }
    }

    int a = positions[0][0], b = positions[0][0];
    // greater<int>
    priority_queue< pair<int, int>, vector< pair<int, int> >, CompareDist > current_snippets;
    vector<vector<int>::iterator> iterators;
    for (int i = 0; i < n; ++i) {
      sort(positions[i].begin(), positions[i].end());
      iterators.push_back(positions[i].begin());
      current_snippets.push(make_pair(*iterators[i], i));
      a = min(a, *iterators[i]);
      b = max(b, *iterators[i]);
      ++iterators[i];
    }
    int min_snippet = b - a + 1;
    int pos_first_last = -1; // position of first iterator arriving at last position
    while (!current_snippets.empty()) {
      auto top_snippet = current_snippets.top();
      current_snippets.pop();
      int pos = top_snippet.first;
      a = pos;
      if (pos_first_last != -1)
        a = min(a, pos_first_last);
      min_snippet = min(min_snippet, b - a + 1);
      //cout << "[" << a << ", " << b << "]" << endl;
      int owner = top_snippet.second;
      if (iterators[owner] != positions[owner].end()) {
        current_snippets.push(make_pair(*iterators[owner], owner));
        b = max(b, *iterators[owner]);
        ++iterators[owner];
      } else if (pos_first_last == -1) {
        pos_first_last = pos;
      }
    }
    cout << min_snippet << endl;
  }
  return 0;
}
