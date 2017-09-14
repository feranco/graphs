#include <fstream>
#include <stdexcept>
#include <vector>

using namespace std;

struct Edge {
  int vertex;
  Edge(int x) : vertex(x) {}
};

vector<vector<Edge>> buildGraph(ifstream& ifs) {
  int n_vertex;
  bool directed;
  ifs >> n_vertex >> directed;

  if (n_vertex <= 0) {
    throw std::invalid_argument("Number of vertices in a Graph must be nonnegative");
  }

  vector<vector<Edge>> graph(n_vertex, vector<Edge>());

  int start, end;
  while (ifs >> start >> end) {
    graph[start].emplace_back(end);
    if (!directed) {
      graph[end].emplace_back(start);
    }
  }
  return graph;
}
