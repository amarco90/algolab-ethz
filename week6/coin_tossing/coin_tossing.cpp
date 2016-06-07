#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
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
    vector<int> final_scores(n, 0);
    map<pair<int, int>, int> unknown_rounds;
    for (int mm = 0; mm < m; ++mm) {
      int a, b, c;
      cin >> a >> b >> c;
      switch (c) {
        case 0: {
          int tmp = max(a, b);
          a = min(a, b);
          b = tmp;
          auto p = make_pair(a, b);
          auto it = unknown_rounds.find(p);
          if (it != unknown_rounds.end()) {
            ++unknown_rounds[p];
          } else {
            unknown_rounds.insert(pair<pair<int, int>, int> (p, 1));
          }
          break;
        }
        case 1:
          --final_scores[a];
          break;
        case 2:
          --final_scores[b];
          break;
      }
    }
    bool impossible = false;
    int s_sum = 0;
    for (int i = 0; i < n; ++i) {
      int s;
      cin >> s;
      final_scores[i] += s;
      s_sum += s;
      if (final_scores[i] < 0)
        impossible = true;
    }
    if (impossible || s_sum != m) {
      cout << "no" << endl;
      continue;
    }
    Graph G(final_scores.size() + unknown_rounds.size());
    EdgeCapacityMap capacity = get(edge_capacity, G);
    ReverseEdgeMap rev_edge = get(edge_reverse, G);
    ResidualCapacityMap res_capacity = get(edge_residual_capacity, G);
    EdgeAdder ea(G, capacity, rev_edge);

    Vertex _source = add_vertex(G);
    Vertex sink = add_vertex(G);
    int i = 0;
    for (auto it = unknown_rounds.begin(); it != unknown_rounds.end(); ++it) {
      ea.addEdge(_source, i, it->second);
      ea.addEdge(i, it->first.first + unknown_rounds.size(), it->second);
      ea.addEdge(i, it->first.second + unknown_rounds.size(), it->second);
      ++i;
    }
    i = unknown_rounds.size();
    for (auto it = final_scores.begin(); it != final_scores.end(); ++it) {
      ea.addEdge(i, sink, *it);
      ++i;
    }
    push_relabel_max_flow(G, _source, sink);
    EdgeIt e, eend;
    for (tie(e, eend) = edges(G); e != eend; ++e) {
      if (sink == target(*e, G) && res_capacity[*e] != 0) {
        break;
      }
    }
    cout << ((e == eend) ? "yes" : "no") << endl;
  }
  return 0;
}
