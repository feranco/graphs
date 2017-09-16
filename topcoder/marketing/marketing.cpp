//topcoder: marketing
/*
You work for a very large company that markets many different products. 
In some cases, one product you market competes with another. 
To help deal with this situation you have split the intended consumers into two groups, namely Adults and Teenagers.
If your company markets 2 products that compete with each other, selling one to Adults and the other to Teenagers will help maximize profits.
Given a list of the products that compete with each other, 
you are going to determine whether all can be marketed such that no pair of competing products are both sold to Teenagers or both sold to Adults. 
If such an arrangement is not feasible your method will return -1. Otherwise, it should return the number of possible ways of marketing all of the products.
The products will be given in a String[] compete whose kth element describes product k. The kth element will be a single-space delimited list of integers.
 These integers will refer to the products that the kth product competes with. For example:
compete = {"1 4",
            "2",
            "3",
            "0",
	    ""}
The example above shows product 0 competes with 1 and 4, product 1 competes with 2, product 2 competes with 3, and product 3 competes with 0. 
Note, competition is symmetric so product 1 competing with product 2 means product 2 competes with product 1 as well.
Ways to market:
1) 0 to Teenagers, 1 to Adults, 2 to Teenagers, 3 to Adults, and 4 to Adults
2) 0 to Adults, 1 to Teenagers, 2 to Adults, 3 to Teenagers, and 4 to Teenagers
Your method would return 2.
*/

#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

enum class Consumers {Adults, Teenagers, None};

vector<int> parseProduct (const string& product) {
  vector<int> result;
  stringstream ss(product);
  string competitor;
  while (getline(ss, competitor, ' ')) {
    result.emplace_back(stoi(competitor));
  }
  return result;
}

vector<vector<int>> buildGraph (const vector<string>& compete) {
  vector<vector<int>> graph(compete.size(), vector<int>());
  for (int i = 0; i < compete.size(); ++i) {
    vector<int> competitors = parseProduct(compete[i]);
    for (int product : competitors) {
      graph[i].emplace_back(product);
      //graph shall be undirected, so add back edges
      graph[product].emplace_back(i);
    }
  }
  return graph;
}

bool howMany (const vector<vector<int>>& graph, int product, Consumers label, vector<bool>* discovered, vector<Consumers>* product_labels) {
  (*discovered)[product] = true;
  (*product_labels)[product] = label;
  for (const auto& competitor : graph[product]) {
    if (!(*discovered)[competitor]) howMany(graph, competitor, (label == Consumers::Adults) ? Consumers::Teenagers :  Consumers::Adults, discovered, product_labels);
    else if ((*product_labels)[competitor] == label) return false; 
  }
  return true;
}

long int howMany (const vector<vector<int>>& graph) {
  vector<bool> processed_products(graph.size(), false);
  vector<Consumers> product_labels(graph.size(), Consumers::None);;
  int combinations = 0;

  for (int i = 0; i < graph.size(); ++i) {
    if (!processed_products[i]) {
      if (!howMany(graph, i, Consumers::Adults, &processed_products, &product_labels)) return -1;
      ++combinations;
    }
  }
  return pow(2,combinations);
}

int main (void) {
  vector<vector<string>> competes{{"1 4","2","3","0",""}, {"1","2","0"}, {"1","2","3","0","0 5","1"},
			          {"","","","","","","","","","","","","","","","","","","","",
				      "","","","","","","","","",""}, {"1","2","3","0","5","6","4"}};
  for (const auto& compete : competes) {
    vector<vector<int>> graph = buildGraph(compete);
    cout << howMany(graph) << endl;
  }
 
}
