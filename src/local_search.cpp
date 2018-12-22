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
bool init_solution(vector<int>& assignment, vector<int>& facility_used, data& m_data, vector<int>& m_random) {
  // 随机选择一个商店，顺序安排顾客，容量满了之后再随机选择另一个商店，重复这个过程
  // 初始解中未开放的商店意味着永远关闭，后续不会安排顾客过去
  int p = get_random(m_data.facility_num);
  m_random.push_back(p);
  for(int i = 0; i < m_data.customer_num; i++) {
    while(true) {
      int facility = p;
      if(m_data.facility_capacity[facility]-facility_used[facility] >= m_data.customer_demand[i]) {
        assignment[i] = facility;
        facility_used[facility] += m_data.customer_demand[i];
        break;
      } else {
        while(true) {
          p = get_random(m_data.facility_num);
          if(find(m_random.begin(), m_random.end(), p) == m_random.end()) {
            m_random.push_back(p);
            break;
          }
          if(m_random.size() == m_data.facility_num)
            return false; // 当前的策略导致无法安排完所有顾客，抛弃此次初始解
        }
      }
    }
  }
  return true;
}

bool can_open(int facility, vector<int>& m_random) {
  // 初始时开放的商店才能继续使用，其余的商店不再开放
  if(find(m_random.begin(), m_random.end(), facility) != m_random.end())
    return true;
  else
    return false;
}

// 爬山法
int local_search(data& m_data) {
  int final_result = 100000;
  int final_times;
  int final_last_update;
  for(int x = 0; x < 10; x++) {
    int result = 0;
    vector<int> facility_status(m_data.facility_num, 0);
    vector<int> assignment(m_data.customer_num, 0);
    vector<int> facility_used(m_data.facility_num, 0);
    vector<int> m_random;
    // 获得一个初始解
    // 如果求解异常，跳过这次执行
    if(!init_solution(assignment, facility_used, m_data, m_random)) {
      x--;
      continue;
    } 
    result = evaluation(assignment, m_data);
    int last_update = -1;
    int times = 500;
    for(int i = 0; i < times; i++) {
      int cur = 0;
      int customer1, customer2;
      int facility1, facility2;
      int type; // 确定邻域中的最优解是用哪种策略找到的
      for(int j = 0; j < 10; j++) {
        // int choice = get_random(100);
        // 修改一个顾客的安排得到一个邻域，同时找10个邻域，从中选出最好的
        int tmp_customer, tmp_facility;
        while(true) {
          // 得到一个可执行的调整（不超过最大容量）
          tmp_customer = get_random(m_data.customer_num);
          tmp_facility = get_random(m_data.facility_num);
          // 不选取关闭的商店
          // 第一个初始解开放所有的商店
          if(facility_used[tmp_facility] == 0 && !can_open(tmp_facility, m_random) && x != 0)
            continue;
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
          customer1 = tmp_customer;
          facility1 = tmp_facility;
        }
      }
      // 如果邻域中有更优的解，将这个解作为新解
      if(cur < 0) {
          facility_used[assignment[customer1]] -= m_data.customer_demand[customer1];
          assignment[customer1] = facility1;
          facility_used[facility1] += m_data.customer_demand[customer1];
          result += cur;
          last_update = i;
      }
      if(i == times-1) {
        if(i-last_update < 500) // 如果迭代一段时间仍然得不到更优的解，则认为已经得到了局部最优解，停止迭代
          times += 500;
      }
    }
    if(result < final_result) {
      final_result = result;
      final_times = times;
      final_last_update = last_update;
      m_data.assignment = assignment;
      m_data.facility_used = facility_used;
    }
  }
  cout << "HC final cost: " << final_result << " total times: " << final_times << " last update: " << final_last_update << " check: " << evaluation(m_data.assignment, m_data) << endl;
  return final_result;
}