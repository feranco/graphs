//Floyd-Warshall all pairs shortest path implementation
//O(V^3) consider each vertex (V) as possible intermediate vertex int the 
//shortest path between all pairs of vertices (V^2)
#include "../common/defs.h"
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

//this value shall be grater than the sum of all weights
//but not too large to prevent overflow
const int inf = 1 << 13; 

vector<vector<int>> floydWarshall (const vector<vector<Node>>& graph) {
  //build and initialize all pairs distances matrix
  vector<vector<int>> distances(graph.size(), vector<int>(graph.size(), inf));
  for (int i = 0; i < graph.size(); ++i) {
    distances[i][i] = 0; //important: the distance of each vertex from itself is 0
    for (int j = 0; j < graph[i].size();++j) {
      distances[i][graph[i][j].vertex] = graph[i][j].weight;
    }
  } 
	
  for (int k = 0; k < distances.size(); ++k) {
    for (int i = 0; i < distances.size(); ++i) {
      for (int j = 0; j < distances.size(); ++j)
	distances[i][j] = min(distances[i][j], distances[i][k] + distances[k][j]);
    }
  }
  return distances;
}

int main(void) {
  //ifstream ifs( "../common/all_pairs_graph.txt" );
  ifstream ifs( "all_pairs_graph.txt" );
  if (!ifs) throw runtime_error("graph not defined!");

  auto graph = buildGraph(ifs);
  auto distances = floydWarshall(graph);
}
