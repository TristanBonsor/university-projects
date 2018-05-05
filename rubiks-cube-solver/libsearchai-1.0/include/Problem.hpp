////////////////////////////////////////////////////////////////////////////////
// Problem.hpp
// Copyright (C) 2013  Tristan W. Bonsor
//
// Abstract base class for searchable problem spaces.
////////////////////////////////////////////////////////////////////////////////
#ifndef __PROBLEM_HPP__
#define __PROBLEM_HPP__

#include <vector>

#include "Successor.hpp"

// Provides a uniform interface upon which a search agent may search for a
// solution contained within a problem space. Sub-classes will implement all
// methods and define a specific problem space.
//
// Template Parameters:
// ACTION     Action component of a state successor pair.
// STATE      State component of a state successor pair.
// SEARCHNODE Contains an <Action,State> pair along with other search data;
//            Typically a sub-class of SearchNode (see SearchNode.hpp).
// PATHCOST   The type used for tracking quantity in all path cost search
//            logic. Note that this could be a compound type (ie. could contain
//            different running costs).
template<class ACTION, class STATE, class SEARCHNODE, class PATHCOST>
class Problem {
public:
  // Stores copy of shared pointer reference to the initial state of a problem.
  Problem(const std::shared_ptr<STATE>& init_state_ptr)
  : init_state_ptr_(init_state_ptr) {}

  // Returns copy of shared pointer reference to the initial state of a problem.
  virtual std::shared_ptr<STATE> initialState() const {
    return init_state_ptr_;
  }

  // Returns vector of <Action,State> successor objects (see Successor.hpp).
  virtual std::vector<Successor<ACTION, STATE> >
  successorFn(const std::shared_ptr<STATE>& state_ptr) = 0;

  // Returns the path cost for a given state transition. The shared pointer
  // references to search nodes referencing the current state and successor
  // state, as well as a shared pointer reference to the action that begot
  // the transition, are all used in the path cost calculation.
  virtual PATHCOST stepCost(const std::shared_ptr<SEARCHNODE>& orig_node_ptr,
			    const std::shared_ptr<ACTION>& action_ptr,
			    const std::shared_ptr<SEARCHNODE>& new_node_ptr) const = 0;

  // Test if search node (refenced by a shared pointer) contains a goal state
  // for a problem.
  virtual bool goalTest(const std::shared_ptr<STATE>& state_ptr) const = 0;

protected:
  // Maintains a reference to the problem's initial state.
  std::shared_ptr<STATE> init_state_ptr_;
};

#endif // __PROBLEM_HPP__
