/* 
   topcoder: escape
   You are playing a video game that involves escaping from a dangerous area. 
   Within the area there are DEADLY regions you can't enter, HARMFUL regions 
   that take 1 life for every step you make in them, and NORMAL regions that don't affect you in any way. 
   You will start from (0,0) and have to make it to (500,500) using only Up, Left, Right, and Down steps. 
   The map will be given as a String[] deadly listing the DEADLY regions and a String[] harmful listing the HARMFUL regions.
   The elements in each of these parameters will be formatted as follows:
   Input format(quotes for clarity): "X1 Y1 X2 Y2" where
   (X1,Y1) is one corner of the region and
   (X2,Y2) is the other corner of the region
   The corners of the region are inclusive bounds (i.e. (4,1) and (2,2) 
   include x-values between 4 and 2 inclusive and y-values between 1 and 2 inclusive). 
   All unspecified regions are considered NORMAL. 
   If regions overlap for a particular square, then whichever region is worst takes effect 
   (e.g. DEADLY+HARMFUL = DEADLY, HARMFUL+NORMAL = HARMFUL, HARMFUL+HARMFUL = HARMFUL, DEADLY+NORMAL=DEADLY).
   Damage taken at each step occurs based on the destination square and not on the starting square (e.g. if the square (500,500)
   is HARMFUL you WILL take a point of damage stepping onto it; if the square (0,0) is HARMFUL you WON'T take a point of damage stepping off of it;
   this works analogously for DEADLY squares). 

   Return the least amount of life you will have to lose in order to reach the destination. 
   Return -1 if there is no path to the destination. 
   Your character is not allowed to leave the map (i.e. have X or Y less than 0 or greater than 500). 
*/

#include <array>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

const int map_size_x = 501;
const int map_size_y = 501;

enum class Region{Normal, Harmful, Deadly};

class Escape {

  struct Coordinate {
    int x;
    int y;
    Region region;
    int min_damage;
    Coordinate (void) {
      x = -1;
      y = -1;
      region = Region::Normal;
      min_damage = numeric_limits<int>::max();
    }
    Coordinate (int x, int y) : x(x), y(y), region(Region::Normal) {}
    bool isValid (void) {
      return (x >= 0 && x <= map_size_x &&
	      y >= 0 && y <= map_size_y &&
	      region != Region::Deadly);
    }

    int damage (void) {
      return (region == Region::Harmful) ? 1 : 0;
    }
    bool operator>(const Coordinate& rhs) const {
      return min_damage > rhs.min_damage;
    }
  };

  array<array<Coordinate,map_size_y>,map_size_x> map; 

  vector<int> parseRegion (const string& region) {
    stringstream ss(region);
    string point;
    vector<int> result;
    while (getline(ss, point, ' ')) {
      result.emplace_back(stoi(point));
    }
    return result;
  }

public:

  Escape (const vector<string>& regions) {
    //init all map to Normal
    for (int i = 0; i < map_size_x; ++i) {
      for (int j = 0; j < map_size_y; ++j) {
	map[i][j].x = i;
	map[i][j].y = j;
	map[i][j].region = Region::Normal;
      }
    }
    //set harmful areas
    vector<int> harmful = parseRegion(regions[0]);
    for (int i = harmful[0]; i < harmful[2]; ++i) {
      for (int j = harmful[1]; j < harmful[3]; ++j) {
	map[i][j].region = Region::Harmful;
      }
    }
    //set deadly areas
    vector<int> deadly = parseRegion(regions[1]);
    for (int i = deadly[0]; i < deadly[2]; ++i) {
      for (int j = deadly[1]; j < deadly[3]; ++j) {
	map[i][j].region = Region::Deadly;
      }
    }
  }

  int lowest (void) {
    //minimum amount of damages to reach each point
    //vector<vector<int>> damages(map_size_x, vector<int>(map_size_y, numeric_limits<int>::max()));//INIT to numeric_limits<int>::max()
    //queue for unexplored points 
    //auto cmp = [](const Coordinate& lhs, const Coordinate& rhs){return lhs.min_damage > rhs.min_damage;};
 
     priority_queue<Coordinate,vector<Coordinate>,greater<Coordinate>> unexplored;
    unexplored.emplace(map[0][0]);

    while (!unexplored.empty()) {
      const Coordinate& current = unexplored.top();
      unexplored.pop();

      if (current.x == map_size_x-1 && current.y == map_size_y-1) return current.min_damage;

      //if (damages[current.x][current.y] 
      //const array<Coordinate,4> steps = {{-1,0},{0,-1},{1,0},{0,1}};
      const vector<Coordinate> steps{{-1,0},{0,-1},{1,0},{0,1}};
      for (const auto& step : steps) {
	if (map[current.x + step.x][current.y + step.y].isValid() &&
	    (map[current.x][current.y].min_damage + map[current.x + step.x][current.y + step.y].damage() <
	     map[current.x + step.x][current.y + step.y].min_damage)) {
	  map[current.x + step.x][current.y + step.y].min_damage = map[current.x][current.y].min_damage + map[current.x + step.x][current.y + step.y].damage();
	  unexplored.emplace(map[current.x + step.x][current.y + step.y]);
	}
      }
 
    }
    return -1;
  }

};

int main (void) {
  Escape e({{"500 0 0 500"},{"0 0 0 0"}});
  cout << e.lowest();
}
  
