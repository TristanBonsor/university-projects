////////////////////////////////////////////////////////////////////////////////
// generator.cc
// Copyright (C) 2013  Tristan W. Bonsor
//
// Generates a random Rubik's cube configuration.
// 
// Command line arguments:
// <cardinality> n=2, 3, or 4 (nxnxn cube).
// <moves> n >= 0 (cube may be solved in n or less moves).
////////////////////////////////////////////////////////////////////////////////
#include <cstdlib>
#include <iostream>

#include "RubikProblem.hpp"

#define NUM_FACES 6

const std::string colors = "WRGBYO";

int main(int argc, char** argv) {

  if(argc != 3) {
    std::cout << "Usage: " << argv[0] << " CARDINALITY NUM_MOVES\n";
    return 0;
  }

  int cardinality = atoi(argv[1]);
  int moves = atoi(argv[2]);

  int num_colors = cardinality * cardinality;

  RubikState::RubikStateValue config;

  for(int i = 0; i < NUM_FACES; ++i)
    config.append(num_colors, colors[i]);

  std::shared_ptr<const RubikState> state(new RubikState(config));

  RubikProblem problem(state, cardinality);

  srand(time(0));

  for(int i = 0; i < moves; ++i) {
    int ix = rand() % (NUM_FACES * cardinality);
    config.assign(problem.successorFn(state)[ix].getState()->getValue());
    state.reset(new RubikState(config));
  }

  std::cout << cardinality << "\n"
	    << "FRONT\n";

  for(int i = 0; i < cardinality; ++i)
    std::cout << config.substr(i * cardinality, cardinality) << "\n";

  std::cout << "RIGHT\n";

  for(int i = 0; i < cardinality; ++i)
    std::cout << config.substr(num_colors + i * cardinality, cardinality) << "\n";

  std::cout << "TOP\n";

  for(int i = 0; i < cardinality; ++i)
    std::cout << config.substr(2 * num_colors + i * cardinality, cardinality) << "\n";

  std::cout << "BOTTOM\n";

  for(int i = 0; i < cardinality; ++i)
    std::cout << config.substr(3 * num_colors + i * cardinality, cardinality) << "\n";

  std::cout << "LEFT\n";

  for(int i = 0; i < cardinality; ++i)
    std::cout << config.substr(4 * num_colors + i * cardinality, cardinality) << "\n";

  std::cout << "BACK\n";

  for(int i = 0; i < cardinality; ++i)
    std::cout << config.substr(5 * num_colors + i * cardinality, cardinality) << "\n";

  return 0;
}
