#include "data_reader.hpp"

data get_data(const char* path) {
  std::ifstream file(path);       //此处的字符串为文件的路径
  float my_read; // 提供的输入里有'.',所以用float来处理
  int facility_num, customer_num;

  // 获取工厂数量以及顾客数量
  file >> my_read;
  facility_num = int(my_read);
  file >> my_read;
  customer_num = int(my_read);
  vector<vector<int>> init_assignment_cost(facility_num, vector<int>(customer_num, 0));
  data m_data(facility_num, customer_num, init_assignment_cost);

  // 读取各个工厂的容量和运营开销
  for(int i = 0; i < facility_num; i++) {
    file >> my_read;
    m_data.facility_capacity.push_back(int(my_read));
    file >> my_read;
    m_data.facility_opening_cost.push_back(int(my_read));
  }

  // 读取顾客需求
  for(int i = 0; i < customer_num; i++) {
    file >> my_read;
    m_data.customer_demand.push_back(int(my_read));
  }

  // 读取顾客分配的开销
  for(int i = 0; i < facility_num; i++)
  for(int j = 0; j < customer_num; j++) {
    file >> my_read;
    m_data.assignment_cost[i][j] = int(my_read);
  }
  file.close();
  return m_data;
}

void print(data& m_data) {
  cout << m_data.facility_num << " " << m_data.customer_num << endl;
  for(int i = 0; i < m_data.facility_num; i++) {
    cout << m_data.facility_capacity[i] << " " << m_data.facility_opening_cost[i] << endl;
  }

  for(auto i:m_data.customer_demand)
    cout << i << " ";
  cout << endl;

  for(int i = 0; i < m_data.assignment_cost.size(); i++) {
    for(int j = 0; j < m_data.assignment_cost[0].size(); j++) {
      cout << m_data.assignment_cost[i][j] << " ";
    }
    cout << endl;
  }
}