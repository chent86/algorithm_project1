#include "data_reader.hpp"

data get_data(const char* path) {
  std::ifstream file(path);       //此处的字符串为文件的路径
  string my_read;
  char * token;
  char tmp[100];
  int facility_num, customer_num;

  // 获取工厂数量以及顾客数量
  getline(file, my_read);
  strcpy(tmp, my_read.c_str());
  token = strtok(tmp," .");
  facility_num = atoi(token);
  token = strtok (NULL, "  .");
  customer_num = atoi(token);
  vector<vector<int>> init_assignment_cost(facility_num, vector<int>(customer_num, 0));
  data m_data(facility_num, customer_num, init_assignment_cost);

  // 读取各个工厂的容量和运营开销
  for(int i = 0; i < facility_num; i++) {
    getline(file, my_read);
    strcpy(tmp, my_read.c_str());
    token = strtok(tmp," .");
    m_data.facility_capacity.push_back(atoi(token));
    token = strtok(NULL," .");
    m_data.facility_opening_cost.push_back(atoi(token));
  }

  int data_column_num = 10; // 提供的数据是每行10列

  // 读取顾客需求
  for(int i = 0; i < customer_num/data_column_num; i++) {
    getline(file, my_read);
    strcpy(tmp, my_read.c_str());
    token = strtok(tmp," .");
    m_data.customer_demand.push_back(atoi(token));
    for(int j = 0; j < data_column_num-1; j++) {
      token = strtok(NULL," .");
      m_data.customer_demand.push_back(atoi(token));
    }
  }

  // 读取顾客分配的开销
  for(int i = 0; i < facility_num; i++)
  for(int j = 0; j < customer_num/data_column_num; j++) {
    getline(file, my_read);
    strcpy(tmp, my_read.c_str());
    token = strtok(tmp," .");
    m_data.assignment_cost[i][j*data_column_num] = atoi(token);
    for(int k = 0; k < data_column_num-1; k++) {
      token = strtok(NULL," .");
      m_data.assignment_cost[i][j*data_column_num+k+1] = atoi(token);
    } 
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