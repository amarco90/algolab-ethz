#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bipartite.hpp>
#include <boost/graph/connected_components.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Face_handle Face_handle;
typedef Triangulation::Vertex_handle Vertex_handle;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;

int main() {
  std::ios_base::sync_with_stdio(false);
  int T;
  std::cin >> T;
  while (T--) {
    int n, m, r;
    std::cin >> n >> m >> r;
    double r2 = ((double) r) * ((double) r);
    std::vector<K::Point_2> stations;
    stations.reserve(n);
    std::map<K::Point_2, int> vertex_ids;
    for (int i = 0; i < n; ++i) {
      int x, y;
      std::cin >> x >> y;
      auto p = K::Point_2(x, y);
      stations.push_back(p);
      vertex_ids.insert(std::make_pair(p, i));
    }
    // Define triangulation
    Triangulation t;
    t.insert(stations.begin(), stations.end());
    // Define graph
    Graph G(n);
    for (auto vit = t.finite_vertices_begin(); vit != t.finite_vertices_end(); ++vit) {
      auto vc = t.incident_vertices(vit); // Vertex_circulator
      do {
        if (CGAL::squared_distance(vit->point(), vc->point()) <= r2) {
          int u = vertex_ids.find(vit->point())->second;
          int v = vertex_ids.find(vc->point())->second;
          if (u > v)
            add_edge(u, v, G);
        }
      } while (++vc != t.incident_vertices(vit));
    }
    bool interferences = !boost::is_bipartite(G);
    if (!interferences) { // maybe not all needed edges were included in the graph
      for (auto vit = t.finite_vertices_begin(); vit != t.finite_vertices_end();
          ++vit) {
        auto ec = t.incident_edges(vit); // Edge_circulator
        std::vector<K::Point_2> edge_points;
        do {
          if (t.is_infinite(ec)) continue;
          auto f = ec->first;
          auto p = f->vertex(f->ccw(ec->second))->point();
          for (auto ep: edge_points) {
            if (CGAL::squared_distance(p, ep) <= r2) {
              int u = vertex_ids.find(p)->second;
              int v = vertex_ids.find(ep)->second;
              add_edge(u, v, G);
            }
          }
          edge_points.push_back(p);
        } while (++ec != t.incident_edges(vit));
      }
      interferences = !boost::is_bipartite(G);
    }
    std::vector<int> component(boost::num_vertices(G));
    connected_components(G, &component[0]);
    for (int i = 0; i < m; ++i) {
      int ax, ay, bx, by;
      std::cin >> ax >> ay >> bx >> by;
      if (interferences) {
        std::cout << "n";
        continue;
      }
      auto holmes = K::Point_2(ax, ay);
      auto watson = K::Point_2(bx, by);
      if (CGAL::squared_distance(holmes, watson) <= r2) {
        std::cout << "y";
        continue;
      }
      auto nearest_h = t.nearest_vertex(holmes)->point();
      int comp_h = component[vertex_ids.find(nearest_h)->second];
      auto nearest_w = t.nearest_vertex(watson)->point();
      int comp_w = component[vertex_ids.find(nearest_w)->second];
      if (CGAL::squared_distance(holmes, nearest_h) <= r2
          && CGAL::squared_distance(watson, nearest_w) <= r2
          && comp_h == comp_w) {
        std::cout << "y";
        continue;
      }
      std::cout << "n";
    }
    std::cout << std::endl;
  }
  return 0;
}
