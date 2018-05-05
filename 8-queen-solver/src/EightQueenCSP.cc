////////////////////////////////////////////////////////////////////////////////
// EightQueenCSP.cc
// Copyright (C) 2013  Tristan W. Bonsor
//
// 8-Queen constraint satisfaction problem module for CSCI 485 Lab 6: 8-Queen
// Constraint Satisfaction Problem.
// EightQueenCSP class definition.
////////////////////////////////////////////////////////////////////////////////
#include <algorithm>

#include "EightQueenCSP.hpp"

bool EightQueenCSP::isConsistant(const int& value,
				 const std::vector<int>& assignment) const {

  int v_x = value % 8;
  int v_y = (value - v_x) / 8;

  int b1 = v_y - v_x;
  int b2 = v_y + v_x;

  for(auto it = assignment.cbegin(); it != assignment.cend(); ++it) {
    if(*it != UNASSIGNED) {
      int it_x = *it % 8;
      int it_y = (*it - it_x) / 8;

      // Test the horizontal and vertical.
      if(v_x == it_x) return false;
      if(v_y == it_y) return false;

      // Test diagonals.
      if(it_y == (it_x + b1)) return false;
      if(it_y == (-it_x + b2)) return false;
    }
  }

  return true;
}

int
EightQueenCSP::selectUnassignedVariable(const std::vector<int>& assignment) const {

  for(unsigned int i = 0; i < assignment.size(); ++i)
    if(assignment[i] == EightQueenCSP::UNASSIGNED)
      return i;

  return -1;
}

std::vector<int>
EightQueenCSP::orderDomainValues(const std::vector<int>& assignment) const {

  std::vector<int> values;

  for(int i = 0; i < num_values_; ++i) values.push_back(i);

  for(auto it = assignment.cbegin(); it != assignment.cend(); ++it)
    std::remove(values.begin(), values.end(), *it);

  return values;
}

int EightQueenCSP::getNumVariables() const {

  return num_variables_;
}

bool EightQueenCSP::isComplete(const std::vector<int>& assignment) const {

  for(auto it = assignment.cbegin(); it != assignment.cend(); ++it)
    if(*it == EightQueenCSP::UNASSIGNED) return false;

  return true;
}
