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

double ceil_to_double(const CGAL::Quotient<ET>& x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

double floor_to_double(const CGAL::Quotient<ET>& x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  while (true) {
    int p, a, b;
    std::cin >> p;
    if (p == 0)
      break;
    std::cin >> a >> b;

    const int X = 0;
    const int Y = 1;
    const int Z = 2;
    Program final_qp;
    if (p == 1) {
      // CGAL::SMALLER makes the restrictions <=
      Program qp(CGAL::SMALLER, true, 0, false, 0); // x, y >= 0
      qp.set_a(X, 0, 1); qp.set_a(Y, 0, 1); qp.set_b(0, 4); // x+y <= 4
      qp.set_a(X, 1, 4); qp.set_a(Y, 1, 2); qp.set_b(1, a * b); // 4x+2y <= ab
      qp.set_a(X, 2, -1); qp.set_a(Y, 2, 1); qp.set_b(2, 1); // -x+y <= 1
      // max f(x) = - min -f(x)
      // -by + ax^2
      qp.set_c(Y, -b); qp.set_d(X, X, 2*a);
      final_qp = qp;
    } else if (p == 2) {
      // CGAL::LARGER makes the restrictions >=
      Program qp(CGAL::LARGER, false, 0, false, 0);
      qp.set_u(X, true, 0); qp.set_u(Y, true, 0); // x, y <= 0
      qp.set_a(X, 0, 1); qp.set_a(Y, 0, 1); qp.set_b(0, -4); // xy >= -4
      // 4x+2y+z^2 >= -ab
      qp.set_a(X, 1, 4); qp.set_a(Y, 1, 2); qp.set_a(Z, 1, 1);
          qp.set_b(1, -(a*b));
      qp.set_a(X, 2, -1); qp.set_a(Y, 2, 1); qp.set_b(2, -1); // -x*y >= -1
      // min ax^2 + by + z^4
      qp.set_d(X, X, 2*a); qp.set_c(Y, b); qp.set_d(Z, Z, 2);
      final_qp = qp;
    }
    Solution s = CGAL::solve_quadratic_program(final_qp, ET());
    if (s.is_optimal() && p == 1) {
      std::cout << (int) floor_to_double(-s.objective_value()) << std::endl;
    } else if (s.is_optimal() && p == 2) {
      std::cout << (int) ceil_to_double(s.objective_value()) << std::endl;
    } else if (s.is_unbounded()) {
      std::cout << "unbounded" << std::endl;
    } else {
      std::cout << "no" << std::endl;
    }
  }
  return 0;
}
