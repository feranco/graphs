//topcoder: bombman
/*
  Bomb Man is trapped inside a maze shaped like a grid. You must help him find the exit as fast as possible. 
  The maze will be given as a String[] where each element corresponds to a row in the grid and each character
  in an element corresponds to a cell in that row. '#' marks a wall, '.' an empty cell, 'B' the start position
  of Bomb Man and 'E' the exit. Below is an example of a maze in this format, given as a String[]:
  {".....B.",
  ".#####.",
  ".#...#.",
  ".#E#.#.",
  ".###.#.",
  "......."}
  In each time unit, Bomb Man can move one cell up, down, left or right. 
  Thus, in the maze above, he can reach the exit in 14 time units by just walking.
  To be able to reach the exit quicker, Bomb Man is in possession of a number of bombs, 
  each of which can blow a hole in a wall and thus open up new passages. 
  Instead of moving in any of the four cardinal directions, 
  Bomb Man can (if he has bombs left) blow a hole in a wall located in any of the four neighboring squares. 
  The bomb will go off in the time unit after he has placed the bomb, 
  creating an empty cell that Bomb Man can enter in the time unit after that. 
  That is, if he places a bomb in time unit t, he can enter the cell earliest in time unit t+2. 
  In the maze above, Bomb Man can then reach the exit in 8 time units by first walking left, placing a bomb
  , waiting for the bomb to explode, and then walking down, down, left, left and down.
  Create a class BombMan containing the method shortestPath which takes a String[] maze, containing the maze
  in the format described above, and an int bombs, the number of bombs in Bomb Man's possession. 
  The method should return an int, the least number of time units required for Bomb Man to reach the exit.
  If it's not possible for Bomb Man to reach the exit, return -1 (see example 1).
*/
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

class Bombman {
  struct Coordinate {
    int x;
    int y;
    int time_unit;
    bool isValid (const vector<vector<int>>& maze) {
      return (x >= 0 && x < maze.size() &&
	      y >= 0 && y < maze[0].size());
    }
    bool operator== (const Coordinate& rhs) {
      return (x == rhs.x && y < rhs.y);
    }
  };
  
  vector<vector<Coordinate>> maze;
  Coordinate start, end;
  int bombs;
public:
  
  Bombman (const vector<string>& _maze, int _bombs) : bombs(_bombs) {
    for (int row = 0; row < _maze.size(); ++row) {
      maze.emplace_back(vector<Coordinate>{});
      for (int col = 0; col < _maze[row].size(); ++col) {
	int time_unit = (_maze[row][col] == '#') ? 2 : 1;
	maze[row].emplace_back(Coordinate{row, col, time_unit});
	if (_maze[row][col] == 'B') {start.x = row; start.y = col;};
        if (_maze[row][col] == 'E') {end.x = row; end.y = col;};
      }
    }
  }
  
  int shortestPath (void) {
    vector<vector<int>> time_units(maze.size(),vector<int>(maze[0].size(),numeric_limits<int>::max()));
    vector<vector<int>> bombs_left(maze.size(),vector<int>(maze[0].size(),bombs));
    auto cmp = [&] (const Coordinate& lhs, const Coordinate& rhs)  {
      if (time_units[lhs.x][lhs.y] == time_units[rhs.x][rhs.y])
	return bombs_left[lhs.x][lhs.y] < bombs_left[rhs.x][rhs.y];
      else
	return time_units[lhs.x][lhs.y] > time_units[rhs.x][rhs.y];
    };
    priority_queue<Coordinate, vector<Coordinate>, decltype(cmp)> q(cmp);
    q.emplace(start);

    while (!q.empty()) {
      Coordinate current = q.top();
      q.pop();

      if (current == end) return time_units[current.x][current.y];

      if (current.time_unit < time_units[current.x][current.y]) {
	vector<pair<int,int>> destinations{{current.x-1, current.y},{current.x, current.y-1},{current.x+1, current.y},{current.x, current.y+1}};
	for (const auto& dest : destinations) {
	  if (dest.first >= 0 && dest.first < maze.size() && dest.second >= 0 && dest.second < maze[0].size() &&
	      time_units[current.x][current.y] + maze[dest.first][dest.second].time_unit < time_units[dest.first][dest.second]  &&
	      (maze[dest.first][dest.second].time_unit == 1 || bombs_left[current.x][current.y] > 0)) {
            
	    time_units[dest.first][dest.second] = time_units[current.x][current.y] + maze[dest.first][dest.second].time_unit;
	    q.emplace(Coordinate{dest.first, dest.second, time_units[dest.first][dest.second]});
	    if (bombs_left[current.x][current.y] > 0 && maze[dest.first][dest.second].time_unit == 2) bombs_left[current.x][current.y]--;
	  }
	}
      }
    }
			      
    return -1;
  }
  
};


int main (void) {
  
  vector<string> maze{".....B.",
      ".#####.",
      ".#...#.",
      ".#E#.#.",
      ".###.#.",
      "......."};
  Bombman b(maze,1);
  cout << b.shortestPath();
  
}
