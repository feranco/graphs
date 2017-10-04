//breadth first search implementation
//mark as discovered before push in the queue
//O(V + E)

#include "../common/defs.h"
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

//start: vertex from where start bfs
vector<int> bfs(const vector<vector<Edge>>& graph, int start) {
  //vector contain vertices visited by bfs
  vector<int> bfs_vertices;
  
  //flag indicating already discovered vertex
  vector<bool> discovered(graph.size(),false);

  //queue containing vertices not already processed
  queue<int> bfs_queue;
  discovered[start] = true;
  bfs_queue.push(start);
  

  while (!bfs_queue.empty()) {
    int current_vertex = bfs_queue.front();
    bfs_queue.pop();
    //visit current vertex
    bfs_vertices.emplace_back(current_vertex);
    
    //cycle on (outgoing) edges of current vertex
    for (const auto& edge : graph[current_vertex]) {
      if (!discovered[edge.vertex]) {
	//it's important to mark a vertex as processed before
	//to enqueue it, so that the queue has not duplicates
	discovered[edge.vertex] = true;
	bfs_queue.emplace(edge.vertex);
      }
    }
  }

  //output bfs output
  return bfs_vertices; 
}

int main(void) {
  ifstream ifs( "../common/directed_acyclic_graph.txt" );
  if (!ifs) throw runtime_error("graph not defined!");
    
  auto graph = buildGraph(ifs);
  auto bfs_vertices = bfs(graph, 0);
  for (int vertex : bfs_vertices) cout << vertex << " ";
}
