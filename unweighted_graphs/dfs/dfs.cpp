//deep first search implementation
//O(V + E)

#include "../common/defs.h"
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

void dfs(const vector<vector<Edge>>& graph, int vertex, vector<bool>* processed, vector<int>* dfs_vertices) {

  for (const auto& edge : graph[vertex]) {
    if (!(*processed)[edge.vertex]) dfs(graph, edge.vertex, processed, dfs_vertices);
  }
  (*processed)[vertex] = true;
  dfs_vertices->emplace_back(vertex);
}

vector<int> dfs(const vector<vector<Edge>>& graph) {
  //stack containing vertex topologically sorted
  vector<int> dfs_vertices;
  //flag indicating already processed vertex
  vector<bool> processed(graph.size(),false);

  for (int i = 0; i < graph.size(); ++i) {
    if (!processed[i]) dfs(graph, i, &processed, &dfs_vertices);
  }

  //output dfs output
  return dfs_vertices; 
}

int main(void) {
  ifstream ifs( "../common/directed_acyclic_graph.txt" );
  if (!ifs) throw runtime_error("graph not defined!");
    
  auto graph = buildGraph(ifs);
  auto dfs_vertices = dfs(graph);
  for (int vertex : dfs_vertices) cout << vertex << " ";
}
