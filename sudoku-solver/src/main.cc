////////////////////////////////////////////////////////////////////////////////
// main.cc
// Copyright (C) 2013  Tristan W. Bonsor
//
// Main module for CSCI 485 Lab 7: Sudoku Constraint Satisfaction Problem.
////////////////////////////////////////////////////////////////////////////////
#include "SudokuCsp.hpp"

int main() {

  Variable<unsigned int> x2(2);
  x2.value = 7;
  Variable<unsigned int> x4(4);
  x4.value = 6;
  Variable<unsigned int> x5(5);
  x5.value = 8;
  Variable<unsigned int> x9(9);
  x9.value = 4;
  Variable<unsigned int> x10(10);
  x10.value = 5;
  Variable<unsigned int> x12(12);
  x12.value = 7;
  Variable<unsigned int> x14(14);
  x14.value = 9;
  Variable<unsigned int> x19(19);
  x19.value = 9;
  Variable<unsigned int> x27(27);
  x27.value = 2;
  Variable<unsigned int> x32(32);
  x32.value = 4;
  Variable<unsigned int> x33(33);
  x33.value = 8;
  Variable<unsigned int> x34(34);
  x34.value = 5;
  Variable<unsigned int> x38(38);
  x38.value = 5;
  Variable<unsigned int> x42(42);
  x42.value = 2;
  Variable<unsigned int> x46(46);
  x46.value = 7;
  Variable<unsigned int> x47(47);
  x47.value = 3;
  Variable<unsigned int> x48(48);
  x48.value = 5;
  Variable<unsigned int> x53(53);
  x53.value = 1;
  Variable<unsigned int> x61(61);
  x61.value = 2;
  Variable<unsigned int> x66(66);
  x66.value = 2;
  Variable<unsigned int> x68(68);
  x68.value = 7;
  Variable<unsigned int> x70(70);
  x70.value = 8;
  Variable<unsigned int> x71(71);
  x71.value = 9;
  Variable<unsigned int> x75(75);
  x75.value = 3;
  Variable<unsigned int> x76(76);
  x76.value = 1;
  Variable<unsigned int> x78(78);
  x78.value = 4;

  AssignmentSet<unsigned int> initial_state;
  initial_state.add(x2);
  initial_state.add(x4);
  initial_state.add(x5);
  initial_state.add(x9);
  initial_state.add(x10);
  initial_state.add(x12);
  initial_state.add(x14);
  initial_state.add(x19);
  initial_state.add(x27);
  initial_state.add(x32);
  initial_state.add(x33);
  initial_state.add(x34);
  initial_state.add(x38);
  initial_state.add(x42);
  initial_state.add(x46);
  initial_state.add(x47);
  initial_state.add(x48);
  initial_state.add(x53);
  initial_state.add(x61);
  initial_state.add(x66);
  initial_state.add(x68);
  initial_state.add(x70);
  initial_state.add(x71);
  initial_state.add(x75);
  initial_state.add(x76);
  initial_state.add(x78);

  SudokuCsp csp(3);

  BacktrackingSearch<unsigned int> search;

  AssignmentSet<unsigned int> result = search(initial_state, csp);

  std::set<Variable<unsigned int> > initial_assignments = initial_state.getCurrentAssignments();
  std::set<Variable<unsigned int> > solution_assignments = result.getCurrentAssignments();

  unsigned int ix = 0;

  std::cout << "Initial state:";
  for(auto it = initial_assignments.cbegin(); it != initial_assignments.cend(); ++it) {
    unsigned int enumeration = it->getEnumeration();

    if(!(ix % (3 * 3)))
      std::cout << "\n";

    while(ix < enumeration) {
      std::cout << "  ";

      if(!(++ix % (3 * 3)))
	std::cout << "\n";
    }

    ++ix;
    std::cout << it->value << " ";
  }
  for(; ix < 3 * 3 * 3 * 3; ++ix) {
    if(!(ix % (3 * 3)))
      std::cout << "\n";

    std::cout << "  ";
  }
  std::cout << "\n";

  ix = 0;

  std::cout << "Solution:";
  for(auto it = solution_assignments.cbegin(); it != solution_assignments.cend(); ++it) {
    if(!(ix++ % (3 * 3)))
      std::cout << "\n";
    std::cout << it->value << " ";
  }
  std::cout << "\n";

  return 0;
}
