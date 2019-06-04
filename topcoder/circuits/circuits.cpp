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
#include <algorithm>
#include <gtest/gtest.h>

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
  for (size_t i = 0; i < connects.size(); ++i) {
    auto connection = parseInput(connects[i]);
    auto cost = parseInput(costs[i]);
    for (size_t j = 0; j < connection.size(); ++j) {

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
  return *max_element(costs.begin(), costs.end());
}

//Basically the problem ask to find the max cost path in a DAG
//First topological sorting and then starting from the first node
//explore each edge updating the cost of the destination node if
//the cost to reach the current node plus the cost of the edge exceeds
//the cost of the destination node
int howLong(const vector<vector<Edge>>& graph) {
  //topologically sorted components
  stack<int> sorted;
  //processed components
  vector<bool> processed(graph.size());

  for (size_t i = 0; i < graph.size(); ++i) {
    if (!processed[i]) {
      topologicalSort(graph, i, &sorted, &processed);
    }
  }
  return findCriticalPath(graph,  &sorted);
}

TEST(CircuitsTest, Test1) {
  vector<string> connects{ "1 2",
                           "2",
                           ""};
  vector<string> costs{"5 3",
                       "7",
                       ""};
  const int expected = 12;
  auto dag = buildGraph(connects, costs);
  ASSERT_EQ(howLong(dag),expected);
}

TEST(CircuitsTest, Test2) {
  vector<string> connects{ "1 2 3 4 5","2 3 4 5","3 4 5","4 5","5",""};
  vector<string> costs{ "2 2 2 2 2","2 2 2 2","2 2 2","2 2","2","" };
  const int expected = 10;
  auto dag = buildGraph(connects, costs);
  ASSERT_EQ(howLong(dag),expected);
}

TEST(CircuitsTest, Test3) {
  vector<string> connects{ "1 2 3 4 5","2 3 4 5","3 4 5","4 5","5",""};
  vector<string> costs{ "2 2 2 2 2","2 2 2 2","2 2 2","2 2","2","" };
  const int expected = 10;
  auto dag = buildGraph(connects, costs);
  ASSERT_EQ(howLong(dag),expected);
}

TEST(CircuitsTest, Test4) {
  vector<string> connects{ "","2 3 5","4 5","5 6","7","7 8","8 9","10",
                           "10 11 12","11","12","12",""};
  vector<string> costs{ "","3 2 9","2 4","6 9","3","1 2","1 2","5",
                        "5 6 9","2","5","3","" };
  const int expected = 22;
  auto dag = buildGraph(connects, costs);
  ASSERT_EQ(howLong(dag),expected);
}


int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
