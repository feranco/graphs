//Bellman Ford implementation
//O(V*E) for each iteration all outcomin edge are explored updating 
//the shortest path from the source to each vertex if possible
//in the worst case V-1 iterations are performed
#include "../common/defs.h"
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

const int inf = 1 << 13;

vector<int> belmannFord(const vector<vector<Node>>& graph, int source) {
  //build and initialize all pairs distances matrix
  vector<int> distances(graph.size(), inf);
  distances[source] = 0;

  for (int k = 0; k < graph.size() - 1; ++k) {
    bool updated = false;
    //iterate an each vertex checking outcoming edges
    for (int i = 0; i < graph.size(); ++i) {
      for (int j = 0; j < graph[i].size(); ++j) {
	int dest = graph[i][j].vertex;
	int weight = graph[i][j].weight;
	if (distances[i] + weight < distances[dest]) {
	  distances[dest] = distances[i] + weight;
	  updated = true;
	}
      }
    }
    if (!updated) break;
  }
  return distances;
}

int main(void) {
  ifstream ifs( "../common/all_pairs_graph.txt" );
  if (!ifs) throw runtime_error("graph not defined!");

  auto graph = buildGraph(ifs);
  auto distances = belmannFord(graph,0);
}
