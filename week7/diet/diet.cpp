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

double floor_to_double(const CGAL::Quotient<ET>& x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  while (true) {
    int n, m;
    std::cin >> n >> m;
    if (n == 0 && m == 0)
      break;

    Program lp(CGAL::SMALLER, true, 0, false, 0); // all foods >= 0
    // nutrients
    for (int i = 0; i < n; ++i) {
      int min_i, max_i;
      std::cin >> min_i >> max_i;
      // ax <= b --> -ax >= -b
      lp.set_b(2*i, -min_i); // lower bound for nutrient
      lp.set_b(2*i + 1, max_i); // upper bound for nutrient
    }
    // foods
    for (int i = 0; i < m; ++i) {
      int p;
      std::cin >> p;
      lp.set_c(i, p); // min equation price multiplier (min \sum_i p_i*f_i)
      for (int j = 0; j < n; ++j) {
        int c;
        std::cin >> c;
        lp.set_a(i, 2*j, -c); // factors for lower bound nutrient
        lp.set_a(i, 2*j + 1, c); // factors for upper bound nutrient
      }
    }
    Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
    if (s.is_optimal()) {
      std::cout << (int) floor_to_double(s.objective_value()) << std::endl;
    } else {
      std::cout << "No such diet." << std::endl;
    }
  }
  return 0;
}
