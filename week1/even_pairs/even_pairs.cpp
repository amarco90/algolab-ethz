#include <iostream>

using namespace std;

int S[80000];

int main() {
  int n;
  int tmp;
  cin >> n;

  S[0] = 0;
  for (int i = 1; i <= n; ++i) {
    cin >> tmp;
    S[i] = S[i - 1] + tmp;
  }

  int even = 1;
  int odd = 0;
  int even_pairs = 0;
  for (int j = 1; j <= n; ++j) {
    if (S[j]%2 == 0) {
      even_pairs += even;
      ++even;
    } else {
      even_pairs += odd;
      ++odd;
    }
  }
  cout << even_pairs << endl;
  return 0;
}
