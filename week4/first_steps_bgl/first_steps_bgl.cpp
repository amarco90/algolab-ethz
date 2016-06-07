#include <iostream>
#include <algorithm>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/tuple/tuple.hpp>

using namespace boost;
using namespace std;

typedef adjacency_list<vecS, vecS, undirectedS, no_property, property<edge_weight_t, int> > Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef property_map<Graph, edge_weight_t>::type WeightMap;

int main() {
  std::ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while (T--) {
    int n, m;
    cin >> n >> m;
    Graph G(n);
    WeightMap wm = get(edge_weight, G);
    while (m--) {
      int u, v, c;
      Edge e;
      cin >> u >> v >> c;
      tie(e, tuples::ignore) = add_edge(v, u, G);
      wm[e] = c;
    }
    // Sum of weight of MST
    int w = 0;
    vector<Edge> mst;
    kruskal_minimum_spanning_tree(G, back_inserter(mst));
    for (auto ite = mst.begin(); ite != mst.end(); ++ite)
      w += wm[*ite];
    // distance from node 0 to node furthest from it
    int d = -1;
    vector<int> dist(n);
    vector<int> pred(n);
    dijkstra_shortest_paths(G, 0, predecessor_map(make_iterator_property_map(pred.begin(),
                            get(vertex_index, G)))
                              .distance_map(make_iterator_property_map(dist.begin(),
                                get(vertex_index, G))
                              )
                           );
    graph_traits<Graph>::vertex_iterator vi, vend;
    for (tie(vi, vend) = vertices(G); vi != vend; ++vi)
      d = max(d, dist[*vi]);
    cout << w << " " << d << endl;
  }
  return 0;
}
