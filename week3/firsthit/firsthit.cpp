#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;

double floor_to_double(const K::FT& x) {
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

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
    p_src = P(x, y);
    q = P(a, b);
    auto phileas_ray = K::Ray_2(p_src, q);
    while (n--) {
      double r, s, t, u;
      std::cin >> r >> s >> t >> u;
      S seg = S(P(r, s), P(t, u));
      segments.push_back(seg);
    }
    bool one_found = false;
    P min_dist_p;
    K::FT min_dist;
    for (auto it = segments.begin(); it != segments.end(); ++it) {
      if (CGAL::do_intersect(phileas_ray, *it)) {
        auto o = CGAL::intersection(phileas_ray, *it);
        K::FT dist;
        P ipoint;
        if (P* op = boost::get<P>(&*o)) {
          dist = CGAL::squared_distance(p_src, *op);
          ipoint = *op;
        } else if (S* os = boost::get<S>(&*o)) {
          if (CGAL::squared_distance(p_src, os->source())
              < CGAL::squared_distance(p_src, os->target())) {
            dist = CGAL::squared_distance(p_src, os->source());
            ipoint = os->source();
          } else {
            dist = CGAL::squared_distance(p_src, os->target());
            ipoint = os->target();
          }
        }
        if (!one_found) {
          min_dist = dist;
          min_dist_p = ipoint;
          one_found = true;
        } else if (dist < min_dist) {
          min_dist = dist;
          min_dist_p = ipoint;
        }
      }
    }
    if (one_found)
      std::cout << (long long) floor_to_double(min_dist_p.x()) << " "
                << (long long) floor_to_double(min_dist_p.y()) << std::endl;
    else
      std::cout << "no" << std::endl;
  }
  return 0;
}
