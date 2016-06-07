#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <iostream>
#include <limits>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

int main() {
  std::ios_base::sync_with_stdio(false);
  int T;
  std::cin >> T;
  while (T--) {
    int m, n;
    std::cin >> m >> n;

    std::vector<K::Point_2> participants;
    std::vector<int> participant_radius;
    std::vector<K::Point_2> lamps;
    std::vector<int> time_dead(m, std::numeric_limits<int>::max());
    for (int i = 0; i < m; ++i) {
      int x, y, r;
      std::cin >> x >> y >> r;
      participants.push_back(K::Point_2(x, y));
      participant_radius.push_back(r);
    }
    int h;
    std::cin >> h;
    for (int i = 0; i < n; ++i) {
      int x, y;
      std::cin >> x >> y;
      lamps.push_back(K::Point_2(x, y));
    }
    Triangulation t;
    t.insert(lamps.begin(), lamps.end());
    for (int i = 0; i < m; ++i) {
      // check if intersection area > 0
      K::Point_2 nearest_lamp = t.nearest_vertex(participants[i])->point();
      double closest_dist = CGAL::squared_distance(nearest_lamp,
                                                   participants[i]);
      double radius_intersect = h + participant_radius[i];
      double radius_intersect2 = radius_intersect * radius_intersect;
      if (radius_intersect2 <= closest_dist) // the closest is already too far
        continue;
      for (int j = 0; j < n; ++j) {
        double dist = CGAL::squared_distance(participants[i], lamps[j]);
        if (radius_intersect2 > dist) {
          time_dead[i] = j;
          break;
        }
      }
    }
    int winner_time = *std::max_element(time_dead.begin(), time_dead.end());
    for (int i = 0; i < m; ++i)
      if (time_dead[i] == winner_time) std::cout << i << " ";
    std::cout << std::endl;
  }
  return 0;
}
