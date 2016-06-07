#include <iostream>
#include <limits>
#include <cstdlib>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
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

struct Request {
  int s, t; // source, target
  int d, a; // departure, arrival
  int p;    // profit
  Request(int ps, int pt, int pd, int pa, int pp)
      : s(ps), t(pt), d(pd), a(pa), p(pp) {}
};

int main() {
  std::ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while (T--) {
    int N, S;
    cin >> N >> S;
    vector<int> sources(S);
    for (int i = 0; i < S; ++i)
      cin >> sources[i];
    vector<Request> requests;
    requests.reserve(N);
    set<int> times;
    for (int i = 0; i < N; ++i) {
      int s, t, d, a, p;
      cin >> s >> t >> d >> a >> p;
      --s; --t;
      requests.push_back(Request(s, t, d, a, p));
      times.insert(d);
      times.insert(a);
    }
    int n_slots = times.size();
    vector<int> timesv(times.begin(), times.end());

    const int MAX_PROFIT = 100;
    Graph G(S * n_slots);
    EdgeCapacityMap capacity = get(edge_capacity, G);
    EdgeWeightMap weight = get(edge_weight, G);
    ReverseEdgeMap rev_edge = get(edge_reverse, G);
    ResCapacityMap res_capacity = get(edge_residual_capacity, G);
    EdgeAdder ea(G, capacity, weight, rev_edge);
    Vertex _source = add_vertex(G);
    Vertex _sink = add_vertex(G);
    // source -> first time
    for (int i = 0; i < S; ++i)
      ea.addEdge(_source, i * n_slots, sources[i], 0);
    // connection between times
    for (int i = 0; i < S; ++i) {
      int offset = i * n_slots;
      ea.addEdge(offset + n_slots - 1, _sink, numeric_limits<int>::max(), MAX_PROFIT);
      for (int j = 0; j < n_slots - 1; ++j) {
        ea.addEdge(offset + j, offset + j + 1, numeric_limits<int>::max(), MAX_PROFIT);
      }
    }
    // request connections
    for (auto req: requests) {
      int pos1 = distance(timesv.begin(), equal_range(timesv.begin(),
                          timesv.end(), req.d).first);
      int u = req.s * n_slots + pos1;
      int pos2 = distance(timesv.begin(), equal_range(timesv.begin(),
                          timesv.end(), req.a).first);
      int v = req.t * n_slots + pos2;
      int vert_between = pos2 - pos1;
      ea.addEdge(u, v, 1, vert_between * MAX_PROFIT - req.p);
    }
    //int flow = push_relabel_max_flow(G, _source, _sink);
    //cycle_canceling(G);
    successive_shortest_path_nonnegative_weights(G, _source, _sink);
    int flow = 0;
    OutEdgeIt e, eend;
    for (tie(e, eend) = out_edges(vertex(_source, G), G); e != eend; ++e)
      flow += capacity[*e] - res_capacity[*e];
    int cost = find_flow_cost(G);
    cout << flow * n_slots * MAX_PROFIT - cost << endl;
  }
}
