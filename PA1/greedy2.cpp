/*
In this programming problem and the next you'll code up the greedy algorithms from lecture for minimizing the weighted sum of completion times..

Download the text file below.

jobs.txt
This file describes a set of jobs with positive and integral weights and lengths. It has the format

[number_of_jobs]

[job_1_weight] [job_1_length]

[job_2_weight] [job_2_length]

...

For example, the third line of the file is "74 59", indicating that the second job has weight 74 and length 59.

You should NOT assume that edge weights or lengths are distinct.

Your task in this problem is to run the greedy algorithm that schedules jobs in decreasing order of the difference (weight - length). Recall from lecture that this algorithm is not always optimal. IMPORTANT: if two jobs have equal difference (weight - length), you should schedule the job with higher weight first. Beware: if you break ties in a different way, you are likely to get the wrong answer. You should report the sum of weighted completion times of the resulting schedule --- a positive integer --- in the box below.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string>

using std::vector;
using std::ifstream;
using std::string;

class Job {

public:
  int weight, length;
  Job(int w, int l) : weight(w), length(l) {};

  double score() const {
    return (double) weight / length;
  }

  bool operator < (const Job& that) const {
    if (this->score() != that.score()) {
      return this->score() > that.score();
    }
    else {
      return this->weight > that.weight;
    }
  }

  string print() const {
    return std::to_string(weight) + ", " + std::to_string(length) + ", score = " + std::to_string(this->score());
  }
};

unsigned long int total_weight(const vector<Job> &jobs) {
  unsigned long int sum = 0, length = 0;
  for (Job j: jobs) {
    length += j.length;
    sum += length * j.weight;
  }
  return sum;
}

vector<Job> ifstream_lines(ifstream& fs) {
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

bool sort_check(const vector<Job> &j) {
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

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Please provide a valid filename for testing." << std::endl;
  } else {
    ifstream fs;
    fs.exceptions(ifstream::failbit | ifstream::badbit);
    try {
      fs.open(argv[1]);

      // Original main
      std::cout<<"greedy read file"<<std::endl;

      vector<Job> jobs = ifstream_lines(fs);
      std::sort(jobs.begin(), jobs.end());

      // for (Job j: jobs) {
      //   std::cout << j.print() << std::endl;
      // }

      std::cout << "Sort check: " << sort_check(jobs) << std::endl;
      std::cout << "Size check: " << jobs.size() << std::endl;

      unsigned long int sum = total_weight(jobs);
      std::cout << "total weight = " << sum << std::endl;
    } catch (ifstream::failure& e) {
      std::cerr << "Exception opening/reading file" << std::endl;
    }
    fs.close();
  }
  return 0;
}
