////////////////////////////////////////////////////////////////////////////////
// rube.cc
// Copyright (C) 2013  Tristan W. Bonsor
//
// Main file for CSCI 485 Assignment 1: Rubik's cube solver.
////////////////////////////////////////////////////////////////////////////////
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unistd.h>

#include "RubikAgent.hpp"
#include "RubikParser.hpp"
#include "RubikProblem.hpp"

// Print command line usage to stdout.
void printUsage(const std::string& prog_name);

// Process command line arguments.
void processArgs(const int& argc,
		 char*const*const& argv,
		 bool& is_verbose,
		 char*& in_file_ptr);

// Process data from input file.
std::pair<unsigned int, std::shared_ptr<const RubikState> >
processInput(char*const& in_file_ptr);

// Print Rubik's cube solution to stdout.
void printSolution(std::vector<Successor<const RubikAction, const RubikState> >& solution);

int main(int argc, char** argv) {

  bool is_verbose = false;
  char* in_file_ptr = nullptr;

  processArgs(argc, argv, is_verbose, in_file_ptr);

  std::pair<unsigned int, std::shared_ptr<const RubikState> >
    parse_result = processInput(in_file_ptr);

  unsigned int cardinality
    = std::get<0>(parse_result);

  std::shared_ptr<const RubikState> initial_state
    = std::get<1>(parse_result);

  RubikProblem problem(initial_state, cardinality);

  RubikAgent agent(std::cout);

  auto solution = agent.solve(problem, is_verbose, 0);

  printSolution(solution);

  return 0;
}

void printUsage(const std::string& prog_name) {
  std::cout << "Usage: " << prog_name << " [-v] FILE\n";
}

void processArgs(const int& argc,
		 char*const*const& argv,
		 bool& is_verbose,
		 char*& in_file_ptr) {
 int c;

  if(argc < 2) {
    printUsage(argv[0]);
    exit(1);
  }

  opterr = 0;

  while((c = getopt(argc, argv, "v")) != -1) {
    switch(c) {
    case 'v':
      is_verbose = true;
      break;
    case '?':
    default:
      printUsage(argv[0]);
      exit(1);
    }
  }

  if(is_verbose && argc == 2) {
    printUsage(argv[0]);
    exit(1);
  }

  in_file_ptr = argv[optind];
}

std::pair<unsigned int, std::shared_ptr<const RubikState> >
processInput(char*const& in_file_ptr) {

  std::fstream fin;
  RubikParser parser;

  fin.open(in_file_ptr, std::fstream::in);

  std::queue<std::tuple<RubikScanner::TokenType, char, unsigned int> >
    tokens = RubikScanner::scan(fin);

  fin.close();

  std::pair<unsigned int, std::shared_ptr<const RubikState> >
    parse_result = parser.parse(tokens);

  if(parser.isError()) {
    std::cout << parser.getErrorMessage();
    exit(1);
  }

  return parse_result;
}

void printSolution(std::vector<Successor<const RubikAction, const RubikState> >& solution) {
  std::cout << "****************************************\n"
	    << "SOLUTION FOUND\n";
  for(auto it = solution.rbegin(); it != solution.rend(); ++it) {
    std::cout << "****************************************\n";

    auto action_ptr = it->getAction();
    auto state_ptr  = it->getState();

    switch(action_ptr->getType()) {
    case RubikAction::RubikActionType::START:
      std::cout << "Action:    " << "Start\n";
      break;
    case RubikAction::RubikActionType::ROTATE_XY:
      std::cout << "Action:    "     << "Rotate row "
		<< action_ptr->getIndex() << "\n";
      break;
    case RubikAction::RubikActionType::ROTATE_YZ:
      std::cout << "Action:    "     << "Rotate rank "
		<< action_ptr->getIndex() << "\n";
      break;
    case RubikAction::RubikActionType::ROTATE_ZX:
      std::cout << "Action:    "     << "Rotate column "
		<< action_ptr->getIndex() << "\n";
      break;
    default:
      std::cout << "Action:         " << "None\n";
      break;
    }

    switch(action_ptr->getDirection()) {
    case RubikAction::RubikActionDirection::CW:
      std::cout << "Direction: " << "Clockwise\n";
      break;
    case RubikAction::RubikActionDirection::CCW:
      std::cout << "Direction: " << "Counter-Clockwise\n";
      break;
    default:
      std::cout << "Direction: " << "None\n";
    }

    std::cout << "Value:     " << state_ptr->getValue() << "\n";
  }
}
