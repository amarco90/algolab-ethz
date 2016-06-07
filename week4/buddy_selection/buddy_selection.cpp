#include <iostream>
#include <algorithm>
#include <set>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
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
    int n, f, c;
    cin >> n >> c >> f;
    Graph G(n);
    vector< set<string> > characteristics;
    WeightMap wm = get(edge_weight, G);
    for (int u = 0; u < n; ++u) {
      string characteristic;
      set<string> newset;
      characteristics.push_back(newset);
      for (int i = 0; i < c; ++i) {
        cin >> characteristic;
        characteristics[u].insert(characteristic);
      }
      for (int v = 0; v < (int) characteristics.size(); ++v) {
        if (u == v) continue;

        set<string> intersect;
        set_intersection(characteristics[u].begin(), characteristics[u].end(),
                         characteristics[v].begin(), characteristics[v].end(),
                         inserter(intersect, intersect.begin()));
        if ((int) intersect.size() > f) {
          Edge e;
          tie(e, tuples::ignore) = add_edge(u, v, G);
          wm[e] = (int) intersect.size();
        }
      }
    }
    vector<graph_traits<Graph>::vertex_descriptor> mate(n);
    edmonds_maximum_cardinality_matching(G, &mate[0]);
    graph_traits<Graph>::vertex_iterator vi, vi_end;
    int connections = 0;
    for(tie(vi, vi_end) = vertices(G); vi != vi_end; ++vi)
      if (mate[*vi] != graph_traits<Graph>::null_vertex() && *vi < mate[*vi])
        ++connections;

    cout << ((connections == n/2) ? "not optimal" : "optimal") << endl;
  }
  return 0;
}
