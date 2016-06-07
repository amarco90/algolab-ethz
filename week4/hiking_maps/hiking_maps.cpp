#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iostream>
#include <limits>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

K::Point_2 q0, q1, q2, q3, q4, q5;

bool inside_map(K::Point_2 &p) {
  return (CGAL::left_turn(q0, q1, p) || CGAL::collinear(q0, q1, p))
      && (CGAL::left_turn(q2, q3, p) || CGAL::collinear(q2, q3, p))
      && (CGAL::left_turn(q4, q5, p) || CGAL::collinear(q4, q5, p));
}

// WARNING!! modifies the first vector
void vsum(std::vector<int> &a, std::vector<int> &b) {
  for (int i = 0; i < a.size(); ++i)
    a[i] += b[i];
}

void vsub(std::vector<int> &a, std::vector<int> &b) {
  for (int i = 0; i < a.size(); ++i)
    a[i] -= b[i];
}

bool all_contained(std::vector<int> &contained) {
  return std::all_of(contained.begin(), contained.end(), [](int i){return i>0;});
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int T;
  std::cin >> T;
  while (T--) {
    int m, n;
    std::cin >> m >> n;
    std::vector<K::Point_2> legs;
    legs.reserve(m);
    for (int i = 0; i < m; ++i) {
      int x, y;
      std::cin >> x >> y;
      legs.push_back(K::Point_2(x, y));
    }
    std::vector< std::vector<int> > cont_leg;
    for (int i = 0; i < n; ++i) {
      int x0, y0, x1, y1, x2, y2, x3, y3, x4, y4, x5, y5;
      std::cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4
          >> x5 >> y5;
      q0 = K::Point_2(x0, y0);
      q1 = K::Point_2(x1, y1);
      q2 = K::Point_2(x2, y2);
      q3 = K::Point_2(x3, y3);
      q4 = K::Point_2(x4, y4);
      q5 = K::Point_2(x5, y5);

      // orient the points to test with left_turn after
      if (CGAL::right_turn(q0, q1, q2))
        std::swap(q0, q1);
      if (CGAL::right_turn(q2, q3, q4))
        std::swap(q2, q3);
      if (CGAL::right_turn(q4, q5, q0))
        std::swap(q4, q5);

      std::vector<int> contained(m - 1, 0);
      for (int li = 0; li < legs.size() - 1; ++li) {
        if (inside_map(legs[li]) && inside_map(legs[li + 1]))
          contained[li] = 1;
      }
      cont_leg.push_back(contained);
    }
    int i = 0;
    std::vector<int> contained(m - 1, 0);
    int min_size = std::numeric_limits<int>::max();
    for (int j = i; j < n; ++j) {
      vsum(contained, cont_leg[j]);
      while (all_contained(contained)) {
        min_size = std::min(min_size, j - i + 1);
        vsub(contained, cont_leg[i]);
        ++i;
      }
    }
    std::cout << min_size << std::endl;
  }
  return 0;
}
