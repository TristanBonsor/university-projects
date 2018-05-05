////////////////////////////////////////////////////////////////////////////////
// main.cc
// Copyright (C) 2013  Tristan W. Bonsor
//
// Main module for CSCI 485 Lab 6: 8-Queen Constraint Satisfaction Problem.
////////////////////////////////////////////////////////////////////////////////
#include <iostream>

#include "EightQueenCSP.hpp"

// Returns either a vector of unassigned variables (failure) or else a vector
// containing a solution.
std::vector<int> backtrackingSearch(const EightQueenCSP& csp);

// Returns either a complete assignment (solution) or else an incomplete
// assignment (failure).
std::vector<int> recursiveBacktrackingSearch(std::vector<int>& assignment,
					     const EightQueenCSP& csp);

int main() {

  EightQueenCSP csp;

  std::vector<int> result = backtrackingSearch(csp);

  if(csp.isComplete(result)) {
    int i = 0;

    std::cout << "Solution:\n";
    for(auto it = result.cbegin(); it != result.cend(); ++it) {
      int c = *it % 8;
      int r = (*it - c) / 8;

      std::cout << "q_" << i++ << " = (" << c << ", " << r << ")\n";
    }
  }
  else std::cout << "No solution found\n";

  return 0;
}

std::vector<int> backtrackingSearch(const EightQueenCSP& csp) {

  std::vector<int> initial_state(csp.getNumVariables(),
				 EightQueenCSP::UNASSIGNED);

  return recursiveBacktrackingSearch(initial_state, csp);
}

std::vector<int> recursiveBacktrackingSearch(std::vector<int>& assignment,
					     const EightQueenCSP& csp) {

  if(csp.isComplete(assignment)) return assignment;

  int variable = csp.selectUnassignedVariable(assignment);

  std::vector<int> values = csp.orderDomainValues(assignment);

  for(auto value : values) {
    if(csp.isConsistant(value, assignment)) {
      assignment[variable] = value;

      std::vector<int> result = recursiveBacktrackingSearch(assignment, csp);

      if(csp.isComplete(result)) return result;
      else assignment[variable] = EightQueenCSP::UNASSIGNED;
    }
  }

  return std::vector<int>(8, EightQueenCSP::UNASSIGNED);
}
