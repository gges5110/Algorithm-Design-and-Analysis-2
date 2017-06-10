#define DEBUG True
#ifdef DEBUG
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
#include <limits>
#include "prim.h"

using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::vector;
using std::numeric_limits;
using std::priority_queue;

class compareDist {
  const vector<int>& _dist;
public:
  compareDist(const vector<int>& dist): _dist(dist) {}

  bool operator()(int u, int v) {
    return _dist[u] < _dist[v];
  }
};

class Graph {
public:
  void read_file(std::ifstream& fs) {
    cout << "read file: " << endl;

    fs >> vertex_size >> edge_size;

    adjacency_list.resize(vertex_size, vector<int>(vertex_size, 0));

    int v1, v2, w;
    for (int i = 0; i < edge_size; i++) {
      fs >> v1 >> v2 >> w;
      // cout << v1 << " -> " << v2 << ": " << w << endl;
      adjacency_list[v1 - 1][v2 - 1] = w;
      adjacency_list[v2 - 1][v1 - 1] = w;
    }
  }

  int PrimMST() {
    cout << "Starting Prim's algorithm." << endl;
    vector<int> key(vertex_size, numeric_limits<int>::max());
    vector<int> parent(vertex_size, -1);
    int sum = 0;

    key[0] = 0;
    std::unordered_set<int> Q;
    MyQueue<int, vector<int>, compareDist> unvisited{compareDist(key)};
    for (int i = 0; i < vertex_size; ++i)
      unvisited.push(i);

    int iteration = 0;
    while (!unvisited.empty()) {
      cout << "Iteration: " << iteration++ << endl;
      // Find the min(Q) by key value
      int u = unvisited.top();
      unvisited.pop();
      if (parent[u] != -1) {
        sum += adjacency_list[u][parent[u]];
        cout << u << " -> " << parent[u] << endl;
      }

      cout << "Update vertex cost." << endl;
      for (int v = 0; v < vertex_size; ++v) {
        if (adjacency_list[u][v] != 0) {
          auto search = Q.find(v);
          if (search != Q.end() && adjacency_list[u][v] < key[v]) {
            parent[v] = u;
            key[v] = adjacency_list[u][v];
          }
        }
      }
    }

    return sum;
  }

  void print() {
    for (int i = 0; i < vertex_size; ++i) {
      for (int j = 0; j < vertex_size; ++j) {
        std::cout << adjacency_list[i][j] << " ";
      }
      std::cout << std::endl;
    }
  }

private:
  int vertex_size, edge_size;
  vector<vector<int>> adjacency_list;
  int find_min(const std::unordered_set<int> &Q, const vector<int> &key) const {
    int min_id, min = numeric_limits<int>::max();

    for (int id: Q) {
      cout << "find min id = " << id << endl;
      if (key[id] < min) {
        min_id = id;
        min = key[id];
      }
    }

    return min_id;
  }
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Please provide a valid filename for testing." << std::endl;
  } else {
    ifstream fs;
    fs.exceptions(ifstream::failbit | ifstream::badbit);
    try {
      fs.open(argv[1]);

      // Original main
      Graph graph;
      graph.read_file(fs);
      // graph.print();

      cout << "Minimum spanning tree weight = " << graph.PrimMST() << endl;
    } catch (ifstream::failure& e) {
      std::cerr << "Exception opening/reading file" << std::endl;
    }
    fs.close();
  }
  return 0;
}
#endif
