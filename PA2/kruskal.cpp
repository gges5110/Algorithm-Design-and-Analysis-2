/*
Compile command:
g++ -o kruskal.exe -I ./ kruskal.cpp -std=c++11
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <fstream>
#include "UnionFind.h"

using std::vector;
using std::cout;
using std::endl;
using std::ifstream;

class Edge {
public:
  int v1, v2, w;

  Edge(int _v1, int _v2, int _w) : v1(_v1), v2(_v2), w(_w) {}
  bool operator < (const Edge &that) const {
    return (this->w < that.w);
  }

  void print() {
    cout << v1 << ", " << v2 << ", " << w << endl;
  }
};

class Kruskal {
public:
  int vertex_size;
  vector<Edge> edges;

  Kruskal(int vertex_size, const vector<Edge> &edges) : vertex_size(vertex_size), edges(edges) {}

  vector<Edge> MST() {
    return MST(0);
  }

  vector<Edge> MST(int k) {
    vector<Edge> T;

    // Sort the edges in order of increasing cost
    std::sort(edges.begin(), edges.end());

    UF union_find(vertex_size);

    for (Edge e: edges) {
      if (!union_find.find(e.v1, e.v2)) {
        T.push_back(e);
        union_find.unite(e.v1, e.v2);
      }
      if (T.size() == vertex_size - k) {
        break;
      }
    }
    return T;
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
      int vertex_size, lines;
      vector<Edge> edges;
      fs >> vertex_size >> lines;
      int v1, v2, w;
      for (int line = 0; line < lines - 1; ++line) {
        fs >> v1 >> v2 >> w;
        Edge e(v1, v2, w);
        edges.push_back(e);
      }

      Kruskal k(vertex_size, edges);
      vector<Edge> T = k.MST(4);
      cout << T[T.size() - 1].w << endl;
    } catch (ifstream::failure& e) {
      std::cerr << "Exception opening/reading file" << std::endl;
      cout << e.what() << endl;
    }
    fs.close();
  }
  return 0;
}
