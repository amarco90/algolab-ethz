#include <iostream>
#include <algorithm>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
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
      int e1, e2;
      Edge e;
      cin >> e1 >> e2;
      tie(e, tuples::ignore) = add_edge(e1, e2, G);
      wm[e] = 1;
    }
    vector<Edge> mst;
    kruskal_minimum_spanning_tree(G, back_inserter(mst));
    int num_critical = 0;
    vector< pair<int, int> > critical_bridges;
    for (auto ite = mst.begin(); ite != mst.end(); ++ite) {
      vector<int> component(num_vertices(G));
      int src = source(*ite, G);
      int tgt = target(*ite, G);
      remove_edge(src, tgt, G);
      int num = connected_components(G, &component[0]);
      add_edge(src, tgt, G);
      if (num != 2) { // WARNING this should be 1! It's only 2 because of by default vertex
        critical_bridges.push_back(make_pair(min(src, tgt), max(src, tgt)));
        ++num_critical;
      }
    }
    cout << num_critical << endl;
    sort(critical_bridges.begin(), critical_bridges.end(),
         [](const std::pair<int,int> &left, const std::pair<int,int> &right) {
           return left.first != right.first ?
                    left.first < right.first :
                    left.second < right.second;
    });
    for (auto it = critical_bridges.begin(); it != critical_bridges.end(); ++it)
      cout << it->first << " " << it->second << endl;
  }
  return 0;
}
