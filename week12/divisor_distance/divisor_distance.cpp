#include <iostream>
#include <algorithm>
#include <bitset>
#include <vector>

using namespace std;

#define HIGHER_BOUND 10000010
bitset<HIGHER_BOUND> is_prime;

// NOT USED to solve the problem!! just here for reference :)
bool is_prime(int n) {
  long long i = 2;
  // 1 is not prime
  if (n <= 1) return false;
  // If the number is even
  if      (n == 2) return true;
  else if (n%i == 0) return false;
  // If the number is odd
  ++i;
  long long root = sqrt(n);
  while (root >= i) {
    if (n%i == 0)
      return false;
    i += 2;
  }
  return true;
}

void prime_sieve() {
  is_prime.set(); // all true;
  is_prime.reset(0); // 0 not prime
  is_prime.reset(1); // 1 not prime
  for (int prime = 2; prime * prime < HIGHER_BOUND; ++prime) {
    if (is_prime[prime]) {
      // all multiples of the prime number are not prime
      for (int multiple = prime + prime; multiple < HIGHER_BOUND; multiple += prime)
        is_prime.reset(multiple);
    }
  }
}

// return next smallest prime that divides n
int next_smallest_prime(int n, int current) {
  if (is_prime[n])
    return 1;
  for (int prime = current; prime < n; ++prime)
    if (is_prime[prime] && n%prime == 0)
      return prime;
  return 1;
}

// returns a vector with all the divisors of v
vector<int> divisors(int v) {
  vector<int> dv;
  dv.push_back(v);
  if (v == 1)
    return dv;
  int prime = 2;
  while (!is_prime[v]) {
    prime = next_smallest_prime(v, prime);
    int mdiv = v / prime;
    dv.push_back(mdiv);
    v = mdiv;
  }
  dv.push_back(1);
  return dv;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  prime_sieve();
  int T;
  cin >> T;
  while (T--) {
    int n, c;
    cin >> n >> c;
    while (c--) {
      int v1, v2;
      cin >> v1 >> v2;
      vector<int> dv1 = divisors(v1);
      vector<int> dv2 = divisors(v2);
      int i = 0;
      int j = 0;
      while (dv1[i] != dv2[j]) {
        if (dv1[i] > dv2[j])
          ++i;
        else
          ++j;
      }
      cout << i + j << endl;
    }
  }
  return 0;
}

