//topological sorting implementation
//topological sorting: linear ordering of graph's vertices such that
//all all directed edges go from left to right
//idea: apply dfs sorting the vertices in the reverse order they are processed
//when a directed edge (x,y) is explored only two cases are possible:
//1) y is undiscovered, so DFS is applied to y, y is processed before x and x
//appears before y in the topological sorting
//2) y is already processed and x appears before y in the topological sorting
//O(V + E)

#include "../common/defs.h"
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

void topologicalSorting(const vector<vector<Edge>>& graph, int vertex, vector<bool>* processed, stack<int>* sorted) {

  for (const auto& edge : graph[vertex]) {
    if (!(*processed)[edge.vertex]) topologicalSorting(graph, edge.vertex, processed, sorted);
  }
  (*processed)[vertex] = true;
  sorted->emplace(vertex);
}

vector<int> topologicalSorting(const vector<vector<Edge>>& graph) {
  //stack containing vertex topologically sorted
  stack<int> sorted;
  //flag indicating already processed vertex
  vector<bool> processed(graph.size(),false);

  for (int i = 0; i < graph.size(); ++i) {
    if (!processed[i]) topologicalSorting(graph, i, &processed, &sorted);
  }

  //output topologically sorted vertices 
  vector<int> result;
  while (!sorted.empty()) {
    result.emplace_back(sorted.top());
    cout << sorted.top() << " ";
    sorted.pop();
    
  }
  return result; 
}

int main(void) {
  ifstream ifs( "../common/directed_acyclic_graph.txt" );
  if (!ifs) throw runtime_error("graph not defined!");
    
  auto graph = buildGraph(ifs);
  auto sorted = topologicalSorting(graph);
}
