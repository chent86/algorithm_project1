#ifndef LOCAL_HPP
#define LOCAL_HPP

#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>
#include "define.hpp"
using namespace std;

int local_search(data& m_data);

void init_solution(vector<int>& assignment, vector<int>& facility_used, data& m_data, vector<int>& m_random);

int get_random(int range);

int evaluation(vector<int>& assignment, data& m_data);

bool can_open(int facility, vector<int>& m_random);

#endif