//O(Elog(V)) in the worst case all E edges are processed ond for each
//edge an erase/insert operation is performed in O(log(V)) time 

#include "../common/defs.h"
#include <iostream>
#include <limits>
#include <set>
#include <utility>

using namespace std;

struct Comp {
  //for bst shortest path
  bool operator() (const pair<int,int>& lhs, const pair<int,int>& rhs) const {
    //important: sort by distance (the second field of pair)
    return (lhs.second < rhs.second);
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
  set<pair<int,int>,Comp> open_set;
  //add s to open set and set its shortest distance to 0
  distances[source] = 0;
  open_set.emplace(make_pair(source,0));

  while (!open_set.empty()) {
    //fetch the nearest unexplored node u
    const auto nearest = *(open_set.begin());
    open_set.erase(open_set.begin());
    int u = nearest.first;

    // stop when destination is reached
    if (u == destination) break;

    // iterate through all outcoming edges v from u 
    for (const auto& edge : graph[u]){
      int v = edge.vertex, w = edge.weight;
      if (distances[u] + w < distances[v]) {
	// add the vertex to set or update if it was already in set
	if (distances[v] < numeric_limits<int>::max()) open_set.erase(open_set.find({v,distances[v]}));
	// update distance if possible
	distances[v] = distances[u] + w;
	// update parent
	parents[v] = u;
	open_set.emplace(make_pair(v,distances[v]));	  
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
