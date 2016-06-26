#include <iostream>
#include <fstream>
#include <string>
#include <vector>
// #include <unordered_map>
// #include <unordered_set>

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
            cout << v1 << " -> " << v2 << ": " << w << endl;
            adjacency_list[v1 - 1][v2 - 1] = w;
            adjacency_list[v2 - 1][v1 - 1] = w;
        }
    }

    int PrimMST() {
        int* C = new int[vertex_size];
        int* E = new int[vertex_size];



        delete [] C, E;
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
};


int main() {
    Graph graph;
    string filename("prim_testcase1.txt");
    std::ifstream readFile(filename);

    graph.read_file(readFile);
    graph.print();

}
