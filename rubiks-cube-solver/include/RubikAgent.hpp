////////////////////////////////////////////////////////////////////////////////
// RubikAgent.hpp
// Copyright (C) 2013  Tristan W. Bonsor
//
// RubikAgent class declaration.
////////////////////////////////////////////////////////////////////////////////
#ifndef __RUBIKAGENT_HPP__
#define __RUBIKAGENT_HPP__

#include "Agent.hpp"
#include "RubikPriorityFringe.hpp"

// Purpose built Agent sub-class for solving Rubik's cube problems.
class RubikAgent : public Agent<const RubikAction,
				const RubikState,
				RubikSearchNode,
				ActionHeuristicCost,
				std::ostream> {
public:
  // Out stream reference maintained for life time of RubikAgent.
  RubikAgent(std::ostream& out_stream) : Agent(out_stream) {}

  // Find a solution to a given Rubik's cube problem.
  // Returns a vector of successors from the initial state to a goal state.
  virtual std::vector<Successor<const RubikAction, const RubikState> >
  solve(Problem<const RubikAction, const RubikState, RubikSearchNode,
		ActionHeuristicCost>& problem,
	const bool& verbose,
	const int& search_type) {

    std::vector<Successor<const RubikAction, const RubikState> > solution;

    // Find a solution.
    RubikPriorityFringe fringe;
    std::shared_ptr<RubikSearchNode> result_ptr = treeSearch(problem,
							     fringe,
							     verbose);

    if(!result_ptr.get())
      return solution;

    // Order solution from intial state to goal state.
    for(auto node_ptr = result_ptr;
	node_ptr.get() != nullptr;
	node_ptr = node_ptr->getParent())
      solution.push_back(Successor<const RubikAction, const RubikState>(node_ptr->getAction(),
									node_ptr->getState()));

    return solution;
  }
};

#endif // __RUBIKAGENT_HPP__
