#include <fstream>
#include <stdexcept>
#include <vector>

using namespace std;

struct Node {
  int vertex;
  int weight;
  Node(int x, int y) : vertex(x), weight(y) {}
  bool operator> (const Node& rhs) const {
    return (weight > rhs.weight);
  }
};

vector<vector<Node>> buildGraph(ifstream& ifs) {
  int n_vertex;
  bool directed;
  ifs >> n_vertex >> directed;

  if (n_vertex <= 0) {
    throw std::invalid_argument("Number of vertices in a Graph must be nonnegative");
  }

  vector<vector<Node>> graph(n_vertex, vector<Node>());

  int start, end, weight;
  while (ifs >> start >> end >> weight) {
    graph[start].emplace_back(end, weight);
    if (!directed) {
      graph[end].emplace_back(start, weight);
    }
  }
  return graph;
}
