#include <iostream>
#include <algorithm>
#include <queue>
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
  int T;
  cin >> T;
  while (T--) {
    int g, s, l;
    cin >> g >> s >> l;
    Graph G(g + s);
    EdgeCapacityMap capacity = get(edge_capacity, G);
    ReverseEdgeMap rev_edge = get(edge_reverse, G);
    ResidualCapacityMap res_capacity = get(edge_residual_capacity, G);
    EdgeAdder ea(G, capacity, rev_edge);
    Vertex _source = add_vertex(G);
    Vertex _sink = add_vertex(G);
    while (l--) {
      int ground, satellite;
      cin >> ground >> satellite;
      ea.addEdge(ground, g + satellite, 1);
    }
    for (int i = 0; i < g; ++i) {
      ea.addEdge(_source, i, 1);
    }
    for (int i = g; i < g + s; ++i) {
      ea.addEdge(i, _sink, 1);
    }

    push_relabel_max_flow(G, _source, _sink);
    vector<int> vis(g+s+2, false);
    vis[_source] = true;
    std::queue<int> Q;
    Q.push(_source);
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
    // minimum vertex cover is formed by the non visited vertices of ground
    // stations (left side of bipartite graph) and visited vertices of
    // (right side of bipartite graph) satellites
    int g_prime = 0, s_prime = 0;
    vector<int> result;
    for (int i = 0; i < g; ++i) {
      if (!vis[i]) {
        ++g_prime;
        result.push_back(i);
      }
    }
    for (int i = g; i < g + s; ++i) {
      if (vis[i]) {
        ++s_prime;
        result.push_back(i - g);
      }
    }
    cout << g_prime << " " << s_prime << endl;
    bool first = true;
    for (auto it = result.begin(); it != result.end(); ++it) {
      if (!first)
        cout << " ";
      cout << *it;
      first = false;
    }
    cout << endl;
  }
  return 0;
}
