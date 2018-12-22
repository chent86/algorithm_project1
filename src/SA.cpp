#include "SA.hpp"

int SA(data& m_data) {

  int final_result = 100000;
  int final_times;
  int final_last_update;

  for(int x = 0; x < 5; x++) {
    int result = 0;
    float T = 10000;        // 初温
    float min_T = 0.00001;  // 末温
    float factor = 0.9999;  // 降温系数
    vector<int> facility_status(m_data.facility_num, 0);
    vector<int> assignment(m_data.customer_num, 0);
    vector<int> facility_used(m_data.facility_num, 0);
    vector<int> m_random;

    if(!init_solution(assignment, facility_used, m_data, m_random)) {
      x--;
      continue;
    } 
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
        // 不选取关闭的商店
        // 第一个初始解开放所有的商店
        if(facility_used[facility] == 0 && !can_open(facility, m_random) && x != 0)
          continue;        
        if(m_data.facility_capacity[facility]-facility_used[facility] >= m_data.customer_demand[customer]
        && facility != assignment[customer]) // 要避开调度到同一个商店的情况(相当于没调动，而且会出bug)
          break;
      }
      int E = m_data.assignment_cost[facility][customer]-m_data.assignment_cost[assignment[customer]][customer];
      if(facility_used[facility] == 0) {
        // 当前调整如果需要开启新的商店
        E += m_data.facility_opening_cost[facility];
      } 
      if(facility_used[assignment[customer]] == m_data.customer_demand[customer]) {
        // 当前调整如果会关闭旧的商店
        E -= m_data.facility_opening_cost[assignment[customer]];
      }
      if(E <= 0) { // 如果是优的解，接受
        facility_used[assignment[customer]] -= m_data.customer_demand[customer];
        assignment[customer] = facility;
        facility_used[assignment[customer]] += m_data.customer_demand[customer];
        result += E;
        last_update = i;
        // cout << i << " : " << result << endl;
      } else {  // 如果是差的解，以一定概率接受
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
      T *= factor; // 降温
      i++;
    }
    if(result < final_result) {
      final_result = result;
      final_times = i;
      final_last_update = last_update;
      m_data.assignment = assignment;
      m_data.facility_used = facility_used;
    }
  }
  cout << "SA final cost: " << final_result << " total times: " << final_times << " last update: " << final_last_update << " check: " << evaluation(m_data.assignment, m_data) << endl;
  return final_result;
  // cout << "check result..." << endl;
  // check(assignment, m_data);
}