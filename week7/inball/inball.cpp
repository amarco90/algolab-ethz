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
    int n, d;
    std::cin >> n;
    if (n == 0)
      break;
    std::cin >> d;

    // Let c be the center of the ball and r the radius. The constraints can be
    // phrased as:
    // a^T (x + r*a / ||a||_2 ) <= b
    // The above, in words: a is normal to a^T (i.e. perpendicular) and
    // a / ||a||_2 is then the normal unit vector. In addition,
    // a^T a = (||a||_2)^2 . So the constraint finally is :
    // aT x + r *||a||2  <= b
    const int R = d;
    Program lp(CGAL::SMALLER, false, 0, false, 0);
    lp.set_l(R, true, 0); // the radious cannot be negative
    lp.set_c(R, -1); // min -R = max R
    for (int i = 0; i < n; ++i) {
      int sq_sum = 0; // to compute the norm
      for (int j = 0; j < d; ++j) {
        int a;
        std::cin >> a;
        lp.set_a(j, i, a);
        sq_sum += a*a;
      }
      lp.set_a(R, i, sqrt(sq_sum));

      int b;
      std::cin >> b;
      lp.set_b(i, b);
    }
    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_optimal()) {
      std::cout << (int) CGAL::to_double(-s.objective_value()) << std::endl;
    } else if (s.is_unbounded()) {
      std::cout << "inf" << std::endl;
    } else {
      std::cout << "none" << std::endl;
    }
  }
  return 0;
}
