#include <iostream>
#include <algorithm>
#include <limits>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>

using namespace boost;
using namespace std;

typedef adjacency_list<vecS, vecS, directedS> Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::out_edge_iterator out_edge_iterator;

int which_component(vector< set<int> > &components, int elem) {
  for (int i = 0; i < components.size(); ++i) {
    if (components[i].find(elem) != components[i].end())
      return i;
  }
  return -1;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while (T--) {
    int n, m;
    cin >> n >> m;
    Graph G(n);
    vector<int> costs;
    for (int i = 0; i < m; ++i) {
      int u, v;
      cin >> u >> v;
      add_edge(u-1, v-1, G);
    }
    for (int i = 0; i < n; ++i) {
      int c;
      cin >> c;
      costs.push_back(c);
    }
    vector<int> scc(n);
    int nscc = strong_components(G,
        make_iterator_property_map(scc.begin(), get(vertex_index, G)));
    vector< set<int> > components(nscc);
    for (int i = 0; i < scc.size(); ++i) {
      components[scc[i]].insert(i);
    }
    vector<int> parent_scc(nscc, -1);
    for (int i = 0; i < nscc; ++i) {
      for (auto it = components[i].begin(); it != components[i].end(); ++it) {
        out_edge_iterator oei, oeiend;
        for (tie(oei, oeiend) = out_edges(*it, G); oei != oeiend; ++oei) {
          int t = target(*oei, G);
          if (components[i].find(t) == components[i].end())
            parent_scc[which_component(components, t)] = i;
        }
      }
    }
    // for the SCC with no parents (parent=-1) sum the cost
    int final_cost = 0;
    for (int i = 0; i < nscc; ++i) {
      if(parent_scc[i] == -1) {
        int cheapest = numeric_limits<int>::max();
        for (auto it = components[i].begin(); it != components[i].end(); ++it)
          cheapest = min(cheapest, costs[*it]);
        final_cost += cheapest;
      }
    }
    cout << final_cost << endl;
  }
  return 0;
}
