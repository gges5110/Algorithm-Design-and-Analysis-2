#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm> // std::max

using std::vector;

class Knapsack {
private:
  vector<int> _v, _w;
  int _knapsack_size, _number_of_items;
public:
  Knapsack(int knapsack_size, int number_of_items, const vector<int> &v, const vector<int> &w) :
  _knapsack_size(knapsack_size),
  _number_of_items(number_of_items),
  _v(v), _w(w) {}

  int compute() const {
    vector<int> prev(_knapsack_size, 0), curr(_knapsack_size, 0);
    for (int i = 1; i < _number_of_items; ++i) {
      for (int j = 0; j < _knapsack_size; ++j) {
        if (j - _w[i] >= 0) {
          curr[j] = std::max(prev[j], prev[j - _w[i]] + _v[i]);
        } else {
          curr[j] = prev[j];
        }
      }

      // Copy curr to prev.
      for (int j = 0; j < _knapsack_size; ++j) {
        prev[j] = curr[j];
      }
    }

    return curr[_knapsack_size - 1];
  }
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Please provide a valid filename for testing." << std::endl;
  } else {
    std::ifstream fs;
    fs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
      fs.open(argv[1]);

      // Original main
      int knapsack_size, number_of_items;
      fs >> knapsack_size >> number_of_items;
      vector<int> v(number_of_items), w(number_of_items);
      for (int i = 0; i < number_of_items; ++i) {
          fs >> v[i] >> w[i];
      }
      Knapsack knapsack(knapsack_size, number_of_items, v, w);
      std::cout << knapsack.compute() << std::endl;
    } catch (std::ifstream::failure& e) {
      std::cerr << "Exception opening/reading file" << std::endl;
      std::cout << e.what() << std::endl;
    }
    fs.close();
  }
  return 0;
}
