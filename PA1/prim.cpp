#include <iostream>
#include <fstream>
#include <string>
#include <vector>
// #include <unordered_map>
#include <unordered_set>

using std::string;
using std::cout;
using std::endl;

class Graph {
public:
    int vertex_size, edge_size;
    int** adjacency_list;

    void read_file(std::ifstream& fs) {
        cout << "read file: " << endl;

        fs >> vertex_size >> edge_size;

        cout << "vertex_size = " << vertex_size << ", edge_size = " << edge_size << endl;
        cout << "Initialize matrix: " << endl;

        adjacency_list = new int*[vertex_size];
        for (int i = 0; i < vertex_size; ++i) {
            adjacency_list[i] = new int[vertex_size];
        }

        // Initialize adjacency_list;
        for (int i = 0; i < vertex_size; ++i) {
            for (int j = 0; j < vertex_size; ++j) {
                adjacency_list[i][j] = 0;
            }
        }

        cout << "Fill out matrix: " << endl;

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
        int* key = new int[vertex_size];
        int* parent = new int[vertex_size];
        int sum = 0;

        // Initialize key and parent
        for (int i = 0; i < vertex_size; ++i) {
            key[i] = 10000;
            parent[i] = -1;
        }

        key[0] = 0;
        std::unordered_set<int> Q;
        for (int i = 0; i < vertex_size; ++i)
            Q.insert(i);

        int iteration = 0;
        while (!Q.empty()) {
            cout << "Iteration: " << iteration++ << endl;
            // Find the min(Q) by key value
            int u = find_min(Q, key);
            Q.erase(u);
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

        delete [] key, parent;
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

    ~Graph() {
        for (int i = 0; i < vertex_size; ++i)
            delete [] adjacency_list[i];
        delete [] adjacency_list;
    }

private:
    int find_min(const std::unordered_set<int> &Q, const int* key) const {
        int min_id, min = 10000;

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


int main() {
    Graph graph;
    string filename("prim_testcase0.txt");
    std::ifstream readFile(filename);

    graph.read_file(readFile);
    // graph.print();

    cout << "Minimum spanning tree weight = " << graph.PrimMST() << endl;

}
