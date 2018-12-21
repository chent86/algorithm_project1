#include "local_search.hpp"

int get_random(int range) {
  return rand()%range;
}

int evaluation(vector<int>& assignment, data& m_data) {
  int result = 0; // 要记得初始化
  vector<int> facility_status(m_data.facility_num, 0);
  for(int i = 0; i < assignment.size(); i++) {
    facility_status[assignment[i]] = 1;
  }
  for(int i = 0; i < facility_status.size(); i++) {
    if(facility_status[i] != 0)
      result += m_data.facility_opening_cost[i];
  }
  for(int i = 0; i < assignment.size(); i++) {
    result += m_data.assignment_cost[assignment[i]][i];
  }
  return result;
}

void init_solution(vector<int>& assignment, vector<int>& facility_used, data& m_data) {
  // cout << "generating init solution..." << endl;
  for(int i = 0; i < m_data.customer_num; i++) {
    while(true) {
      int facility = get_random(m_data.facility_num);
      if(m_data.facility_capacity[facility]-facility_used[facility] >= m_data.customer_demand[i]) {
        assignment[i] = facility;
        facility_used[facility] += m_data.customer_demand[i];
        break;
      }
    }
  }
}

void local_search(data& m_data) {
  int result = 0;
  vector<int> facility_status(m_data.facility_num, 0);
  vector<int> assignment(m_data.customer_num, 0);
  vector<int> facility_used(m_data.facility_num, 0);

  init_solution(assignment, facility_used, m_data);
  result = evaluation(assignment, m_data);
  // cout << "init cost: " << result << endl;
  // cout << evaluation(assignment, m_data) << endl;
  int last_update = -1;
  int times = 2000;
  for(int i = 0; i < times; i++) {
    int cur = 0;
    int customer;
    int facility;
    for(int j = 0; j < 10; j++) {
      // 修改一个顾客的安排得到一个邻域，同时找10个邻域，从中选出最好的
      int tmp_customer, tmp_facility;
      while(true) {
        // 得到一个可执行的调整（不超过最大容量）
        tmp_customer = get_random(m_data.customer_num);
        tmp_facility = get_random(m_data.facility_num);
        if(m_data.facility_capacity[tmp_facility]-facility_used[tmp_facility] >= m_data.customer_demand[tmp_customer] 
        && tmp_facility != assignment[tmp_customer]) // 要避开调度到同一个工厂的情况(相当于没调动，而且会出bug)
          break;
      }
      int tmp_result = m_data.assignment_cost[tmp_facility][tmp_customer]-m_data.assignment_cost[assignment[tmp_customer]][tmp_customer];
      if(facility_used[tmp_facility] == 0) {
        // 变动需要开启新的工厂
        tmp_result += m_data.facility_opening_cost[tmp_facility];
      } 
      if(facility_used[assignment[tmp_customer]] == m_data.customer_demand[tmp_customer]) {
        // 变动会关闭旧的工厂
        tmp_result -= m_data.facility_opening_cost[assignment[tmp_customer]];
      }
      if(tmp_result < cur) {
        cur = tmp_result;
        customer = tmp_customer;
        facility = tmp_facility;
      }
    }
    if(cur < 0) {
      // cout << customer << " origin: " << assignment[customer] << " new: " << facility << endl;
      facility_used[assignment[customer]] -= m_data.customer_demand[customer];
      assignment[customer] = facility;
      facility_used[assignment[customer]] += m_data.customer_demand[customer];
      result += cur;
      // for(auto i: assignment)
      //   cout << i << " ";
      // cout << endl;      
      // cout << result << " vs " << evaluation(assignment, m_data) << endl;
      // if(result != evaluation(assignment, m_data)) {
      //   return;
      // }
      last_update = i;
      // cout << i << " : " << result << endl;
    }
  }
  // cout << "final solution: ";
  // for(auto i: facility_used)
  //   cout << i << " ";
  // cout << endl;
  // for(auto i: assignment)
  //   cout << i << " ";
  // cout << endl;
  cout << "HC final cost: " << result << " total times: " << times << " last update: " << last_update << " check: " << evaluation(assignment, m_data) << endl;
  // check(m_data, assignment);
  // cout << "check result..." << endl;
  // check(assignment, m_data);
}