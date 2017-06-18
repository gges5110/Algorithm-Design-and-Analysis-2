#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include "UnionFind.h"
// #include <chrono>

using std::unordered_map;
using std::unordered_set;
using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::ifstream;
// using namespace std::chrono;

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

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Please provide a valid filename for testing." << std::endl;
  } else {
    ifstream fs;
    fs.exceptions(ifstream::failbit | ifstream::badbit);
    try {
      fs.open(argv[1]);

      // Original main
      Kruskal kruskal(fs);
    } catch (ifstream::failure& e) {
      std::cerr << "Exception opening/reading file" << std::endl;
      cout << e.what() << endl;
    }
    fs.close();
  }
  return 0;
}
