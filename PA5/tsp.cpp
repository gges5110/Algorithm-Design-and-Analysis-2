#include <iostream>
#include <fstream>
#include <string>
#include <cmath> /*sqrt, pow*/
#include <algorithm> /* std::next_permutation */
#include <vector>
#include <unordered_set>
#include <unordered_map>

using std::unordered_map;
using std::unordered_set;
using std::sqrt;
using std::pow;
using std::string;
using std::ifstream;
using std::cout;
using std::endl;
using std::next_permutation;
using std::vector;


template <typename T>
class Coordinate {
public:
    T x, y;
    Coordinate(T _x, T _y): x(_x), y(_y) {}
    Coordinate() {}
};

double Eucledian_distance(Coordinate<double> p_1, Coordinate<double> p_2) {
    return sqrt(pow(p_1.x - p_2.x, 2) + pow(p_1.y - p_2.y, 2));
}

class Set_vertex {
public:
    int vertex;
    unordered_set<int> set;
    Set_vertex(int vertex, unordered_set<int> set): vertex(vertex), set(set) {}
    Set_vertex() {}

    bool operator==(const Set_vertex& that) const {
        return (vertex == that.vertex && set == that.set);
    }

    void print() {
        cout << "Print Set_vertex info: " << "vertex = " << vertex << ", set = ";
        for (int i: set) {
            cout << i << ", ";
        }
        cout << endl;
    }
};

class KeyHasher {
public:
    std::size_t operator()(const Set_vertex& key) const {
        using std::size_t;
        using std::hash;

        unordered_set<int>::hasher fn = key.set.hash_function();

        // TODO: need to come up with a better hash function.
        // return (31 * key.vertex + fn(key.set));
        int sum = 0;
        for (int i: key.set) {
            sum += i;
        }
        return (31 * key.vertex + sum);
    }
};


class Cost_parent {
public:
    double cost, parent;
    Cost_parent(double cost, int parent): cost(cost), parent(parent) {}
    Cost_parent() {
        // cost = -1;
        // parent = -1;
    }
    void print() {
        cout << "Print Cost_parent info: cost = " << cost << ", parent = " << parent << endl;
    }
};

class TSP {
private:
    int node_size;
    Coordinate<double>* nodes;
    double** distance_matrix;

    void calculate_distances() {
        distance_matrix = new double*[node_size];
        for (int i = 0; i < node_size; ++i) {
            distance_matrix[i] = new double[node_size];
        }

        for (int i = 0; i < node_size; ++i) {
            distance_matrix[i][i] = 0;
            for (int j = i + 1; j < node_size; ++j) {
                double d = Eucledian_distance(nodes[i], nodes[j]);
                distance_matrix[i][j] = d;
                distance_matrix[j][i] = d;
            }
        }
    }

    vector<int> complimentary(vector<int> set, int size) {
        vector<int> answer;
        for (int i = 1; i < size; ++i) {
            if (std::find(set.begin(), set.end(), i) == set.end()) {
                // i is not inside the set
                answer.push_back(i);
            }
        }
        return answer;
    }

    unordered_set<int> Vector_to_set(const vector<int>& v) const {
        unordered_set<int> temp_set;
        for (int i: v) {
            temp_set.insert(i);
        }
        return temp_set;
    }

    vector<vector<int>> combinations(int k) {
        vector<int> combinations;
        vector<vector<int>> answer;
        permutation(1, k, combinations, answer);
        return answer;
    }

    void permutation(int offset, int k, vector<int> combinations, vector<vector<int>> &answer) {
        if (k == 0) {
            vector<int> temp = combinations;
            answer.push_back(temp);
            return;
        }
        for (int i = offset; i < node_size; ++i) {
             combinations.push_back(i);
             permutation(i + 1, k - 1, combinations, answer);
             combinations.pop_back();
        }
    }

public:
    TSP(ifstream& fs) {
        fs >> node_size;
        nodes = new Coordinate<double>[node_size];
        double x, y;

        for (int line = 0; line < node_size; ++line) {
            fs >> x >> y;
            Coordinate<double> temp(x, y);
            nodes[line] = temp;
        }

        calculate_distances();
        minCost();
    }



    void minCost() {
        unordered_map<Set_vertex, Cost_parent, KeyHasher> map;

        for (int vertex = 1; vertex < node_size; ++vertex) {
            unordered_set<int> temp_set;
            Set_vertex setVertex(vertex, temp_set);
            Cost_parent costParent(distance_matrix[0][vertex], 0);
            map.insert({setVertex, costParent});
        }

        unordered_map<Set_vertex, Cost_parent, KeyHasher> new_map;
        for (int set_size = 1; set_size < node_size - 1; ++set_size) {
            vector<vector<int>> S = combinations(set_size);
            for (vector<int> set: S) {
                for (int k: complimentary(set, node_size)) {
                    Cost_parent min_costParent = getMinCost(k, Vector_to_set(set), map);
                    Set_vertex now(k, Vector_to_set(set));
                    new_map.insert({now, min_costParent});
                }
            }
            map = new_map;
            new_map.clear();
        }

        // [0, {1, 2, 3}]
        unordered_set<int> set;
        for (int i = 1; i < node_size; ++i) {
            set.insert(i);
        }
        Cost_parent min_costParent = getMinCost(0, set, map);
        cout << "Final answer = " << min_costParent.cost << endl;
    }

    Cost_parent getMinCost(const int k, const unordered_set<int> &set, unordered_map<Set_vertex, Cost_parent, KeyHasher> &map) {
        double min = 100000;
        // Set_vertex min_setVertex;
        Cost_parent min_costParent(min, -1);

        // cout << "Calculating node " << k << endl;
        unordered_set<int> temp_set = set;
        for (int m: set) {
            temp_set.erase(m);
            Set_vertex temp_setVertex(m, temp_set);
            Cost_parent temp_costParent = map[temp_setVertex];
            double cost = temp_costParent.cost + distance_matrix[m][k];
            if (cost < min) {
                min = cost;
                // min_setVertex = temp_setVertex;
                min_costParent = temp_costParent;
            }
            temp_set.insert(m);
        }
        min_costParent.cost = min;
        // cout << "Min cost = " << min << endl;
        return min_costParent;
    }

    ~TSP() {
        for (int i = 0; i < node_size; ++i)
            delete [] distance_matrix[i];
        delete [] nodes, distance_matrix;
    }

};


int main() {
    const string filename("tsp.txt");
    ifstream fs(filename);
    TSP tsp(fs);
    // vector<vector<int>> answer = tsp.combinations(2);
    // for (vector<int> set: answer) {
    //     for (int i: set) {
    //         cout << i << ", ";
    //     }
    //     cout << endl;
    // }

}
