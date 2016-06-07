#include <iostream>
#include <algorithm>

using namespace std;

int main() {
  std::ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while (T--) {
    int N;
    cin >> N;
    int falling_until = 0;
    for (int n = 0; n < N; ++n) {
      int h;
      cin >> h;
      if (n <= falling_until) {
        falling_until = max(falling_until, n + h - 1);
      }
    }
    cout << min(falling_until + 1, N) << endl;
  }
  return 0;
}
