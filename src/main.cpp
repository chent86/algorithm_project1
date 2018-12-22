#include "define.hpp"
#include "data_reader.hpp"
#include "local_search.hpp"
#include "SA.hpp"
#include "math.h"
#include <string>
#include <cstdlib>
#include <ctime>

int main () {
  srand(time(0));

  std::ofstream HC_result("output/HC_result");
  std::ofstream HC_detail("output/HC_detail");
  std::ofstream SA_result("output/SA_result");
  std::ofstream SA_detail("output/SA_detail");
  clock_t startTime,endTime;
  for(int i = 1; i <= 71; i++) {
    string file = "input/Instances/p"+to_string(i);
    data m_data = get_data(file.c_str());
    startTime = clock();
    int result = local_search(m_data);
    endTime = clock();
    HC_result << "p" << to_string(i) << " " << to_string(result) << " " << to_string((double)(endTime - startTime)/CLOCKS_PER_SEC) << endl;
    HC_detail << "[p" << to_string(i) << "]" << endl;
    HC_detail <<to_string(result) << endl;
    for(int i = 0; i < m_data.facility_num; i++)
      if(m_data.facility_used[i] != 0)
        HC_detail << "1 ";
      else
        HC_detail << "0 ";
    HC_detail << endl;
    for(int i = 0; i < m_data.customer_num; i++)
      HC_detail << m_data.assignment[i] << " ";
    HC_detail << endl;

    startTime = clock();
    result = SA(m_data);
    endTime = clock();
    SA_result << "p" << to_string(i) << " " << to_string(result) << " " << to_string((double)(endTime - startTime)/CLOCKS_PER_SEC) << endl;
    SA_detail << "[p" << to_string(i) << "]" << endl;
    SA_detail <<to_string(result) << endl;
    for(int i = 0; i < m_data.facility_num; i++)
      if(m_data.facility_used[i] != 0)
        SA_detail << "1 ";
      else
        SA_detail << "0 ";
    SA_detail << endl;
    for(int i = 0; i < m_data.customer_num; i++)
      SA_detail << m_data.assignment[i] << " ";
    SA_detail << endl;
  }
  HC_result.close();
  HC_detail.close();
  // SA_result.close();
  // SA_detail.close();

  // string file = "input/Instances/p70";
  // data m_data = get_data(file.c_str());
  // print(m_data);
  // for(int i = 0; i < 5; i++) {
  //   local_search(m_data);
  // }
  // for(int i = 0; i < 5; i++) {
  //   SA(m_data);
  // }
  return 0;
}