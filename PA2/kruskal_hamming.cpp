#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
// #include <chrono>

using std::unordered_map;
using std::unordered_set;
using std::vector;
using std::string;
using std::cout;
using std::endl;
// using namespace std::chrono;

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
    Kruskal(std::ifstream &fs) {
        // Step 1: read file.
        fs >> vertex_size >> hamming_bits;
        permutation_size = hamming_bits + hamming_bits * (hamming_bits - 1) / 2;
        // vertex_size = 20000;
        for (int i = 0; i < vertex_size; ++i) {
            char bit;
            string str;
            for (int j = 0; j < hamming_bits; ++j) {
                fs >> bit;
                str += bit;
            }
            map.insert({str, i});
        }

        // Step 2: connect all the neighbors within distance of 2.
        UF union_find(vertex_size);
        int sum = 0;
        for (auto it = map.begin(); it != map.end(); ++it) {
            for (string permuted: permute(it->first)) {
                auto search = map.find(permuted);
                if (search != map.end() && !union_find.find(it->second, search->second)) {
                    union_find.unite(it->second, search->second);
                    sum++;
                }
            }
        }

        // Step 3: count how many clusters there are inside the union find.
        cout << "Answer = " << map.size() - sum << endl;
    }

private:
    int vertex_size, hamming_bits, permutation_size;
    unordered_map<string, int> map;

    string permute(const string &str, const int index) const {
        string permute_str = str;
        permute_str[index] = str[index] == '0' ? '1' : '0';
        return permute_str;
    }

    vector<string> permute(const string &str) const {
        vector<string> str_arr(permutation_size);
        for (int i = 0; i < hamming_bits; ++i) {
            string temp = permute(str, i);
            str_arr.push_back(temp);
            for (int j = i + 1; j < hamming_bits; ++j) {
                str_arr.push_back(permute(temp, j));
            }
        }
        return str_arr;
    }
};

int main() {
    // high_resolution_clock::time_point t1 = high_resolution_clock::now();
    const string filename("clustering_big.txt");
    std::ifstream fs(filename);
    Kruskal kruskal(fs);

    // high_resolution_clock::time_point t2 = high_resolution_clock::now();
    // auto duration = duration_cast<seconds>(t2 - t1).count();
    // cout << "Duration = " << duration << endl;

    return 0;
}
