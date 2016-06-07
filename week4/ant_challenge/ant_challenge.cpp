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
    vector<Graph> species;
    vector<WeightMap> wms;
    vector<int> hives;
    int t, e, s, a, b;
    cin >> t >> e >> s >> a >> b;
    for (int ss = 0; ss < s; ++ss) {
      Graph newg(t);
      species.push_back(newg);
      wms.push_back(get(edge_weight, newg));
    }
    while (e--) {
      int t1, t2;
      cin >> t1 >> t2;
      for (int ss = 0; ss < s; ++ss) {
        Edge e;
        int w;
        cin >> w;
        tie(e, tuples::ignore) = add_edge(t1, t2, species[ss]);
        wms[ss][e] = w;
      }
    }
    for (int ss = 0; ss < s; ++ss) {
      int h;
      cin >> h;
      hives.push_back(h);
    }
    Graph finalg(t);
    WeightMap finalwm;
    for (int ss = 0; ss < s; ++ss) {
      vector<Edge> mst;
      kruskal_minimum_spanning_tree(species[ss], back_inserter(mst));
      for (auto ite = mst.begin(); ite != mst.end(); ++ite) {
        Edge e;
        tie(e, tuples::ignore) = add_edge(source(*ite, species[ss]),
                                          target(*ite, species[ss]),
                                          finalg);
        finalwm[e] = wms[ss][*ite];
      }
    }
    vector<int> dist(t);
    vector<int> pred(t);
    dijkstra_shortest_paths(finalg, a,
                            predecessor_map(make_iterator_property_map(pred.begin(),
                            get(vertex_index, finalg)))
                              .distance_map(make_iterator_property_map(dist.begin(),
                                get(vertex_index, finalg))
                              )
                           );
    cout << dist[b] << endl;
  }
  return 0;
}
