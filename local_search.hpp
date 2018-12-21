#ifndef LOCAL_HPP
#define LOCAL_HPP

#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>
#include "define.hpp"
using namespace std;

void local_search(data& m_data);

void init_solution(vector<int>& assignment, vector<int>& facility_used, data& m_data);

int get_random(int range);

int evaluation(vector<int>& assignment, data& m_data);

#endif