#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

int main() {
  std::ios_base::sync_with_stdio(false);
  int N;
  cin >> N;
  for (int i = 0; i < N; ++i) {
    long l, m, n;
    cin >> l >> m >> n;

    long min_space_left;
    long optimal_a, optimal_b;

    min_space_left = l;

    // find a*m + b*n <= l
    if (n*n > l) { // then b < sqrt(l)
      for (int b = ((int) sqrt(l)) - 1; b >= 0; --b) {
        if (b*n <= l) {
          long space_left = l - b*n;
          int a = space_left / m;
          space_left -= a*m;
          if (space_left < min_space_left) {
            min_space_left = space_left;
            optimal_a = a; optimal_b = b;
          }
        }
      }
    } else { // a < sqrt(l)
      for (int a = min(((long) sqrt(l))-1, n-1); a >= 0; --a) {
        if (a*m <= l) {
          long space_left = l - a*m;
          int b = space_left / n;
          space_left -= b*n;
          if (space_left < min_space_left || (space_left == min_space_left && b > optimal_b)) {
            min_space_left = space_left;
            optimal_a = a; optimal_b = b;
          }
        }
      }
    }
    cout << optimal_a << " " << optimal_b << " " << min_space_left << endl;
  }
  return 0;
}
