#include "define.hpp"
#include "data_reader.hpp"
#include "local_search.hpp"
#include <string>
#include <cstdlib>
#include <ctime>

// void SA(data& m_data) {

// }
    
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