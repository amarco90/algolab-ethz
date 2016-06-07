#include <iostream>
#include <algorithm>
#include <queue>
#include <set>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
  property<edge_capacity_t, long,
    property<edge_residual_capacity_t, long,
      property<edge_reverse_t, Traits::edge_descriptor> > > > Graph;
typedef property_map<Graph, edge_capacity_t>::type EdgeCapacityMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type ReverseEdgeMap;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::edge_iterator EdgeIt;
typedef graph_traits<Graph>::out_edge_iterator OutEdgeIt;

struct EdgeAdder {
  EdgeAdder(Graph & G, EdgeCapacityMap &capacity, ReverseEdgeMap &rev_edge)
    : G(G), capacity(capacity), rev_edge(rev_edge) {}

  void addEdge(int u, int v, long c) {
    Edge e, reverseE;
    tie(e, tuples::ignore) = add_edge(u, v, G);
    tie(reverseE, tuples::ignore) = add_edge(v, u, G);
    capacity[e] = c;
    capacity[reverseE] = 0;
    rev_edge[e] = reverseE;
    rev_edge[reverseE] = e;
  }
  Graph &G;
  EdgeCapacityMap &capacity;
  ReverseEdgeMap &rev_edge;
};

int main() {
  std::ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while (T--) {
    int n, m;
    cin >> n >> m;
    Graph G(n);
    EdgeCapacityMap capacity = get(edge_capacity, G);
    ReverseEdgeMap rev_edge = get(edge_reverse, G);
    ResidualCapacityMap res_capacity = get(edge_residual_capacity, G);
    EdgeAdder ea(G, capacity, rev_edge);
    while (m--) {
      int a, b, c;
      cin >> a >> b >> c;
      ea.addEdge(a, b, c);
    }

    int best_source = -1;
    int best_sink = -1;
    int min_flow = numeric_limits<int>::max();
    set< pair<int, int> > explored_pairs;
    for (int i = 1; i < n; ++i) {
      int flow = push_relabel_max_flow(G, 0, i);
      if (flow < min_flow) {
        min_flow = flow;
        best_source = 0;
        best_sink = i;
      }
      flow = push_relabel_max_flow(G, i, 0);
      if (flow < min_flow) {
        min_flow = flow;
        best_source = i;
        best_sink = 0;
      }
    }

    push_relabel_max_flow(G, best_source, best_sink);
    vector<int> vis(n, false);
    vis[best_source] = true;
    std::queue<int> Q;
    Q.push(best_source);
    while (!Q.empty()) {
      const int u = Q.front();
      Q.pop();
      OutEdgeIt ebeg, eend;
      for (tie(ebeg, eend) = out_edges(u, G); ebeg != eend; ++ebeg) {
        const int v = target(*ebeg, G);
        if (res_capacity[*ebeg] == 0 || vis[v]) continue;
        vis[v] = true;
        Q.push(v);
      }
    }

    cout << min_flow << endl;
    cout << count(vis.begin(), vis.end(), true);
    for (int i = 0; i < n; ++i) {
      if (vis[i]) cout << " " << i;
    }
    cout << endl;
  }
  return 0;
}
