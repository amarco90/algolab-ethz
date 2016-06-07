#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <iostream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

int main() {
  std::ios_base::sync_with_stdio(false);
  while (true) {
    int n, m;
    std::cin >> n;
    if (n == 0)
      break;

    std::vector<K::Point_2> restaurants;
    restaurants.reserve(n);
    while (n--) {
      int x, y;
      std::cin >> x >> y;
      restaurants.push_back(K::Point_2(x, y));
    }
    Triangulation t;
    t.insert(restaurants.begin(), restaurants.end());
    std::cin >> m;
    while (m--) {
      int x, y;
      std::cin >> x >> y;
      auto proposed_loc = K::Point_2(x, y);
      auto nearest_rest = t.nearest_vertex(proposed_loc)->point();
      std::cout << (unsigned long long)
          CGAL::to_double(CGAL::squared_distance(nearest_rest, proposed_loc))
          << std::endl;
    }
  }
  return 0;
}
