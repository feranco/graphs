#include "../../unweighted_graphs/common/defs.h"
#include <iostream>
#include <stack>
#include <queue>
#include <vector>

using namespace std;

vector<int> shortestPath(const vector<vector<Edge>>& graph, int source, int destination) {

  const size_t vertices = graph.size();
  vector<int> parents(vertices, -1);
  vector<bool> visited(vertices, false);
  queue<int> discovered;//{source};
  discovered.push(source);
  visited[source] = true;
  bool pathFound = false;

  while (!discovered.empty() && !pathFound) {
    int vertex = discovered.front();
    std::cout << "S " << vertex << "D " << destination <<"\n";
    if (vertex == destination) {
      std::cout << "Found";
      pathFound = true;

    }
    else {
      discovered.pop();
      std::cout << "V " << vertex << " Adj ";
      for (auto adj : graph[vertex]) {
        if (visited[adj.vertex] == false) {
          std::cout << adj.vertex << " ";
        parents[adj.vertex] = vertex;
        discovered.push(adj.vertex);
        visited[adj.vertex] = true;
        }

      }
      std::cout << "\n";
    }
  }

    if (!pathFound) return {};

    vector<int> path;
    stack<int> reversedPath;//{discovered.front()};
    reversedPath.push(discovered.front());
    int parent = parents[discovered.front()];
    while (parent > 0) {
      reversedPath.push(parent);
      parent = parents[parent];
    }

    while(!reversedPath.empty()) {
      path.push_back(reversedPath.top());
      reversedPath.pop();
    }
    return path;


}

int main(void) {
  ifstream ifs( "graph.txt" );
  if (!ifs) throw runtime_error("graph not defined!");

  auto graph = buildGraph(ifs);
  auto path = shortestPath(graph,2,3);
  for (int vertex : path) cout << vertex << " ";
}
