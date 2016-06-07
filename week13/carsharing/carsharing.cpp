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

int vertices_between(int dep_t, int arr_t, vector<int> &times) {
  int pos1 = distance(times.begin(), equal_range(times.begin(),
                      times.end(), dep_t).first);
  int pos2 = distance(times.begin(), equal_range(times.begin(),
                      times.end(), arr_t).first);
  return pos2 - pos1;
}

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
    vector< set<int> > station_times(S);
    set<int> set_times;
    set< pair<int, int> > vertices;
    int t_0 = numeric_limits<int>::max();
    int t_max = 0;
    for (int i = 0; i < N; ++i) {
      int s, t, d, a, p;
      cin >> s >> t >> d >> a >> p;
      t_0 = min(t_0, d);
      t_max = max(t_max, a);
      --s; --t;
      requests.push_back(Request(s, t, d, a, p));
      station_times[s].insert(d);
      station_times[t].insert(a);
      vertices.insert(make_pair(s, d));
      vertices.insert(make_pair(t, a));
      set_times.insert(d);
      set_times.insert(a);
    }
    vector<int> all_times(set_times.begin(), set_times.end());
    vector< vector<int> > station_times_v(S);
    for (int i = 0; i < S; ++i)
      station_times_v[i] = vector<int> (station_times[i].begin(), station_times[i].end());
    // define the graph
    const int MAX_PROFIT = 100;
    Graph G(vertices.size());
    EdgeCapacityMap capacity = get(edge_capacity, G);
    EdgeWeightMap weight = get(edge_weight, G);
    ReverseEdgeMap rev_edge = get(edge_reverse, G);
    ResCapacityMap res_capacity = get(edge_residual_capacity, G);
    EdgeAdder ea(G, capacity, weight, rev_edge);
    Vertex _source = add_vertex(G);
    Vertex _sink = add_vertex(G);
    // offsets
    vector<int> offsets(S);
    int offset = 0;
    for (int i = 0; i < S; ++i) {
      offsets[i] = offset;
      offset += station_times[i].size();
    }
    // source -> first time (or source -> sink if no times)
    for (int i = 0; i < S; ++i) {
      if (station_times_v[i].size() == 0) continue;
      int between = vertices_between(t_0, station_times_v[i].front(), all_times);
      ea.addEdge(_source, offsets[i], sources[i], MAX_PROFIT * between);
    }
    // connection between times
    for (int i = 0; i < S; ++i) {
      if (station_times_v[i].size() == 0) continue;
      int between = vertices_between(station_times_v[i].back(), t_max, all_times);
      ea.addEdge(offsets[i] + station_times[i].size() - 1, _sink,
                 numeric_limits<int>::max(), MAX_PROFIT * between);
      for (int j = 0; j < station_times[i].size() - 1; ++j) {
        between = vertices_between(station_times_v[i][j], station_times_v[i][j + 1],
                                   all_times);
        ea.addEdge(offsets[i] + j, offsets[i] + j + 1, numeric_limits<int>::max(),
                   MAX_PROFIT * between);
      }
    }
    // request connections
    for (auto req: requests) {
      auto src_times = station_times_v[req.s];
      int pos1 = distance(src_times.begin(), equal_range(src_times.begin(),
                          src_times.end(), req.d).first);
      int u = offsets[req.s] + pos1;
      auto tgt_times = station_times_v[req.t];
      int pos2 = distance(tgt_times.begin(), equal_range(tgt_times.begin(),
                          tgt_times.end(), req.a).first);
      int v = offsets[req.t] + pos2;
      int between = vertices_between(req.d, req.a, all_times);
      ea.addEdge(u, v, 1, MAX_PROFIT * between - req.p);
    }
    //int flow = push_relabel_max_flow(G, _source, _sink);
    //cycle_canceling(G);
    successive_shortest_path_nonnegative_weights(G, _source, _sink);
    int flow = 0;
    OutEdgeIt e, eend;
    for (tie(e, eend) = out_edges(vertex(_source, G), G); e != eend; ++e)
      flow += capacity[*e] - res_capacity[*e];
    int cost = find_flow_cost(G);
    int max_dist = vertices_between(t_0, t_max, all_times);
    cout << flow * max_dist * MAX_PROFIT - cost << endl;
  }
  return 0;
}
