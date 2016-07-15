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
        if (vertex != that.vertex) {
            return false;
        }
        else if (set != that.set) {
            return false;
        }
        else {
            return true;
        }
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
        return (31 * key.vertex);
    }
};


class Cost_parent {
public:
    int cost, parent;
    Cost_parent(int cost, int parent): cost(cost), parent(parent) {}
    Cost_parent() {
        // cost = -1;
        // parent = -1;
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

    void test() {
        distance_matrix = new double*[node_size];
        for (int i = 0; i < node_size; ++i) {
            distance_matrix[i] = new double[node_size];
        }

        distance_matrix[0][0] = 0;
        distance_matrix[0][1] = 1;
        distance_matrix[0][2] = 15;
        distance_matrix[0][3] = 6;
        distance_matrix[1][0] = 2;
        distance_matrix[1][1] = 0;
        distance_matrix[1][2] = 7;
        distance_matrix[1][3] = 3;
        distance_matrix[2][0] = 9;
        distance_matrix[2][1] = 6;
        distance_matrix[2][2] = 0;
        distance_matrix[2][3] = 12;
        distance_matrix[3][0] = 10;
        distance_matrix[3][1] = 4;
        distance_matrix[3][2] = 8;
        distance_matrix[3][3] = 0;
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

    }

    vector<vector<int>> combinations(int k) {
        vector<int> combinations;
        vector<vector<int>> answer;
        go(1, k, combinations, answer);
        return answer;
    }

    void go(int offset, int k, vector<int> combinations, vector<vector<int>> &answer) {
        if (k == 0) {
            vector<int> temp = combinations;
            answer.push_back(temp);
            return;
        }
        for (int i = offset; i < node_size; ++i) {
             combinations.push_back(i);
             go(i + 1, k - 1, combinations, answer);
             combinations.pop_back();
        }
    }

    TSP() {
        node_size = 4;
        nodes = new Coordinate<double>[node_size];
        test();
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

        vector<int> complimentary_set;
        for (int set_size = 1; set_size < node_size - 1; ++set_size) {
            vector<vector<int>> S = combinations(set_size);
            for (vector<int> set: S) {
                complimentary_set = complimentary(set, node_size);

                for (int k: complimentary_set) {
                    int min = 10000;
                    Set_vertex min_setVertex;
                    Cost_parent min_costParent(min, -1);

                    cout << "Calculating node " << k << endl;// << " with set ";
                    for (int m: set) {
                        // cout << m << ", ";

                        unordered_set<int> temp_set = Vector_to_set(set);

                        // cout <<  "Erasing " << k << endl;
                        temp_set.erase(m);
                        Set_vertex temp_setVertex(m, temp_set);

                        cout << "vertex = " << temp_setVertex.vertex << ", set = ";
                        for (int i: temp_setVertex.set) {
                            cout << i << ", ";
                        }
                        cout << endl;

                        auto search = map.find(temp_setVertex);
                        // cout << "Map size = " << map.size() << endl;
                        if (search != map.end()) {
                            // cout << "found" << endl;
                            Cost_parent temp_costParent = map[temp_setVertex];
                            cout << "Cost = " << temp_costParent.cost + distance_matrix[m][k] << endl;

                            if (temp_costParent.cost + distance_matrix[m][k] < min) {
                                // cout << "inside" << endl;
                                min = temp_costParent.cost + distance_matrix[m][k];
                                min_setVertex = temp_setVertex;
                                min_costParent = temp_costParent;
                            }
                        }
                        else {
                            cout << "not found" << endl;
                        }

                    }
                    // cout << "Min cost = " << min_costParent.cost << ", parent = " << min_costParent.parent << endl;
                    // cout << "Min vertex = " << min_setVertex.vertex << endl;

                    // cout << endl;
                    Set_vertex now(k, Vector_to_set(set));
                    min_costParent.cost += min;
                    map.insert({now, min_costParent});
                }
            }
        }
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
    TSP tsp;
    vector<vector<int>> answer = tsp.combinations(2);
    for (vector<int> set: answer) {
        for (int i: set) {
            cout << i << ", ";
        }
        cout << endl;
    }
    // vector<int> test_v;
    // test_v.push_back(1);
    // test_v.push_back(2);
    // test_v.push_back(3);
    // test_v.push_back(4);
    //
    // for (int i: test_v) {
    //     cout << i << ", ";
    // }
    // cout << endl;
    //
    // while(next_permutation(test_v.begin(), test_v.end())) {
    //     for (int i: test_v) {
    //         cout << i << ", ";
    //     }
    //     cout << endl;
    // }


}
