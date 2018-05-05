////////////////////////////////////////////////////////////////////////////////
// RubikHeuristic.cc
// Copyright (C) 2013  Tristan W. Bonsor
//
// RubikHeuristic implementation file.
////////////////////////////////////////////////////////////////////////////////
#include "RubikHeuristic.hpp"

std::unordered_map<char, int> RubikHeuristic::NUM_COLORS_PER_FACE_INDEX =
  std::unordered_map<char, int>(
    {{RubikColor::WHITE, 0},
	{RubikColor::RED, 0},
	  {RubikColor::BLUE, 0},
	    {RubikColor::ORANGE, 0},
	      {RubikColor::GREEN, 0},
		{RubikColor::YELLOW, 0}});

// Straight forward heuristic that works well to about 9 or 10 move solutions.
RubikHeuristic::RubikHeuristicCost RubikHeuristic::rubikHeuristic(const RubikState& state,
								  const unsigned int& cardinality) {
  if(state.getValue().empty()) return 0;

  int total_color_maxima = 0;
  int local_max = 0;
  int num_color;

  int agg_color_per_face = 0;

  unsigned int max_num_one_color = cardinality * cardinality;
  unsigned int max_num_all_color = 6 * max_num_one_color;

  const RubikState::RubikStateValue& state_value = state.getValue();

  unsigned int floor, ceiling;
  for(unsigned int face = 0; face < 6; ++face) {
    floor = face * max_num_one_color;
    ceiling = floor + max_num_one_color;

    // Tally each color on face.
    for(unsigned int ix = floor; ix < ceiling; ++ix)
      ++NUM_COLORS_PER_FACE_INDEX[state_value[ix]];

    local_max = 0;

    // Find color on face with most tallies.
    for(auto iter = NUM_COLORS_PER_FACE_INDEX.begin();
	iter != NUM_COLORS_PER_FACE_INDEX.end();
	++iter) {
      num_color = std::get<1>(*iter);
      if(num_color > local_max) local_max = num_color;
      iter->second = 0;

      // Aggregate number of distinct color types per face.
      if(num_color > 0)
	++agg_color_per_face;
    }

    // Running total of the largest tally per face.
    total_color_maxima += local_max;
  }

  // Highest score (3x3x3): ((54 - 12) / 3) + (36 / 6) - 1 = 14 + 6 - 1 = 19.
  // Lowest score (3x3x3): 0 (solved).
  // Note that this is added to a current action cost total.
  return (max_num_all_color - total_color_maxima) / cardinality + (agg_color_per_face / 6) - 1;
}
