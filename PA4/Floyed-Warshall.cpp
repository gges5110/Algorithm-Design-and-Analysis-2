/*
In this assignment you will implement one or more algorithms for the all-pairs shortest-path
 problem. Here are data files describing three graphs:
g1.txt
g2.txt
g3.txt

The first line indicates the number of vertices and edges, respectively. Each subsequent line
describes an edge (the first two numbers are its tail and head, respectively) and its length (the
third number). NOTE: some of the edge lengths are negative. NOTE: These graphs may or may
not have negative-cost cycles.

Your task is to compute the "shortest shortest path". Precisely, you must first identify which, if
any, of the three graphs have no negative cycles. For each such graph, you should compute
all-pairs shortest paths and remember the smallest one (i.e., compute min_(u,v)∈V d(u,v),
where d(u,v) denotes the shortest-path distance from u to v).

If each of the three graphs has a negative-cost cycle, then enter "NULL" in the box below. If
exactly one graph has no negative-cost cycles, then enter the length of its shortest shortest
path in the box below. If two or more of the graphs have no negative-cost cycles, then enter
the smallest of the lengths of their shortest shortest paths in the box below.

OPTIONAL: You can use whatever algorithm you like to solve this question. If you have extra
time, try comparing the performance of different all-pairs shortest-path algorithms!

OPTIONAL: Here is a bigger data set to play with.
large.txt
For fun, try computing the shortest shortest path of the graph in the file above.

Floyed-Warshall Algorithm:
let dist be a |V| × |V| array of minimum distances initialized to ∞ (infinity)
for each vertex v
  dist[v][v] ← 0
for each edge (u,v)
  dist[u][v] ← w(u,v)  // the weight of the edge (u,v)
for k from 1 to |V|
  for i from 1 to |V|
     for j from 1 to |V|
        if dist[i][j] > dist[i][k] + dist[k][j]
             dist[i][j] ← dist[i][k] + dist[k][j]
         end if


g++ -o Floyed-Warshall.exe Floyed-Warshall.cpp -std=c++11
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <limits>

using std::vector;
using std::ifstream;
using std::numeric_limits;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Please provide a valid filename for testing." << std::endl;
  } else {
    std::ifstream fs;
    fs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
      fs.open(argv[1]);

      // Original main
      int n, m;
      fs >> n >> m;
      vector<vector<int>> dist(n, vector<int>(n, numeric_limits<int>::max()));
      for (int i = 0; i < n; ++i) {
        dist[i][i] = 0;
      }

      for (int i = 0; i < m; i++) {
        int x, y, w;
        fs >> x >> y >> w;
        dist[x - 1][y - 1] = w;
      }

      for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
          for (int j = 0; j < n; ++j) {
            if (dist[i][k] != numeric_limits<int>::max() && dist[k][j] != numeric_limits<int>::max()) {
              if (dist[i][j] > dist[i][k] + dist[k][j]) {
                // std::cout << "Updating dist[" << i << "][" << j << "], old = " << dist[i][j] << ", to " << dist[i][k] << " + " << dist[k][j] << " = " << dist[i][k] + dist[k][j] << std::endl;
                dist[i][j] = dist[i][k] + dist[k][j];
              }
            }
          }
        }
      }

      // Detect negative cycle
      for (int i = 0; i < n; ++i) {
        if (dist[i][i] < 0) {
          std::cout << "NULL" << std::endl;
          fs.close();
          return 0;
        }
      }

      int min = numeric_limits<int>::max();
      for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
          if (i != j) {
            min = std::min(min, dist[i][j]);
          }
        }
      }
      std::cout << min << std::endl;
      fs.close();
    } catch (std::ifstream::failure& e) {
      std::cerr << "Exception opening/reading file" << std::endl;
      std::cout << e.what() << std::endl;
    }
  }
  return 0;
}
