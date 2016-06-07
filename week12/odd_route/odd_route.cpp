#include <iostream>
#include <algorithm>
#include <limits>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/tuple/tuple.hpp>

using namespace boost;
using namespace std;

typedef adjacency_list<vecS, vecS, directedS, no_property, property<edge_weight_t, int> > Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef property_map<Graph, edge_weight_t>::type WeightMap;

int main() {
  std::ios_base::sync_with_stdio(false);

  int T;
  cin >> T;
  while (T--) {
    int n, m, s, t;
    cin >> n >> m >> s >> t;

    Graph H(4 * n);
    WeightMap wm = get(edge_weight, H);
    // Split all vertices u, 0 <= u < V(G) into 4 parts
    // 4*u (even length , even weight)
    // 4*u+1 (even length, odd weight)
    // 4*u+2 (odd length, even weight)
    // 4*u+3 (odd length, odd weight)
    while (m--) {
      int u, v, w;
      cin >> u >> v >> w;
      Edge e;
      tie(e, tuples::ignore) = add_edge(4*u,     4*v + 2 + w%2, H); wm[e] = w;
      tie(e, tuples::ignore) = add_edge(4*u + 1, 4*v + 3 - w%2, H); wm[e] = w;
      tie(e, tuples::ignore) = add_edge(4*u + 2, 4*v + w%2,     H); wm[e] = w;
      tie(e, tuples::ignore) = add_edge(4*u + 3, 4*v + 1 - w%2, H); wm[e] = w;
    }
    vector<int> dist(4 * n);
    vector<int> pred(4 * n);
    dijkstra_shortest_paths(H, 4*s,
                            predecessor_map(make_iterator_property_map(pred.begin(),
                            get(vertex_index, H)))
                              .distance_map(make_iterator_property_map(dist.begin(),
                                get(vertex_index, H))
                              )
                           );
    if (dist[4*t + 3] == numeric_limits<int>::max())
      cout << "no" << endl;
    else
      cout << dist[4*t + 3] << endl;
  }
  return 0;
}
