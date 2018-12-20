#ifndef DEFINE_HPP
#define DEFINE_HPP

#include <vector>
using namespace std;
struct data {
  int facility_num;
  int customer_num;
  vector<int> facility_capacity;
  vector<int> facility_opening_cost;
  vector<int> customer_demand;
  vector<vector<int>> assignment_cost; // [facility][customer]
  data(int facility_num, int customer_num, vector<vector<int>> init_assignment_cost) {
    this->facility_num = facility_num;
    this->customer_num = customer_num;
    this->assignment_cost = init_assignment_cost;
  }
};

#endif