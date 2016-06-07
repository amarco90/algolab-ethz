#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

struct state { int n, d; }; // node where we are, depth

char visited[110];

int main() {
  std::ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while (T--) {
    memset(visited, 0, 110);
    vector<int> empty;
    vector< vector<int> > graph(110, empty);
    vector<int>::iterator it;

    int n, m, s, t;
    cin >> n >> m >> s >> t;
    while (m--) {
      int ts, tt;
      cin >> ts >> tt;
      graph[ts].push_back(tt);
    }
    state is;
    is.n = s; // set source
    is.d = 0; // set length of path
    int times = 0; // t reached 0 times
    bool path_found = false;
    queue<state> frontier;
    frontier.push(is);
    while (!frontier.empty()) {
      state current = frontier.front();
      if (current.n == t) {
        ++times;
        if (times == 2) {
          path_found = true;
          cout << current.d << endl;
          break;
        }
      }
      ++current.d;
      frontier.pop();
      for (it = graph[current.n].begin(); it != graph[current.n].end(); ++it) {
        if (visited[*it] < 2) {
          ++visited[*it];
          state tmp;
          tmp.n = *it;
          tmp.d = current.d;
          frontier.push(tmp);
        }
      }
    }
    if (!path_found)
      cout << "no" << endl;
  }
  return 0;
}
