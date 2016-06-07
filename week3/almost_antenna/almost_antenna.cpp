#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <iostream>
#include <algorithm>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

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

    std::vector<K::Point_2> citizens;
    double x, y;
    while (n--) {
      std::cin >> x >> y;
      citizens.push_back(K::Point_2(x, y));
    }
    bool first = true;
    K::FT min_r;
    Min_circle mc_all(citizens.begin(), citizens.end(), true);
    for (auto it = mc_all.support_points_begin();
        it != mc_all.support_points_end(); ++it) {
      auto to_remove = std::find(citizens.begin(), citizens.end(), *it);
      citizens.erase(to_remove);
      Min_circle mc(citizens.begin(), citizens.end(), true);
      Traits::Circle c = mc.circle();
      auto r = sqrt(c.squared_radius());
      if (first) {
        min_r = r;
        first = false;
      } else if (r < min_r) {
        min_r = r;
      }
      citizens.push_back(*it);
    }
    std::cout << (long long) ceil_to_double(min_r) << std::endl;
  }
  return 0;
}
