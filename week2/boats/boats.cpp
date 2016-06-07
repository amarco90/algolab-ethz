#include <iostream>
#include <limits>
#include <set>
#include <vector>

using namespace std;

int main() {
  std::ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while (T--) {
    int N;
    cin >> N;
    set< pair<int, int> > boats;
    while (N--) {
      int l, p;
      cin >> l >> p;
      boats.insert(make_pair(p, l));
    }
    int current_start = numeric_limits<int>::min();
    int best_deadline = numeric_limits<int>::max();
    int num_boats = 1;
    for (auto it = boats.begin(); it != boats.end(); ++it) {
      int p = it->first;
      int l = it->second;
      if (p >= best_deadline) {
        ++num_boats;
        current_start = best_deadline;
        best_deadline = numeric_limits<int>::max();
      }
      int possible_deadline = max(current_start, p - l) + l;
      best_deadline = min(best_deadline, possible_deadline);
    }
    cout << num_boats << endl;
  }
  return 0;
}
