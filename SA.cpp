#include "SA.hpp"

void SA(data& m_data) {
  int result = 0;
  float T = 10000;
  float min_T = 0.00001;
  float factor = 0.9999; 
  vector<int> facility_status(m_data.facility_num, 0);
  vector<int> assignment(m_data.customer_num, 0);
  vector<int> facility_used(m_data.facility_num, 0);

  init_solution(assignment, facility_used, m_data);
  result = evaluation(assignment, m_data);
  // cout << "init cost: " << result << endl;

  float last_update = -1;
  int i = 1;
  while(T > min_T) {
    int customer;
    int facility;
    // 随机挑选一个顾客，并给他一个随机的位移，求出系统因此而产生的能量变化
    while(true) {
      // 得到一个可执行的调整（不超过最大容量）
      customer = get_random(m_data.customer_num);
      facility = get_random(m_data.facility_num);
      if(m_data.facility_capacity[facility]-facility_used[facility] >= m_data.customer_demand[customer]
      && facility != assignment[customer]) // 要避开调度到同一个工厂的情况(相当于没调动，而且会出bug)
        break;
    }
    int E = m_data.assignment_cost[facility][customer]-m_data.assignment_cost[assignment[customer]][customer];
    if(facility_used[facility] == 0) {
      // 变动需要开启新的工厂
      E += m_data.facility_opening_cost[facility];
    } 
    if(facility_used[assignment[customer]] == m_data.customer_demand[customer]) {
      // 变动会关闭旧的工厂
      E -= m_data.facility_opening_cost[assignment[customer]];
    }
    if(E <= 0) {
      facility_used[assignment[customer]] -= m_data.customer_demand[customer];
      assignment[customer] = facility;
      facility_used[assignment[customer]] += m_data.customer_demand[customer];
      result += E;
      last_update = i;
      // cout << i << " : " << result << endl;
    } else {
      float R = get_random(10000)/(float)10000;
      float P = exp(-E/T);
      if(R < P) {
        facility_used[assignment[customer]] -= m_data.customer_demand[customer];
        assignment[customer] = facility;
        facility_used[assignment[customer]] += m_data.customer_demand[customer];
        result += E;
        last_update = i;        
      }
    }
    T *= factor;
    i++;
  }
  cout << "SA final cost: " << result << " total times: " << i << " last update: " << last_update << " check: " << evaluation(assignment, m_data) << endl;
  // cout << "check result..." << endl;
  // check(assignment, m_data);
}