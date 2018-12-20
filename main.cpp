#include "define.hpp"
#include "data_reader.hpp"
#include <string>
#include <cstdlib>
#include <ctime>

int get_random(int range) {
  return rand()%range;
}

int evaluation(vector<int>& assignment, vector<int>& facility_used, data& m_data) {
  int result;
  for(int i = 0; i < facility_used.size(); i++) {
    if(facility_used[i] != 0)
      result += m_data.facility_opening_cost[i];
  }
  for(int i = 0; i < assignment.size(); i++) {
    result += m_data.assignment_cost[assignment[i]][i];
  }
  return result;
}

void check(vector<int>& assignment, data& m_data) {
  int result = 0;
  vector<int> used(m_data.facility_num, 0);
  vector<int> status(m_data.facility_num, 0);
  for(int i = 0; i < assignment.size(); i++) {
    result += m_data.assignment_cost[assignment[i]][i];
    used[assignment[i]] += m_data.customer_demand[i];
    status[assignment[i]] = 1;
  }
  for(int i = 0; i < status.size(); i++) {
    if(status[i] == 1)
      result += m_data.facility_opening_cost[i];
  }
  for(auto i: used)
    cout << i << " ";
  cout << endl;
  cout << "it should be: " <<  result << endl;
}

void local_search(data& m_data) {
  int result = 0;
  vector<int> facility_status(m_data.facility_num, 0);
  vector<int> assignment(m_data.customer_num, 0);
  vector<int> facility_used(m_data.facility_num, 0);

  // cout << "facility capacity:" << endl;
  // for(auto i: m_data.facility_capacity)
  //   cout << i << " ";
  // cout << endl;

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

  // cout << "init solution:" << endl;
  // for(auto i: facility_used)
  //   cout << i << " ";
  // cout << endl;

  result = evaluation(assignment, facility_used, m_data);
  // cout << "init cost: " << result << endl;

  int last_update = -1;
  for(int i = 0; i < 500; i++) {
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
        if(m_data.facility_capacity[tmp_facility]-facility_used[tmp_facility] >= m_data.customer_demand[tmp_customer])
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
      facility_used[assignment[customer]] -= m_data.customer_demand[customer];
      assignment[customer] = facility;
      facility_used[assignment[customer]] += m_data.customer_demand[customer];
      result += cur;
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
  cout << "final cost: " << result << " last update: " << last_update << endl;
  // cout << "check result..." << endl;
  // check(assignment, m_data);
}

int main () {
//   for(int i = 1; i <= 71; i++) {
//     string file = "Instances/p"+to_string(i);
//     data m_data = get_data(file.c_str());
//     if(i == 1)
//       print(m_data);
//   }
  
  string file = "Instances/p1";
  data m_data = get_data(file.c_str());
  srand(time(0));
  for(int i = 0; i < 5; i++) {
    local_search(m_data);
  }
  return 0;
}