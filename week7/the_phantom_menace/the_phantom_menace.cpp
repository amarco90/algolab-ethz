#include <iostream>
#include <algorithm>
#include <map>
#include <queue>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

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
    int n, m, s, d;
    cin >> n >> m >> s >> d;
    // Define graph
    Graph G(2 * n);
    EdgeCapacityMap capacity = get(edge_capacity, G);
    ReverseEdgeMap rev_edge = get(edge_reverse, G);
    ResidualCapacityMap res_capacity = get(edge_residual_capacity, G);
    EdgeAdder ea(G, capacity, rev_edge);
    Vertex _source = add_vertex(G);
    Vertex _sink = add_vertex(G);
    for (int i = 0; i < n; ++i) // vertex capacity
      ea.addEdge(i, i + n, 1);
    for (int i = 0; i < m; ++i) {
      int u, v;
      cin >> u >> v;
      ea.addEdge(u + n, v, 1);
    }
    for (int i = 0; i < s; ++i) {
      int src;
      cin >> src;
      ea.addEdge(_source, src, 1);
    }
    for (int i = 0; i < d; ++i) {
      int tgt;
      cin >> tgt;
      ea.addEdge(tgt + n, _sink, 1);
    }
    int flow = push_relabel_max_flow(G, _source, _sink);
    cout << flow << endl;
  }
  return 0;
}
