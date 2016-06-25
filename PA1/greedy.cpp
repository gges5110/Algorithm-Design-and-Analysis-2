#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string>

using std::vector;
using std::string;

class Job {

public:
    int weight, length;
    Job(int w, int l) : weight(w), length(l) {};

    double score() const {
        return (double) weight - length;
    }

    bool operator < (const Job& that) const {
        if (this->score() != that.score()) {
            return this->score() > that.score();
        }
        else {
            return this->weight > that.weight;
        }
    }

    string print() {
        return std::to_string(weight) + ", " + std::to_string(length) + ", score = " + std::to_string(this->score());
    }
};

unsigned long int total_weight(vector<Job> jobs) {
    unsigned long int sum = 0, length = 0;
    for (Job j: jobs) {
        length += j.length;
        sum += length * j.weight;
    }
    return sum;
}

vector<Job> ifstream_lines(std::ifstream& fs) {
    int number_of_jobs; fs >> number_of_jobs;
    int weight, length;
    vector<Job> out;

    for (int i = 0; i < number_of_jobs; i++) {
        fs >> weight >> length;
        Job new_job(weight, length);
        out.push_back(new_job);
    }

    return out;
}

bool sort_check(vector<Job> j) {

    int size = j.size();
    for (int i = 0; i < size - 1; i++) {
        if (j[i].score() < j[i + 1].score()) {
            return false;
        }
        else if (j[i].score() == j[i + 1].score()) {
            if (j[i].weight < j[i + 1].weight) {
                return false;
            }
        }
    }

    return true;
}


int main() {
    string filename("testcase0.txt");
    std::ifstream readFile(filename);
    std::cout<<"greedy read file"<<std::endl;

    vector<Job> jobs = ifstream_lines(readFile);
    std::sort(jobs.begin(), jobs.end());
    // for (int i:weights) {
    //     std::cout << i << std::endl;
    // }

    for (Job j: jobs) {
        std::cout << j.print() << std::endl;
    }

    std::cout << "Sort check: " << sort_check(jobs) << std::endl;
    std::cout << "Size check: " << jobs.size() << std::endl;

    unsigned long int sum = total_weight(jobs);
    std::cout << "total weight = " << sum << std::endl;

    return 0;
}
