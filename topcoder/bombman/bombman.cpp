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
    bool isValid(const vector<vector<int>>& maze) {
      return (x >= 0 && x < maze.size() &&
	      y >= 0 && y < maze[0].size());
    }
    bool operator== (const Coordinate& rhs) {
      return (x == rhs.x && y == rhs.y);
    }
  };

  vector<vector<Coordinate>> maze;
  Coordinate start, end;
  int bombs;
public:

  Bombman(const vector<string>& _maze, int _bombs) : bombs(_bombs) {
    for (int row = 0; row < _maze.size(); ++row) {
      maze.emplace_back(vector<Coordinate>{});
      for (int col = 0; col < _maze[row].size(); ++col) {
	int time_unit = (_maze[row][col] == '#') ? 3 : 1;
	maze[row].emplace_back(Coordinate{ row, col, time_unit });
	if (_maze[row][col] == 'B') { start.x = row; start.y = col; };
	if (_maze[row][col] == 'E') { end.x = row; end.y = col; };
      }
    }
  }

  int shortestPath(void) {
    vector<vector<int>> time_units(maze.size(), vector<int>(maze[0].size(), numeric_limits<int>::max()));
    vector<vector<int>> bombs_left(maze.size(), vector<int>(maze[0].size(), bombs));
    auto cmp = [&](const Coordinate& lhs, const Coordinate& rhs) {
      if (time_units[lhs.x][lhs.y] == time_units[rhs.x][rhs.y])
	return bombs_left[lhs.x][lhs.y] < bombs_left[rhs.x][rhs.y];
      else
	return time_units[lhs.x][lhs.y] > time_units[rhs.x][rhs.y];
    };
    priority_queue<Coordinate, vector<Coordinate>, decltype(cmp)> q(cmp);
    q.emplace(start);
    time_units[start.x][start.y] = 0;

    while (!q.empty()) {
      Coordinate current = q.top();
      q.pop();

      if (current == end) {
	return time_units[current.x][current.y];
      }

      if (current.time_unit <= time_units[current.x][current.y]) {
	vector<pair<int, int>> destinations{ { current.x - 1, current.y },{ current.x, current.y - 1 },{ current.x + 1, current.y },{ current.x, current.y + 1 } };
	for (const auto& dest : destinations) {
	  if (dest.first >= 0 && dest.first < maze.size() && dest.second >= 0 && dest.second < maze[0].size() &&
	      time_units[current.x][current.y] + maze[dest.first][dest.second].time_unit < time_units[dest.first][dest.second] &&
	      (maze[dest.first][dest.second].time_unit == 1 || bombs_left[current.x][current.y] > 0)) {

	    time_units[dest.first][dest.second] = time_units[current.x][current.y] + maze[dest.first][dest.second].time_unit;
	    q.emplace(Coordinate{ dest.first, dest.second, time_units[dest.first][dest.second] });
	    if (bombs_left[current.x][current.y] > 0 && maze[dest.first][dest.second].time_unit == 3) {
	      bombs_left[dest.first][dest.second] = bombs_left[current.x][current.y]-1;
	    }
	    else {
	      bombs_left[dest.first][dest.second] = bombs_left[current.x][current.y];
	    }
	  }
	}
      }
    }

    return -1;
  }

};


int main(void) {

  vector<vector<string>> mazes{ { ".....B.",
	".#####.",
	".#...#.",
	".#E#.#.",
	".###.#.",
	"......." },
      { "B.#.#.#...E" } };
  vector<int> bombs{ 1,2,4 };
  for (int i = 0; i < mazes.size(); ++i) {
    Bombman b(mazes[i], bombs[i]);
    cout << b.shortestPath() << endl;
  }

}
