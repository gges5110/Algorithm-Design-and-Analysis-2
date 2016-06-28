#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using std::vector;
using std::cout;
using std::endl;

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

class UF {
public:
    int *id, *sz;
    UF(int n) {
        id = new int[n];
        sz = new int[n];

        for (int i = 0; i < n; ++i) {
            id[i] = i;
            sz[i] = 1;
        }
    }

    int root(int i) {
        while (i != id[i]) {
            id[i] = id[id[i]];
            i = id[i];
        }
        return i;
    }

    bool find(int p, int q) {
        return root(p) == root(q);
    }

    void unite(int p, int q) {
        int i = root(p);
        int j = root(q);
        if (sz[i] < sz[j]) {
            id[i] = j;
            sz[j] += sz[i];
        }
        else {
            id[j] = i;
            sz[i] += sz[j];
        }
    }

    ~UF() {
        delete [] id, sz;
    }

};


class Kruskal {
public:
    int vertex_size;
    // int** edge_weights;
    vector<Edge> edges;

    Kruskal(std::ifstream& fs) {
        fs >> vertex_size;


        // edge_weights = new int*[vertex_size];
        // for (int i = 0; i < vertex_size; ++i)
        //     edge_weights[i] = new int[vertex_size];
        //
        // for (int i = 0; i < vertex_size; ++i) {
        //     for (int j = 0; j < vertex_size; ++j) {
        //         edge_weights[i][j] = 0;
        //     }
        // }

        int v1, v2, w;
        // for (int i = 0; i < 100; ++i) {
        //     fs >> v1 >> v2 >> w;
        //     Edge e(v1, v2, w);
        //     edges.push_back(e);
        // }


        while (fs >> v1 >> v2 >> w) {
            Edge e(v1, v2, w);
            edges.push_back(e);
            // edge_weights[i - 1][j - 1] = w;
            // edge_weights[j - 1][i - 1] = w;
        }

    }

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

    // ~Kruskal() {
    //     for (int i = 0; i < vertex_size; ++i) {
    //         delete [] edge_weights[i];
    //     }
    //     delete [] edge_weights;
    // }
};

int main() {
    const std::string filename("clustering1.txt");
    std::ifstream fs(filename);
    Kruskal k(fs);
    vector<Edge> T = k.MST(4);
    for (Edge e: T)
        e.print();

}
