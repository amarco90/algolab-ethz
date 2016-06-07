#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bipartite.hpp>
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
typedef graph_traits<Graph>::vertex_iterator VertexIt;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::edge_iterator EdgeIt;
typedef graph_traits<Graph>::out_edge_iterator OutEdgeIt;
// for bipartition
typedef std::vector <default_color_type> partition_t;
typedef typename property_map <Graph, vertex_index_t>::type index_map_t;
typedef iterator_property_map <partition_t::iterator, index_map_t> partition_map_t;

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
    int k, s, m;
    cin >> k >> s >> m;
    int num_vert = k * s;
    Graph G(num_vert);
    for (int i = 0; i < m; ++i) {
      int u, v, h;
      cin >> u >> v >> h;
      for (int j = 0; j < h; ++j) {
        int x, y;
        cin >> x >> y;
        add_edge(u * s + x, v * s + y, G);
      }
    }
    // build bipartite graph
    partition_t partition(num_vertices (G));
    partition_map_t partition_map(partition.begin(), get(vertex_index, G));
    bool is_bip = is_bipartite(G, get(vertex_index, G), partition_map);
    Graph bipG(num_vert);
    EdgeCapacityMap capacity = get(edge_capacity, bipG);
    ReverseEdgeMap rev_edge = get(edge_reverse, bipG);
    ResidualCapacityMap res_capacity = get(edge_residual_capacity, bipG);
    EdgeAdder ea(bipG, capacity, rev_edge);
    Vertex _source = add_vertex(bipG);
    Vertex _sink = add_vertex(bipG);
    if (!is_bip) continue;
    vector<Vertex> white;
    vector<Vertex> black;
    VertexIt vit, vend;
    for (boost::tie(vit, vend) = vertices(G); vit != vend; ++vit) {
      auto color = get(partition_map, *vit);
      if (color == color_traits<default_color_type>::white()) {
        ea.addEdge(_source, *vit, 1);
        white.push_back(*vit);
      } else {
        ea.addEdge(*vit, _sink, 1);
        black.push_back(*vit);
      }
    }
    for (auto wv: white) {
      OutEdgeIt ebeg, eend;
      for (tie(ebeg, eend) = out_edges(wv, G); ebeg != eend; ++ebeg) {
        const int v = target(*ebeg, G);
        ea.addEdge(wv, v, 1);
      }
    }
    for (auto bv: black) {
      OutEdgeIt ebeg, eend;
      for (tie(ebeg, eend) = out_edges(bv, G); ebeg != eend; ++ebeg) {
        const int v = target(*ebeg, bipG);
        ea.addEdge(v, bv, 1);
      }
    }
    push_relabel_max_flow(bipG, _source, _sink);
    vector<int> vis(num_vert + 2, false);
    vis[_source] = true;
    std::queue<int> Q;
    Q.push(_source);
    while (!Q.empty()) {
      const int u = Q.front();
      Q.pop();
      OutEdgeIt ebeg, eend;
      for (tie(ebeg, eend) = out_edges(u, bipG); ebeg != eend; ++ebeg) {
        const int v = target(*ebeg, bipG);
        if (res_capacity[*ebeg] == 0 || vis[v]) continue;
        vis[v] = true;
        Q.push(v);
      }
    }
    int vertex_cover = 0;
    for (auto it = white.begin(); it != white.end(); ++it)
      if (!vis[*it]) ++vertex_cover;
    for (auto it = black.begin(); it != black.end(); ++it)
      if (vis[*it]) ++vertex_cover;
    cout << num_vert - vertex_cover << endl;
  }
  return 0;
}

