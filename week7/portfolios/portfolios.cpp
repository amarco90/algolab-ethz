#include <iostream>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

#ifdef CGAL_USE_GMP
#include <CGAL/Gmpz.h>
typedef CGAL::Gmpz ET;
#else
#include <CGAL/MP_Float.h>
typedef CGAL::MP_Float ET;
#endif

typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int main() {
  std::ios_base::sync_with_stdio(false);

  while (true) {
    int n, m;
    std::cin >> n >> m;
    if (n == 0 && m == 0)
      break;

    const int ret_const = 0; // expected return constraint
    const int cos_const = 1; // cost constraint
    Program qp(CGAL::SMALLER, true, 0, false, 0); // all alphas >= 0
    for (int i = 0; i < n; ++i) {
      int c, r;
      std::cin >> c >> r;
      qp.set_a(i, ret_const, r); // component for return lower bound inequation
      qp.set_a(i, cos_const, c); // component for cost upper bound inequation
    }
    qp.set_r(ret_const, CGAL::LARGER);
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        int v;
        std::cin >> v;
        if (j <= i)
          qp.set_d(i, j, 2*v);
      }
    }
    for (int i = 0; i < m; ++i) {
      int C, R, V;
      std::cin >> C >> R >> V;
      qp.set_b(ret_const, R);
      qp.set_b(cos_const, C);

      Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
      std::cout << (s.is_optimal() && CGAL::to_double(s.objective_value()) <= V
          ? "Yes." : "No.") << std::endl;
    }
  }
  return 0;
}
