#include <iostream>
#include <algorithm>
#include <limits>
#include <stack>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/tuple/tuple.hpp>

using namespace boost;
using namespace std;

typedef adjacency_list<vecS, vecS, undirectedS, no_property, property<edge_weight_t, int> > Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::edge_iterator EdgeIt;
typedef graph_traits<Graph>::out_edge_iterator OutEdgeIt;
typedef property_map<Graph, edge_weight_t>::type WeightMap;

// WARNING! costs is an in-out parameter
void dfs(int src, vector<bool> &visited, Graph &T, WeightMap &wm,
    vector<int> &costs, int max_edge) {
  costs[src] = max_edge;
  OutEdgeIt ei, oeiend;
  for (tie(ei, oeiend) = out_edges(src, T); ei != oeiend; ++ei) {
    int edge_w = max(max_edge, wm[*ei]);
    Vertex v = target(*ei, T);
    if (!visited[v]) {
      visited[v] = true;
      dfs(v, visited, T, wm, costs, edge_w);
    }
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while (T--) {
    int n, i_tatoo;
    cin >> n >> i_tatoo;

    Graph G(n);
    WeightMap wm = get(edge_weight, G);
    for (int j = 0; j < n-1; ++j) {
      for (int k = j+1; k < n; ++k) {
        int w;
        cin >> w;
        Edge e;
        tie(e, tuples::ignore) = add_edge(j, k, G);
        wm[e] = w;
      }
    }
    // Build MST T
    vector<Edge> mst;
    kruskal_minimum_spanning_tree(G, back_inserter(mst));
    long long mst_cost = 0;
    Graph T(n);
    WeightMap wm_T = get(edge_weight, T);
    for (auto ite = mst.begin(); ite != mst.end(); ++ite) {
      Edge e;
      Vertex u = source(*ite, G);
      Vertex v = target(*ite, G);
      tie(e, tuples::ignore) = add_edge(u, v, T);
      int cost = wm[*ite];
      wm_T[e] = cost;
      mst_cost += cost;
    }
    // DFS on T for each vertex
    vector< vector<int> > max_edge(n); // max edge in the path from v to each
                                       // other vertex in the mst
    for (int v = 0; v < n; ++v) {
      vector<int> costs(n, 0);
      vector<bool> visited(n, false);
      visited[v] = true;
      dfs(v, visited, T, wm_T, costs, 0);
      max_edge[v] = costs;
    }
    long long second_mst = numeric_limits<long long>::max();
    EdgeIt ei, eend;
    for (tie(ei, eend) = edges(G); ei != eend; ++ei) {
      Vertex u = source(*ei, G);
      Vertex v = target(*ei, G);
      // edge(u,v,g) returns pair<edge_descriptor, bool> where the bool is
      // whether the edge exists
      if (edge(u, v, T).second) continue; // if it's in the MST skip it!
      int delta = wm[*ei] - max_edge[u][v];
      second_mst = min(second_mst, mst_cost + delta);
    }
    cout << second_mst << endl;
  }
  return 0;
}
