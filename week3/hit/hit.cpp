#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iostream>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;

int main() {
  std::ios_base::sync_with_stdio(false);
  while (true) {
    int n;
    std::cin >> n;
    if (n == 0)
      break;

    std::vector<S> segments;
    double x, y, a, b;
    std::cin >> x >> y >> a >> b;
    P p = P(x, y);
    P q = P(a, b);
    auto phileas_ray = K::Ray_2(p, q);
    while (n--) {
      double r, s, t, u;
      std::cin >> r >> s >> t >> u;
      p = P(r, s);
      q = P(t, u);
      S seg = S(p, q);
      segments.push_back(seg);
    }
    bool intersect = false;
    for (auto it = segments.begin(); it != segments.end(); ++it) {
      if (CGAL::do_intersect(phileas_ray, *it)) {
        intersect = true;
        break;
      }
    }
    std::cout << (intersect ? "yes" : "no") << std::endl;
  }
  return 0;
}
