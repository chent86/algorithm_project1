#include "define.hpp"
#include "data_reader.hpp"
#include "local_search.hpp"
#include "SA.hpp"
#include "math.h"
#include <string>
#include <cstdlib>
#include <ctime>

int main () {
  for(int i = 1; i <= 71; i++) {
    string file = "Instances/p"+to_string(i);
    data m_data = get_data(file.c_str());
    // if(i == 1)
    //   print(m_data); // 查看数据
    cout << i << " ";
    local_search(m_data);
    cout << i << " ";
    SA(m_data);
  }
  
  // string file = "Instances/p67";
  // data m_data = get_data(file.c_str());
  // // print(m_data);
  // srand(time(0));
  // for(int i = 0; i < 5; i++) {
  //   local_search(m_data);
  // }
  // for(int i = 0; i < 5; i++) {
  //   SA(m_data);
  // }
  return 0;
}