#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct interval { int left, right; };

bool equals(const interval &i1, const interval &i2) {
  return i1.left == i2.left && i1.right == i2.right;
}

bool cmp(const interval &i1, const interval &i2) {
  return (i1.left != i2.left)
    ? i1.left < i2.left
    : i1.right > i2.right;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  vector<interval> aliens;
  int T;
  cin >> T;
  while (T--) {
    int n, m;
    cin >> n >> m;

    aliens.clear();
    vector<interval>::iterator it;

    while (n--) {
      interval in;
      cin >> in.left >> in.right;
      if (in.left != 0 || in.right != 0)
        aliens.push_back(in);
    }
    sort(aliens.begin(), aliens.end(), cmp);
    // check if for each human there exists an alien which wound him
    int end_point = 0;
    for (it = aliens.begin(); it != aliens.end(); ++it) {
      if (it->left > end_point +1)
        break;
      else
        end_point = max(end_point, it->right);
    }
    if (end_point < m) {
      cout << 0 << endl;
      continue;
    }
    // counting the superiors (intervals not contained in any other interval)
    int superior = 0;
    int rightmost = -1;
    for (unsigned int i = 1; i < aliens.size(); ++i) {
      if (equals(aliens[i - 1], aliens[i])) {
        if (aliens[i - 1].right > rightmost)
          rightmost = aliens[i - 1].right;
        while (i < aliens.size() && equals(aliens[i - 1], aliens[i]))
          ++i;
      } else if (aliens[i - 1].right > rightmost) {
        rightmost = aliens[i - 1].right;
        ++superior;
      }
    }
    if (aliens.size() == 1 || aliens[aliens.size() - 1].right > rightmost)
      ++superior;
    cout << superior << endl;
  }
  return 0;
}
