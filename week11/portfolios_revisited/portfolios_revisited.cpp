#include <iostream>
#include <limits>
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

double find_result(Solution &s, std::vector<int> &returns) {
  double result = 0;
  int idx = 0;
  for (auto it = s.variable_values_begin(); it !=
       s.variable_values_end(); ++it) {
    result += returns[idx] * CGAL::to_double(*it);
    ++idx;
  }
  return result;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  while (true) {
    int n, m;
    std::cin >> n >> m;
    if (n == 0 && m == 0)
      break;

    std::vector<int> returns(n);
    const int ret_const = 0; // expected return constraint
    const int cos_const = 1; // cost constraint
    Program qp(CGAL::SMALLER, true, 0, false, 0); // all alphas >= 0
    for (int i = 0; i < n; ++i) {
      int c, r;
      std::cin >> c >> r;
      returns[i] = r;
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
      int C, V;
      std::cin >> C >> V;
      qp.set_b(cos_const, C);

      double best_result = std::numeric_limits<double>::min();
      int lower = 0;
      int upper = 40 * 1000000;
      while (lower + 1 != upper) {
        int middle = lower + (upper - lower) / 2;
        //std::cout << "[" << lower << ", " << upper << "]" << std::endl;
        //std::cout << " " << middle << std::endl;
        qp.set_b(ret_const, middle);

        Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
        if (s.is_optimal() && CGAL::to_double(s.objective_value()) <= V) {
          lower = middle;
          best_result = std::max(best_result, find_result(s, returns));
        } else {
          upper = middle;
        }
      }
      std::cout << best_result << std::endl;
    }
  }
  return 0;
}
