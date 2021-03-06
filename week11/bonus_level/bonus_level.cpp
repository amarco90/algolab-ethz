#include <iostream>
#include <cstdlib>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

using namespace boost;
using namespace std;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
    property<edge_capacity_t, long,
        property<edge_residual_capacity_t, long,
            property<edge_reverse_t, Traits::edge_descriptor,
                property <edge_weight_t, long> > > > > Graph;

typedef property_map<Graph, edge_capacity_t>::type EdgeCapacityMap;
typedef property_map<Graph, edge_weight_t >::type EdgeWeightMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type ReverseEdgeMap;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::edge_iterator EdgeIt;
typedef graph_traits<Graph>::out_edge_iterator OutEdgeIt;

struct EdgeAdder {
  EdgeAdder(Graph & G, EdgeCapacityMap &capacity, EdgeWeightMap &weight, ReverseEdgeMap &rev_edge)
    : G(G), capacity(capacity), weight(weight), rev_edge(rev_edge) {}

  void addEdge(int u, int v, long c, long w) {
    Edge e, reverseE;
    tie(e, tuples::ignore) = add_edge(u, v, G);
    tie(reverseE, tuples::ignore) = add_edge(v, u, G);
    capacity[e] = c;
    weight[e] = w;
    capacity[reverseE] = 0;
    weight[reverseE] = -w;
    rev_edge[e] = reverseE;
    rev_edge[reverseE] = e;
  }
  Graph &G;
  EdgeCapacityMap &capacity;
  EdgeWeightMap &weight;
  ReverseEdgeMap  &rev_edge;
};

int main() {
  std::ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while (T--) {
    int n;
    cin >> n;

    const int EXTRA_COST = 100;
    Graph G(3 * n * n);
    EdgeCapacityMap capacity = get(edge_capacity, G);
    EdgeWeightMap weight = get(edge_weight, G);
    ReverseEdgeMap rev_edge = get(edge_reverse, G);
    ResCapacityMap res_capacity = get(edge_residual_capacity, G);
    EdgeAdder ea(G, capacity, weight, rev_edge);
    Vertex _source = add_vertex(G);
    Vertex _sink = add_vertex(G);

    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        int a;
        cin >> a;
        int current = 3 * j + 3 * n * i;
        ea.addEdge(current, current + 1, 1, EXTRA_COST - a);
        ea.addEdge(current, current + 2, 1, EXTRA_COST);
        if (j != n-1) {
          ea.addEdge(current + 1, current + 3, 1, 0);
          ea.addEdge(current + 2, current + 3, 1, 0);
        }
        if (i != n-1) {
          ea.addEdge(current + 1, current + 3 * n, 1, 0);
          ea.addEdge(current + 2, current + 3 * n, 1, 0);
        }
      }
    }
    ea.addEdge(_source, 0, 2, 0);
    ea.addEdge(n * n * 3 - 2, _sink, 1, 0);
    ea.addEdge(n * n * 3 - 1, _sink, 1, 0);

    successive_shortest_path_nonnegative_weights(G, _source, _sink);
    int cost = find_flow_cost(G);
    cout << -cost + 2 * (2 * n - 1) * EXTRA_COST << endl;
  }
}
