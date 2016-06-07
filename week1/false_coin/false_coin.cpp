#include <iostream>
#include <algorithm>
#include <cstring>

using namespace std;

char assignments[1005];
char used[1005];
int left_side[505];
int right_side[505];

void read_array(int *a, int Pi) {
  int tmp;
  for (int i = 0; i < Pi; ++i) {
    cin >> tmp;
    a[i] = tmp;
    used[tmp] = 1;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while (T--) {
    int N, K;
    cin >> N >> K;
    memset(assignments, '?', 1005);
    for (int k = 0; k < K; ++k) {
      memset(used, 0, 1005);
      int Pi;
      cin >> Pi;
      read_array(left_side, Pi);
      read_array(right_side, Pi);
      char weight_result;
      cin >> weight_result;

      // the lightest side always on the left
      if (weight_result == '>') {
        swap(left_side, right_side);
      }

      if (weight_result == '=') {
        for (int i = 0; i < Pi; ++i) {
          assignments[left_side[i]] = '=';
          assignments[right_side[i]] = '=';
        }
      } else {
        for (int i = 0; i < Pi; ++i) {
          // deal with '?'
          if (assignments[left_side[i]] == '?')
            assignments[left_side[i]] = '<';
          if (assignments[right_side[i]] == '?')
            assignments[right_side[i]] = '>';
          // if the coin changed side set to '='
          if (assignments[left_side[i]] == '>')
            assignments[left_side[i]] = '=';
          if (assignments[right_side[i]] == '<')
            assignments[right_side[i]] = '=';
        }
        // if the balance is uneven and the coin is not there
        for (int j = 1; j <= N; ++j) {
          if (!used[j])
            assignments[j] = '=';
        }
      }
    }
    int pos = -1;
    for (int i = 1; i <= N; ++i) {
      if (pos == -1 && assignments[i] != '=')
        pos = i;
      else if (assignments[i] != '=')
        pos = 0;
    }
    cout << pos << endl;
  }
  return 0;
}
