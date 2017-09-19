#include "../common/defs.h"
#include <iostream>
#include <limits>
#include <queue>
#include <algorithm>

using namespace std;

//perform bfs on residual graph to find an augmenting path and store the path in parents
bool findAugmentingPath(const vector<vector<int>>& residual_graph, int source, int sink, vector<int>* parents) {
  //reset parent's path
  parents->assign(parents->size(),-1);
  //flag for already discovered vertices
  vector<bool> discovered(parents->size(), false);
  //queue containing already discovered vertices to be processed
  queue<int> bfs_queue;
  bfs_queue.emplace(source);
  discovered[source] = true;
	
  //perform bfs 
  while (!bfs_queue.empty()) {
    int current = bfs_queue.front();
    bfs_queue.pop();
		
    if (current == sink) return true;
		
    for (int i = 0; i < residual_graph.size(); ++i) {
      if (!discovered[i] && residual_graph[current][i] > 0) {
	discovered[i] = true;
	parents->at(i) = current;
	bfs_queue.emplace(i);
      }
    }
  }
  return false;
}
//go back through augmenting path computing its flow
int augmentingPathFlow (const vector<vector<int>>& residual_graph, int sink, vector<int>& parents) {
  int flow = numeric_limits<int>::max();
  //for each edge of the augmenting path the flow is the minumum
  //between the current flow and the edge residual capacity
  for (int i = sink; parents[i] >= 0; i = parents[i]) {
    flow = min(flow,residual_graph[parents[i]][i]);
  }
	
  return flow;
}

//go back through augmenting path updating the residual graph
void updateResidualGraph (vector<vector<int>>* residual_graph_ptr, int flow, int sink, vector<int>& parents) {
  vector<vector<int>>& residual_graph = *residual_graph_ptr;
  //for each edge of the augmenting path the residual capacity
  //of the edge is decreased by the flow and the residual capacity 
  //of the backward edge is increased by the flow
  for (int i = sink;  parents[i] >= 0; i = parents[i]) {
    residual_graph[parents[i]][i] -= flow;
    residual_graph[i][parents[i]] += flow;
  }
}

int maxFlow (const vector<vector<Node>>& graph, int source, int sink) {
	
  //build residual graph initializing edge capacities with the weights of the input graph
  vector<vector<int>> residual_graph(graph.size(),vector<int>(graph.size(),0));
  for (int i = 0; i < graph.size(); ++i) {
    for (const auto& edge : graph[i]) {
      residual_graph[i][edge.vertex] = edge.weight;
    }
  }
	
  int max_flow = 0;
  vector<int> parents(graph.size(),-1);
	
  //increase the max_flow until an augmenting path exists
  while (findAugmentingPath(residual_graph, source, sink, &parents)) {
    int flow = augmentingPathFlow(residual_graph, sink, parents);
    max_flow += flow;
    updateResidualGraph(&residual_graph, flow, sink, parents);
  }
  return max_flow;
}


int main(void) {
  ifstream ifs( "../common/max_flow_graph.txt" );
  if (!ifs) throw runtime_error("graph not defined!");

  auto graph = buildGraph(ifs);
  cout << maxFlow(graph, 0, 5);
}
