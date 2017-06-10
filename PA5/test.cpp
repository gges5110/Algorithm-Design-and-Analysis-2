#include <iostream>
#include <vector>
#include <map>

using namespace std;

int main() {
    vector<int> v1, v2;
    v1[0] = 1;
    v1[1] = 2;

    v2[0] = 2;
    v2[1] = 1;

    map<vector<int>, int> map;
    map.insert({v1, 1});
    map.insert({v2, 2});

    cout << map[v1] << endl;

    return 0;
}
