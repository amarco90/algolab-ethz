#include <iostream>
#include <algorithm>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/tuple/tuple.hpp>

using namespace boost;
using namespace std;

typedef adjacency_list<vecS, vecS, directedS, no_property, property<edge_weight_t, int> > Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef property_map<Graph, edge_weight_t>::type WeightMap;

// add edge from u to v and from v to u with cost c
void add_edges(Graph &G, WeightMap &wm, int u, int v, int c) {
  Edge e;
  tie(e, tuples::ignore) = add_edge(u, v, G);
  wm[e] = c;
  tie(e, tuples::ignore) = add_edge(v, u, G);
  wm[e] = c;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while (T--) {
    int n, m, k, x, y;
    cin >> n >> m >> k >> x >> y;
    int nvert = n * (k + 1);
    Graph G(nvert);
    WeightMap wm;
    while (m--) {
      int a, b, c, d;
      cin >> a >> b >> c >> d;
      if (d) { // bridge
        for (int i = 0; i < k; ++i) {
          Edge e;
          tie(e, tuples::ignore) = add_edge(a, b + n, G);
          wm[e] = c;
          tie(e, tuples::ignore) = add_edge(b, a + n, G);
          wm[e] = c;
          a += n;
          b += n;
        }
        add_edges(G, wm, a, b, c);
      } else {
        for (int i = 0; i <= k; ++i) { // for k+1 !!!
          add_edges(G, wm, a, b, c);
          a += n;
          b += n;
        }
      }
    }
    vector<int> dist(nvert);
    vector<int> pred(nvert);
    dijkstra_shortest_paths(G, x,
                            predecessor_map(make_iterator_property_map(pred.begin(),
                            get(vertex_index, G)))
                              .distance_map(make_iterator_property_map(dist.begin(),
                                get(vertex_index, G))
                              )
                           );
    cout << dist[y + k*n] << endl;
  }
  return 0;
}

