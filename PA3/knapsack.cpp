#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using std::string;
using std::ifstream;

class Knapsack {
public:
    int *v, *w;
    int knapsack_size, number_of_items;
    Knapsack(ifstream &fs) {
        fs >> knapsack_size >> number_of_items;

        // Memory allocation.
        v = new int[number_of_items];
        w = new int[number_of_items];

        int *prev, *curr;
        prev = new int[knapsack_size];
        curr = new int[knapsack_size];
        for (int j = 0; j < knapsack_size; ++j) {
            prev[j] = 0;
        }

        // Read file.
        for (int i = 0; i < number_of_items; ++i) {
            fs >> v[i] >> w[i];
        }

        for (int i = 1; i < number_of_items; ++i) {
            for (int j = 0; j < knapsack_size; ++j) {
                if (j - w[i] >= 0) {
                    curr[j] = std::max(prev[j], prev[j - w[i]] + v[i]);
                }
                else {
                    curr[j] = prev[j];
                }
            }

            // Copy curr to prev.
            for (int j = 0; j < knapsack_size; ++j) {
                prev[j] = curr[j];
            }
        }

        std::cout << "Answer = " << curr[knapsack_size - 1] << std::endl;

        delete [] prev, curr;
    }

    ~Knapsack() {
        delete [] v, w;
    }
};

int main() {
    const string filename("knapsack_big.txt");
    ifstream fs(filename);
    Knapsack knapsack(fs);
    return 0;
}
