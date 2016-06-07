#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <iostream>
#include <map>

typedef unsigned int uint;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

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

    // rectangle
    int l, b, r, t;
    std::cin >> l >> b >> r >> t;
    auto top = K::Segment_2(K::Point_2(l, t), K::Point_2(r, t));
    auto right = K::Segment_2(K::Point_2(r, t), K::Point_2(r, b));
    auto bottom = K::Segment_2(K::Point_2(l, b), K::Point_2(r, b));
    auto left = K::Segment_2(K::Point_2(l, t), K::Point_2(l, b));
    // bacteria
    std::vector<K::Point_2> bacteria;
    bacteria.reserve(n);
    std::map<K::Point_2, double> min_time;
    while (n--) {
      int x, y;
      std::cin >> x >> y;
      auto p = K::Point_2(x, y);
      bacteria.push_back(p);
      auto dist = CGAL::min(squared_distance(top, p),
                    CGAL::min(squared_distance(right, p),
                      CGAL::min(squared_distance(bottom, p),
                        squared_distance(left, p))));
      double time_to_border = ceil_to_double(sqrt(sqrt(dist) - 0.5));
      min_time[p] = time_to_border;
    }
    Triangulation tri;
    tri.insert(bacteria.begin(), bacteria.end());
    for (auto e = tri.finite_edges_begin(); e != tri.finite_edges_end(); ++e) {
      auto dist = tri.segment(e).squared_length();
      double time = ceil_to_double(sqrt((sqrt(dist) - 1) / 2));
      auto p1 = tri.segment(e).point(0);
      auto p2 = tri.segment(e).point(1);
      min_time[p1] = std::min(min_time[p1], time);
      min_time[p2] = std::min(min_time[p2], time);
    }
    std::vector<double> times;
    times.reserve(bacteria.size());
    for (auto it = min_time.begin(); it != min_time.end(); ++it)
      times.push_back(it->second);
    std::sort(times.begin(), times.end());
    std::cout << (uint) times[0] << " "
        << (uint) times[times.size() / 2] << " "
        << times[times.size() - 1] << std::endl;
  }
  return 0;
}
