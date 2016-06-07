#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <iostream>
#include <vector>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

typedef CGAL::Gmpq ET;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int main() {
  std::ios_base::sync_with_stdio(false);
  int T;
  std::cin >> T;
  while (T--) {
    int a, s, b, e;
    std::cin >> a >> s >> b >> e;
    Program lp(CGAL::LARGER, true, 0, false, 0); // all energies >= 0
    // asteroids
    std::vector<K::Point_2> asteroids(a);
    for (int i = 0; i < a; ++i) {
      int x, y, d;
      std::cin >> x >> y >> d;
      lp.set_b(i, d);
      asteroids[i] = K::Point_2(x, y);
    }
    // laser shots
    std::vector<K::Point_2> shots(s);
    for (int var = 0; var < s; ++var) {
      int x, y;
      std::cin >> x >> y;
      lp.set_c(var, 1);
      shots[var] = K::Point_2(x, y);
    }
    // bounty hunters
    std::vector<K::Point_2> hunters(b);
    for (int i = 0; i < b; ++i) {
      int x, y;
      std::cin >> x >> y;
      hunters[i] = K::Point_2(x, y);
    }
    Triangulation t;
    t.insert(hunters.begin(), hunters.end());
    for (int var = 0; var < s; ++var) {
      ET r2 = -1;
      if (b > 0) { // if there are hunters, find the closest one!
        auto nearest_hunter = t.nearest_vertex(shots[var])->point();
        r2 = CGAL::squared_distance(shots[var], nearest_hunter);
      }
      for (int i = 0; i < a; ++i) {
        ET dist = CGAL::squared_distance(shots[var], asteroids[i]);
        if (r2 == -1 || dist <= r2)
          lp.set_a(var, i, ET(1) / CGAL::max(ET(1), dist));
      }
    }
    Solution sol = CGAL::solve_nonnegative_linear_program(lp, ET());
    std::cout << (sol.is_optimal() && sol.objective_value() <= e ? "y" : "n")
        << std::endl;
  }
  return 0;
}
