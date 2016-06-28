#include <iostream>
#include <fstream>
#include <string>

using std::string;

class Kruskal {
public:
    int vertex_size;

    Kruskal(std::ifstream &fs) {
        fs >> vertex_size;
        for (int i = 0; i < vertex_size; ++i) {
            int bit[24];
            for (int j = 0; j < 24; ++j) {
                fs >> bit[j];
            }

        }

    }

};

int main() {
    const string filename("clustering_big.txt");
    std::ifstream fs(filename);
    Kruskal kruskal(fs);

}
