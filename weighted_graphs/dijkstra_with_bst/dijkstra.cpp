//O(Elog(V)) in the worst case all E edges are processed ond for each
//edge an erase/insert operation is performed in O(log(V)) time 

#include "../common/defs.h"
#include <iostream>
#include <limits>
#include <set>

using namespace std;

void shortestPath (const vector<int>& parents, vector<int>* path, int current, int start) {  
  if (parents[current] < 0) path->emplace_back(current);
  else { shortestPath (parents, path, parents[current], start);
  path->emplace_back(current);
  }
}

int Dijkstra(const vector<vector<Node>>& graph, int source, int destination) {
  //shortest distance of all vertices from source
  vector<int> distances(graph.size(), numeric_limits<int>::max());
  //parent of each vertex in the shortest path
  vector<int> parents(graph.size(), -1);
  set<Node> unknown;

  distances[source] = 0;
  unknown.emplace(source,0);

  while (!unknown.empty()) {
    //fetch the nearest unexplored node
    Node nearest = *(unknown.begin());
    unknown.erase(unknown.begin());

    // stop when destination is reached
    if (nearest.vertex == destination) break;

    // iterate through all outcoming edges from nearest
    for (const auto& edge : graph[nearest.vertex]){
      if (distances[nearest.vertex] + edge.weight < distances[edge.vertex]) {
	// update distance if possible
	distances[edge.vertex] = distances[nearest.vertex] + edge.weight;
	// update parent
	parents[edge.vertex] = nearest.vertex;
	// add the vertex to set or update if it was already in set
	if (unknown.find(edge) != unknown.end()) unknown.erase(unknown.find(edge)); //log cost for the first find
	//if (distances[edge.vertex] != numeric_limits<int>::max()) unknown.erase(unknown.find(edge)); //replace the find in O(1)
	unknown.emplace(edge.vertex,distances[edge.vertex]);	  
      }
    }
    
  }
  vector<int> shortest_path;
  shortestPath (parents, &shortest_path, destination, source);
  for (int vertex : shortest_path) cout << vertex << " ";
  cout << "\n";
  return distances[destination];
}


int main(void) {
  ifstream ifs( "../common/undirected_weighted_graph.txt" );
  if (!ifs) throw runtime_error("graph not defined!");

  auto graph = buildGraph(ifs);
  cout << Dijkstra(graph, 0, 8);
}
