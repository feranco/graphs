/*
There are a total of n courses you have to take, labeled from 0 to n - 1.
Some courses may have prerequisites, for example to take course 0 you have to first take course 1, which is expressed as a pair: [0,1]
Given the total number of courses and a list of prerequisite pairs, return the ordering of courses you should take to finish all courses.
There may be multiple correct orders, you just need to return one of them. If it is impossible to finish all courses, return an empty array.
https://leetcode.com/problems/course-schedule-ii/description/
*/

#include <vector>
#include <iostream>
#include <stack>
#include <utility>

using namespace std;

vector<vector<int>> buildGraph (int numCourses, vector<pair<int,int>>& prerequisites) {
  vector<vector<int>> graph(numCourses, vector<int>(numCourses,0));
  for (const auto& edge : prerequisites) {
    graph[edge.second][edge.first] = 1;
  }
  return graph;
}

bool topSort(const vector<vector<int>>& graph, int v, stack<int>* s, vector<int>* discovered, vector<int>* processed) {
  (*discovered)[v] = 1;
  for (int i = 0; i < graph.size(); ++i) {
    if (graph[v][i]) {
      if ((*discovered)[i] && !(*processed)[i]) return false; //graph is not a dag
      if (!(*discovered)[i]) {
	if (!topSort(graph, i, s, discovered, processed))
	  return false;
      } 
    }
  }
  (*processed)[v] = 1;
  s->push(v);
  return true;
} 

vector<int> findOrder(int numCourses, vector<pair<int,int>>& prerequisites) {
  auto graph = buildGraph(numCourses, prerequisites);

  stack<int> s;
  vector<int> discovered(numCourses,0);
  vector<int> processed(numCourses,0);

  for (int i = 0; i < numCourses; ++i) {
    if (!discovered[i]) {			
      if (!topSort(graph, i, &s, &discovered, &processed)) return {};
    }
  }

  vector<int> result;
  while (!s.empty()) {
    result.emplace_back(s.top());
    s.pop();
  }
  return result;
}

int main (void) {
  vector<pair<int,int>> prerequisites{{1,0},{2,0},{3,1},{3,2}};
  auto order = findOrder(4,prerequisites);
  for (int x : order) cout << x << " ";
}
