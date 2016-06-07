#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <iostream>
#include <vector>

typedef CGAL::Gmpq ET;
typedef CGAL::Quadratic_program<double> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

int main() {
  std::ios_base::sync_with_stdio(false);
  int T;
  std::cin >> T;
  while (T--) {
    int l, s, w;
    std::cin >> l >> s >> w;

    Program lp(CGAL::SMALLER, true, 1, true, 4096); // 2^12 = 4096
    std::vector<K::Point_2> lamps;
    lamps.reserve(l);
    std::vector<K::Point_2> stamps;
    stamps.reserve(s);
    std::vector<K::Segment_2> walls;
    walls.reserve(w);
    for (int i = 0; i < l; ++i) {
      int x, y;
      std::cin >> x >> y;
      lamps.push_back(K::Point_2(x, y));
    }
    for (int j = 0; j < s; ++j) {
      int x, y, M;
      std::cin >> x >> y >> M;
      stamps.push_back(K::Point_2(x, y));
      lp.set_b(2 * j, -1); // \sum{ p_i * r } >= 1
      lp.set_b(2 * j + 1, M); // \sum{ p_i * r } <= M
    }
    for (int k = 0; k < w; ++k) {
      int a, b, c, d;
      std::cin >> a >> b >> c >> d;
      walls.push_back(K::Segment_2(K::Point_2(a, b), K::Point_2(c, d)));
    }
    for (int j = 0; j < s; ++j) {
      for (int i = 0; i < l; ++i) {
        auto tmp_seg = K::Segment_2(lamps[i], stamps[j]);
        bool no_wall_between = true;
        for (int k = 0; k < w; ++k) {
          if (CGAL::do_intersect(tmp_seg, walls[k])) {
            no_wall_between = false;
            break;
          }
        }
        if (no_wall_between) {
          double dist = CGAL::squared_distance(lamps[i], stamps[j]);
          lp.set_a(i, 2 * j, -1 / dist);
          lp.set_a(i, 2 * j + 1, 1 / dist);
        }
      }
    }
    Solution sol = CGAL::solve_linear_program(lp, ET());
    std::cout << (sol.is_infeasible() ? "no" : "yes") << std::endl;
  }
  return 0;
}
