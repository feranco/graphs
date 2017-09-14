#include "../common/defs.h"
#include <iostream>
#include <limits>
#include <queue>

using namespace std;

void shortestPath (const vector<int>& parents, vector<int>* path, int current, int start) {  
  if (parents[current] < 0) path->emplace_back(current);
  else { shortestPath (parents, path, parents[current], start);
  path->emplace_back(current);
  }
}

vector<int> Dijkstra(const vector<vector<Node>>& graph, int source, int destination) {
  //shortest distance of all vertices from source
  vector<int> distances(graph.size(), numeric_limits<int>::max());
  vector<int> parents(graph.size(), -1);
  priority_queue<Node, vector<Node>, greater<Node>> unknown;

  distances[source] = 0;
  unknown.emplace(source,0);

  while (!unknown.empty()) {
    //fetch the nearest unexplored node
    Node nearest = unknown.top();
    unknown.pop();

    // stop when destination is reached
    if (nearest.vertex == destination) break;

    if(nearest.weight <= distances[nearest.vertex]) {
      // iterate through all outcoming edges from nearest
      for (const auto& edge : graph[nearest.vertex]){
	if (distances[nearest.vertex] + edge.weight < distances[edge.vertex]) {
	  // update distance if possible
	  distances[edge.vertex] = distances[nearest.vertex] + edge.weight;
	  // update parent
	  parents[edge.vertex] = nearest.vertex;
	  // add the vertex to queue
	  unknown.emplace(edge.vertex,distances[edge.vertex]);
	  
	}
      }
    }
  }
  vector<int> shortest_path;
  for (auto x : parents) cout << x << " ";
  cout << endl;
  for (auto x : distances) cout << x << " ";
  shortestPath (parents, &shortest_path, destination, source);
  return shortest_path;
}


int main(void) {
  ifstream ifs( "../common/undirected_weighted_graph.txt" );
  if (!ifs)
    {
      cout << "open fail 1" << endl;
    }

  auto graph = buildGraph(ifs);
  auto shortest_path = Dijkstra(graph, 0, 8);
  for (int vertex : shortest_path) cout << vertex << " ";
}
