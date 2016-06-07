#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <iostream>
#include <map>
#include <queue>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Face_handle Face_handle;

int main() {
  std::ios_base::sync_with_stdio(false);
  while (true) {
    int n, m;
    std::cin >> n;
    if (n == 0)
      break;

    std::vector<K::Point_2> infected;
    infected.reserve(n);
    while (n--) {
      int x, y;
      std::cin >> x >> y;
      infected.push_back(K::Point_2(x, y));
    }
    Triangulation t;
    t.insert(infected.begin(), infected.end());
    std::cin >> m;
    while (m--) {
      int x, y;
      double d;
      std::cin >> x >> y >> d;
      auto healthy = K::Point_2(x, y);
      auto nearest_infected = t.nearest_vertex(healthy)->point();
      // already inside the infection area
      if (CGAL::to_double(CGAL::squared_distance(nearest_infected, healthy)) < d) {
        std::cout << "n";
        continue;
      }
      auto face = t.locate(healthy);
      // already outside the infection area
      if (t.is_infinite(face->vertex(0)) || t.is_infinite(face->vertex(1))
          || t.is_infinite(face->vertex(2))) {
        std::cout << "y";
        continue;
      }
      // BFS on the faces of the triangulation starting at the one containing
      // the query point. Add neighbors iff the edge between it and the current
      // face has squared length >= 4d [(2*sqrt(d))^2]
      std::map<Face_handle, bool> visited;
      bool path_found = false;
      std::queue<Face_handle> q;
      q.push(face);
      while (!q.empty()) {
        auto f = q.front();
        q.pop();
        for (int i = 0; i < 3; ++i) {
          // see slide 19 of CGAL proximity structures
          int i_v1 = i % 3;
          int i_v2 = (i + 1) % 3;
          int i_neighbor = (i + 2) % 3;

          auto p1 = f->vertex(i_v1)->point();
          auto p2 = f->vertex(i_v2)->point();
          if (CGAL::to_double(CGAL::squared_distance(p1, p2)) >= 4*d) {
            auto neighbor = f->neighbor(i_neighbor);
            if (t.is_infinite(neighbor->vertex(0))
                || t.is_infinite(neighbor->vertex(1))
                || t.is_infinite(neighbor->vertex(2))) {
              path_found = true;
              break;
            }
            if (!visited[neighbor]) {
              visited[neighbor] = true;
              q.push(neighbor);
            }
          }
        }
        if (path_found)
          break;
      }
      if (path_found)
        std::cout << "y";
      else
        std::cout << "n";
    }
    std::cout << std::endl;
  }
  return 0;
}
