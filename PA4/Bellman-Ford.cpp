#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using std::vector;
using std::cout;
using std::endl;
using std::string;

class Edge {
public:
    int v1, v2, weight;
    Edge(int _v1, int _v2, int _weight) : v1(_v1), v2(_v2), weight(_weight) {}
};

class Graph {
public:
    int vertices, edges;
    vector<Edge> edge_info;

    Graph(std::ifstream &fs) {
        fs >> vertices >> edges;
        for (int line = 0; line < edges; ++line) {
            int v1, v2, w;
            fs >> v1 >> v2 >> w;
            Edge e(v1 - 1, v2 - 1, w);
            edge_info.push_back(e);
        }
    }

    int shortest_path(int source) {
        // Step 1: initialize graph.
        vector<int> distance(vertices);
        for (int i = 0; i < vertices; ++i) {
            distance[i] = 10000;
        }
        distance[source] = 0;

        // Step 2: relax edges repeatedly.
        for (int i = 0; i < vertices - 1; ++i) {
            for (Edge e: edge_info) {
                if (distance[e.v1] + e.weight < distance[e.v2]) {
                    distance[e.v2] = distance[e.v1] + e.weight;
                }
            }
        }

        // Step 3: check for negative-weight cycles or check min distance.
        vector<int>::iterator min_pos;
        min_pos = std::min_element(distance.begin(), distance.end());
        // cout << "min element = " << *min_pos << endl;

        for (Edge e: edge_info) {
            if (distance[e.v1] + e.weight < distance[e.v2]) {
                cout << "Negative weight!" << endl;
            }
        }


        return *min_pos;
    }
};


int main() {
    const string filename("g3.txt");
    std::ifstream fs(filename);
    Graph graph(fs);
    vector<int> graph_min_distance;
    for (int s = 0; s < graph.vertices; ++s) {
        cout << "Source " << s << " done." << endl;
        int d = graph.shortest_path(s);
        graph_min_distance.push_back(d);
    }

    vector<int>::iterator min = std::min_element(graph_min_distance.begin(), graph_min_distance.end());
    cout << "min distance = " << *min << endl;

    return 0;
}
