////////////////////////////////////////////////////////////////////////////////
// EightQueenCSP.hpp
// Copyright (C) 2013  Tristan W. Bonsor
//
// 8-Queen constraint satisfaction problem module for CSCI 485 Lab 6: 8-Queen
// Constraint Satisfaction Problem.
// EightQueenCSP class declaration.
////////////////////////////////////////////////////////////////////////////////
#ifndef __EIGHTQUEENCSP_HPP__
#define __EIGHTQUEENCSP_HPP__

#include <vector>

// Represents an 8-Queen constraint satisfaction problem.
class EightQueenCSP {
public:

  static const int UNASSIGNED = -1;

  // Number of variables and number of values stay the same for lifetime of
  // the object.
  EightQueenCSP() : num_variables_(8), num_values_(64) {}

  // Returns true if the given value is consistant with the current assignment
  // of variables (no queen attacks another), false otherwise.
  bool isConsistant(const int& value,
		    const std::vector<int>& assignment) const;

  // Returns the enumeration of the next variable to assign a value to.
  int selectUnassignedVariable(const std::vector<int>& assignment) const;

  // Returns a vector of available values that may be assigned.
  std::vector<int> orderDomainValues(const std::vector<int>& assignment) const;

  // Returns the total number of variables that need to be assigned a value.
  int getNumVariables() const;

  // Returns true if all variables have been assigned a value, false otherwise.
  bool isComplete(const std::vector<int>& assignment) const;

private:

  // Values stay the same for the lifetime of the object.
  int num_variables_, num_values_;
};

#endif //__EIGHTQUEENCSP_HPP__
