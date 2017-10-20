//O(Elog(V)) in the worst case all E edges are processed and each
//insertion in the priority queue is performed in O(log(E)) time
//but  O(log(E)) =  O(log(V))
#include "../common/defs.h"
#include <iostream>
#include <limits>
#include <queue>

using namespace std;

struct Comp {
  //for bst shortest path
  bool operator() (const pair<int,int>& lhs, const pair<int,int>& rhs) const {
    //important: sort by distance (the second field of pair)
    return (lhs.second > rhs.second);
  }
};

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
  //open set (vertex,min_distance_to_vertex)  with vertices not explored
  priority_queue<pair<int,int>, vector<pair<int,int>>,Comp> open_set;
  //add source to open set and set its shortest distance to 0
  distances[source] = 0;
  open_set.emplace(make_pair(source,0));

  while (!open_set.empty()) {
    //fetch the nearest unexplored node u
    const auto nearest = open_set.top();
    open_set.pop();

    int u = nearest.first, weight_to_u = nearest.second;

    // stop when destination is reached
    if (u == destination) break;

    //check if u is still in the open set: with a pq the open set includes
    //multiple instances of a vertex, but only the nearest is explored
    //the other are discarded
    if(weight_to_u <= distances[u]) {
      // iterate through all outcoming edges v from u
      for (const auto& edge : graph[u]){
	int v = edge.vertex, w = edge.weight;
	if (distances[u] + w < distances[v]) {
	  // update distance if possible
	  distances[v] = distances[u] + w;
	  // update parent
	  parents[v] = u;
	  // add the vertex to queue
	  open_set.emplace(v,distances[v]);	  
	}
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
