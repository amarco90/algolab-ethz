#include <iostream>
#include <algorithm>
#include <limits>
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

void connect(map<pair<int, int>, Vertex> &mapping, EdgeAdder &ea, int u, int i, int j) {
  auto it = mapping.find(make_pair(i, j));
  if (it != mapping.end()) {
    Vertex v = it->second;
    ea.addEdge(u, v, 1);
  }
}

int main() {
  int T;
  cin >> T;
  while (T--) {
    int n;
    cin >> n;
    map<pair<int, int>, Vertex> mapping;
    int n_fields = 0;
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        bool field;
        cin >> field;
        if (field) {
          mapping.insert(make_pair(make_pair(i, j), n_fields));
          ++n_fields;
        }
      }
    }
    vector<Vertex> black; // black vertices
    vector<Vertex> white; // white vertices
    Graph G(n_fields);
    EdgeCapacityMap capacity = get(edge_capacity, G);
    ReverseEdgeMap rev_edge = get(edge_reverse, G);
    ResidualCapacityMap res_capacity = get(edge_residual_capacity, G);
    EdgeAdder ea(G, capacity, rev_edge);
    Vertex _source = add_vertex(G);
    Vertex _sink = add_vertex(G);
    for (auto it = mapping.begin(); it != mapping.end(); ++it) {
      auto pos = it->first;
      int i = pos.first;
      int j = pos.second;
      Vertex u = it->second;
      if ((i + j)%2 == 0) { // black
        black.push_back(u);
        ea.addEdge(_source, u, 1);

        connect(mapping, ea, u, i - 1, j - 2);
        connect(mapping, ea, u, i - 2, j - 1);
        connect(mapping, ea, u, i + 1, j - 2);
        connect(mapping, ea, u, i + 2, j - 1);
        connect(mapping, ea, u, i - 1, j + 2);
        connect(mapping, ea, u, i - 2, j + 1);
        connect(mapping, ea, u, i + 1, j + 2);
        connect(mapping, ea, u, i + 2, j + 1);
      } else { // white
        white.push_back(u);
        ea.addEdge(u, _sink, 1);
      }
    }
    push_relabel_max_flow(G, _source, _sink);
    vector<int> vis(n_fields + 2, false);
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
    int vertex_cover = 0;
    for (auto it = black.begin(); it != black.end(); ++it)
      if (!vis[*it]) ++vertex_cover;
    for (auto it = white.begin(); it != white.end(); ++it)
      if (vis[*it]) ++vertex_cover;
    cout << n_fields - vertex_cover << endl;
  }
  return 0;
}
