#ifndef DEFINE_HPP
#define DEFINE_HPP

#include <vector>
using namespace std;
struct data {
  int facility_num;  // 商店数量
  int customer_num;  // 顾客数量
  vector<int> facility_capacity;  // 商店容量
  vector<int> facility_opening_cost; // 商店开放开销
  vector<int> customer_demand;    // 顾客占的空间
  vector<vector<int>> assignment_cost; // [facility][customer] 安排顾客的开销
  data(int facility_num, int customer_num, vector<vector<int>> init_assignment_cost) {
    this->facility_num = facility_num;
    this->customer_num = customer_num;
    this->assignment_cost = init_assignment_cost;
  }
  vector<int> assignment; // 最终的安排方式
  vector<int> facility_used; // 最终各个工厂消耗的容量
};

#endif