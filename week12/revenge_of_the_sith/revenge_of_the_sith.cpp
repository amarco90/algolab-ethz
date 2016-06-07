#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <iostream>
#include <map>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;

bool conquer(std::vector<K::Point_2> &planets,
    std::map<K::Point_2, int> &vertex_ids, int k, double r2) {
  Graph G;
  Triangulation t;
  t.insert(planets.begin() + k, planets.end());
  for (auto eit = t.finite_edges_begin(); eit != t.finite_edges_end(); ++eit) {
    auto f = eit->first;
    auto p1 = f->vertex(f->cw(eit->second))->point();
    auto p2 = f->vertex(f->ccw(eit->second))->point();
    if (CGAL::squared_distance(p1, p2) <= r2)
      add_edge(vertex_ids[p1], vertex_ids[p2], G);
  }
  if (boost::num_vertices(G) == 0) return false;
  std::vector<int> component(boost::num_vertices(G));
  int ncomp = connected_components(G, &component[0]);
  std::vector<int> n_elements(ncomp, 0);
  for (int i = 0; i < component.size(); ++i)
    ++n_elements[component[i]];
  return *max_element(n_elements.begin(), n_elements.end()) >= k;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int T;
  std::cin >> T;
  while (T--) {
    int n, r;
    std::cin >> n >> r;
    double r2 = ((double) r) * ((double) r);
    std::vector<K::Point_2> planets;
    planets.reserve(n);
    std::map<K::Point_2, int> vertex_ids;
    for (int i = 0; i < n; ++i) {
      int x, y;
      std::cin >> x >> y;
      auto p = K::Point_2(x, y);
      planets.push_back(p);
      vertex_ids.insert(std::make_pair(p, i));
    }
    int upper = n / 2;
    int lower = 1;
    int k = 1;
    while (lower <= upper) {
      int middle = (upper - lower) / 2 + lower;
      //std::cout << "["<< lower <<", "<<upper<<"] " << middle<< std::endl;
      if (conquer(planets, vertex_ids, middle, r2)) {
        k = middle;
        lower = middle + 1;
      } else {
        upper = middle - 1;
      }
    }
    std::cout << k << std::endl;
  }
  return 0;
}
