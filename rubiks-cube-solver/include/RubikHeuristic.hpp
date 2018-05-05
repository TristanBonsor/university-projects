////////////////////////////////////////////////////////////////////////////////
// RubikHeuristic.hpp
// Copyright (C) 2013  Tristan W. Bonsor
//
// RubikHeuristic class declaration.
////////////////////////////////////////////////////////////////////////////////
#ifndef __RUBIKHEURISTIC_HPP__
#define __RUBIKHEURISTIC_HPP__

#include <unordered_map>

#include "RubikColor.hpp"
#include "RubikState.hpp"

// Encapsulates all heuristic calculations for seeking the least cost
// path for solving the rubik's cube.
class RubikHeuristic {
public:
  typedef int RubikHeuristicCost;

  // Returns a value indicating how expensive the path is through this state
  // to the goal state.
  static RubikHeuristicCost rubikHeuristic(const RubikState& state,
					   const unsigned int& cardinality);

private:

  // Used for tallying the occurance of colors on a given cube face.
  static std::unordered_map<char, int> NUM_COLORS_PER_FACE_INDEX;
};

#endif // __RUBIKHEURISTIC_HPP__
