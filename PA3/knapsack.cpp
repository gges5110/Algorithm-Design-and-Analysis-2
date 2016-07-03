#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using std::string;
using std::ifstream;

class Knapsack {
public:
    int *v, *w;
    int **A;
    int knapsack_size, number_of_items;
    Knapsack(ifstream &fs) {
        fs >> knapsack_size >> number_of_items;

        // Memory allocation.
        v = new int[number_of_items];
        w = new int[number_of_items];
        A = new int*[number_of_items];
        for (int i = 0; i < number_of_items; ++i)
            A[i] = new int[knapsack_size];

        for (int j = 0; j < knapsack_size; ++j) {
            A[0][j] = 0;
        }

        // Read file.
        for (int i = 0; i < number_of_items; ++i) {
            fs >> v[i] >> w[i];
        }

        for (int i = 1; i < number_of_items; ++i) {
            for (int j = 0; j < knapsack_size; ++j) {
                if (j - w[i] >= 0)
                    A[i][j] = std::max(A[i - 1][j], A[i - 1][j - w[i]] + v[i]);
                else
                    A[i][j] = A[i - 1][j];
            }
        }

        std::cout << "Answer = " << A[number_of_items - 1][knapsack_size - 1] << std::endl;
    }

    ~Knapsack() {
        for (int i = 0; i < number_of_items; ++i)
            delete [] A[i];
        delete [] A;
        delete [] v, w;
    }
};




int main() {
    const string filename("knapsack_big.txt");
    ifstream fs(filename);
    Knapsack knapsack(fs);
    return 0;
}
