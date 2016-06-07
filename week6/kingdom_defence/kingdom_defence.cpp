// the tricky thing about this problem is to add a lower bound for the edges in
// the flow algorithm. For this purpose for each edge with a lower bound:
//  * Add an edge from source to the dest of the original edge (weight=lower
//    bound)
//  * Add an edge from source of original edge to sink (weight=lower bound)
//  * The original edge now has weight upper_bound - lower_bound
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
typedef graph_traits<Graph>::edge_iterator EdgeIt; // Iterator

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
    int l, p; // locations and paths
    cin >> l >> p;

    Graph G(l);
    EdgeCapacityMap capacity = get(edge_capacity, G);
    ReverseEdgeMap rev_edge = get(edge_reverse, G);
    EdgeAdder ea(G, capacity, rev_edge);

    Vertex _source = add_vertex(G);
    Vertex sink = add_vertex(G);

    int total_flow = 0;
    map<pair<int, int>, int> flows;
    for (int i = 0; i < l; ++i) {
      int g, d;
      cin >> g >> d;
      total_flow += d;
      auto epair = make_pair(_source, i);
      flows.insert(pair<pair<int, int>, int> (epair, g));
      epair = make_pair(i, sink);
      flows.insert(pair<pair<int, int>, int> (epair, d));
    }
    while (p--) {
      int f, t, min_, max_;
      cin >> f >> t >> min_ >> max_;
      total_flow += min_;
      ea.addEdge(f, t, max_ - min_);

      auto epair = make_pair(_source, t);
      flows[epair] += min_;
      epair = make_pair(f, sink);
      flows[epair] += min_;
    }
    for (auto it = flows.begin(); it != flows.end(); ++it)
      ea.addEdge(it->first.first, it->first.second, it->second);

    cout << (total_flow == push_relabel_max_flow(G, _source, sink) ? "yes" : "no")
        << endl;
  }
  return 0;
}
