#include "../common/defs.h"
#include <iostream>
#include <limits>
#include <queue>
#include <numeric>

using namespace std;

void printMST (const vector<int>&parents) {
  for (int i = 1; i < parents.size(); ++i) {
    cout <<  parents[i] << " - " << i << endl;
  }
}

int prim (const vector<vector<Node>>& graph) {
  //weight associated to each vertex in the MST
  vector<int> weights(graph.size(), numeric_limits<int>::max());
  //flag reporting if a vertex is already in MST
  vector<bool> in_mst(graph.size(), false);
  //parent of each vertex in the MST
  vector<int> parents(graph.size(), -1);
  //min heap with not explored edges
  priority_queue<Node, vector<Node>, greater<Node>> unknown;

  unknown.emplace(0,0);
  weights[0] = 0;

  while (!unknown.empty()) {
    //fetch the nearest unexplored edge
    Node nearest = unknown.top();
    unknown.pop();
    in_mst[nearest.vertex] = true;
    for (const auto& edge : graph[nearest.vertex]) {
      //check if nearest isn't yet in the MST and try to update the weight of the end vertex
      //note that in dijkstra the check on the weight is enough because even if edge.vertex
      //were already in the tree, edge.weight would be always greater than the weights[edge.vertex]
      if (!in_mst[edge.vertex] && (edge.weight < weights[edge.vertex])) {
	weights[edge.vertex] = edge.weight;
	parents[edge.vertex] = nearest.vertex;
        unknown.emplace(edge.vertex, edge.weight);
      }
    }
  }
  printMST(parents);
  return accumulate(weights.begin(), weights.end(), 0);							     
}

int main(void) {
  ifstream ifs( "../common/undirected_weighted_graph.txt" );
  if (!ifs) throw runtime_error("graph not defined!");
    
  auto graph = buildGraph(ifs);
  cout <<   prim(graph);
}
