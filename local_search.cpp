#include "local_search.hpp"

int get_random(int range) {
  return rand()%range;
}

// 根据顾客的安排计算总开销
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

// 随机得到一个初始的合法解
void init_solution(vector<int>& assignment, vector<int>& facility_used, data& m_data) {
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

// 爬山法
int local_search(data& m_data) {
  int result = 0;
  vector<int> facility_status(m_data.facility_num, 0);
  vector<int> assignment(m_data.customer_num, 0);
  vector<int> facility_used(m_data.facility_num, 0);

  init_solution(assignment, facility_used, m_data); // 获得一个初始解
  result = evaluation(assignment, m_data);
  int last_update = -1;
  int times = 500;
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
        && tmp_facility != assignment[tmp_customer]) // 要避开调度到同一个商店的情况(相当于没调动，而且会出bug)
          break;
      }
      int tmp_result = m_data.assignment_cost[tmp_facility][tmp_customer]-m_data.assignment_cost[assignment[tmp_customer]][tmp_customer];
      if(facility_used[tmp_facility] == 0) {
        // 变动需要开启新的商店
        tmp_result += m_data.facility_opening_cost[tmp_facility];
      } 
      if(facility_used[assignment[tmp_customer]] == m_data.customer_demand[tmp_customer]) {
        // 变动会关闭旧的商店
        tmp_result -= m_data.facility_opening_cost[assignment[tmp_customer]];
      }
      // 求出10个邻域中的最优解
      if(tmp_result < cur) { 
        cur = tmp_result;
        customer = tmp_customer;
        facility = tmp_facility;
      }
    }
    // 如果邻域中有更优的解，将这个解作为新解
    if(cur < 0) {
      // cout << customer << " origin: " << assignment[customer] << " new: " << facility << endl;
      facility_used[assignment[customer]] -= m_data.customer_demand[customer];
      assignment[customer] = facility;
      facility_used[assignment[customer]] += m_data.customer_demand[customer];
      result += cur;
      last_update = i;
    }
    if(i == times-1) {
      if(i-last_update < 500) // 如果迭代一段时间仍然得不到更优的解，则认为已经得到了局部最优解，停止迭代
        times += 500;
    }
  }
  m_data.assignment = assignment;
  m_data.facility_used = facility_used;
  cout << "HC final cost: " << result << " total times: " << times << " last update: " << last_update << " check: " << evaluation(assignment, m_data) << endl;
  return result;
}