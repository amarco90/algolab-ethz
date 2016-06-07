#include <iostream>
#include <iomanip>

using namespace std;

int main() {
  int T;
  cin >> T;
  while (T--) {
    int M;
    cin >> M;
    double sum = 0;
    double tmp;
    for (int m = 0; m < M; ++m) {
      cin >> tmp;
      sum += tmp;
    }
    cout << setprecision(7) << sum << endl;
  }
  return 0;
}
