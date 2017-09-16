//topcoder: circuits
/*
  An essential part of circuit design and general system optimization is critical path analysis. 
  On a chip, the critical path represents the longest path any signal would have to travel during execution. 
  In this problem we will be analyzing chip designs to determine their critical path length. 
  The chips in this problem will not contain any cycles, i.e. there exists no path from one component of a chip back to itself.

  Given a String[] connects representing the wiring scheme, and a String[] costs representing the cost of each connection, your 
  method will return the size of the most costly path between any 2 components on the chip. In other words, you are to find the 
  longest path in a directed, acyclic graph. Element j of connects will list the components of the chip that can be reached directly
  from the jth component (0-based). Element j of costs will list the costs of each connection mentioned in the jth element of connects.
  As mentioned above, the chip will not contain any cyclic paths. For example:
  connects = {"1 2",
  "2",
  ""}
  costs    = {"5 3",
  "7",
  ""}
  In this example, component 0 connects to components 1 and 2 with costs 5 and 3 respectively. Component 1 connects to component 2 with a cost of 7. 
  All connections mentioned are directed. This means a connection from component i to component j does not imply a connection from component j to component i.
  Since we are looking for the longest path between any 2 components, your method would return 12.
*/

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <limits>
#include <stack>

using namespace std;

struct Edge {
  int component;
  int cost;
  Edge(int comp, int cost) : component(comp), cost(cost) {}
};

vector<int> parseInput(const string& input) {
  stringstream ss(input);
  string value;
  vector<int> result;
  while (getline(ss, value, ' ')) {
    result.emplace_back(stoi(value));
  }
  return result;
}

vector<vector<Edge>> buildGraph(const vector<string>& connects, const vector<string>& costs) {
  vector<vector<Edge>> graph(connects.size(),vector<Edge>());
  for (int i = 0; i < connects.size(); ++i) {
    auto connection = parseInput(connects[i]);
    auto cost = parseInput(costs[i]);
    for (int j = 0; j < connection.size(); ++j) {
			
      graph[i].emplace_back(connection[j], cost[j]);

    }
  }
  return graph;
}

void topologicalSort(const vector<vector<Edge>>& graph, int component, stack<int>* sorted, vector<bool>* processed) {
  for (const auto& edge : graph[component]) {
    if (!(*processed)[edge.component]) {
      topologicalSort(graph, edge.component, sorted, processed);
    }
  }
  (*processed)[component] = true;
  sorted->emplace(component);
}

int findCriticalPath(const vector<vector<Edge>>& graph, stack<int>* sorted) {
  vector<int> costs(sorted->size(), numeric_limits<int>::min());
  costs[sorted->top()] = 0;

  while (!sorted->empty()) {
    int component = sorted->top();
    sorted->pop();
    for (const auto& edge : graph[component]) {
      if (costs[component] + edge.cost > costs[edge.component]) {
	costs[edge.component] = costs[component] + edge.cost;
      }
    }
  }
  return costs.back();
}

int howLong(const vector<vector<Edge>>& graph) {
  //topologically sorted components
  stack<int> sorted;
  //processed components
  vector<bool> processed(graph.size());

  for (int i = 0; i < graph.size(); ++i) {
    if (!processed[i]) {
      topologicalSort(graph, i, &sorted, &processed);
    }
  }


  int main(void) {
    vector<string> connects{ "","2 3","3 4 5","4 6","5 6","7","5 7","" };
    vector<string> costs{ "","30 50","19 6 40","12 10","35 23","8","11 20","" };
    auto dag = buildGraph(connects, costs);
    cout << howLong(dag);
  }
