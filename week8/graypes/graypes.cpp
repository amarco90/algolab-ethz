#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <iostream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Edge_iterator Edge_iterator;

double ceil_to_double(const K::FT& x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  while (true) {
    int n;
    std::cin >> n;
    if (n == 0)
      break;

    std::vector<K::Point_2> graypes;
    graypes.reserve(n);
    while (n--) {
      int x, y;
      std::cin >> x >> y;
      graypes.push_back(K::Point_2(x, y));
    }
    Triangulation t;
    t.insert(graypes.begin(), graypes.end());
    bool first = true;
    K::FT min_dist;
    for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
      auto dist = t.segment(e).squared_length();
      if (dist < min_dist || first) {
        first = false;
        min_dist = dist;
      }
    }
    std::cout << (int) ceil_to_double(sqrt(min_dist) / 2 * 100) << std::endl;
  }
  return 0;
}
