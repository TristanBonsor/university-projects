////////////////////////////////////////////////////////////////////////////////
// Successor.hpp
// Copyright (C) 2013  Tristan W. Bonsor
//
// Successor class declaration.
////////////////////////////////////////////////////////////////////////////////
#ifndef __SUCCESSOR_HPP__
#define __SUCCESSOR_HPP__

#include <memory>

// Represents a <Action,State> pair. A simple container encapsulating shared
// pointer references to the action and state components. Provides a simple
// interface utilized by Agent::expand() (see Agent.hpp). This type is returned
// (stored within a vector) from Problem::successorFn() (see Problem.hpp).
//
// Template Parameters:
// ACTION     Action component of a state successor pair.
// STATE      State component of a state successor pair.
template<class ACTION, class STATE>
class Successor {
public:
  // Stores copies of shared pointer references to the action and state
  // components of this successor pair.
  Successor(const std::shared_ptr<ACTION>& action_ptr,
	    const std::shared_ptr<STATE>& state_ptr)
    : action_ptr_(action_ptr)
    , state_ptr_(state_ptr) {}

  // Returns copy of shared pointer reference to the action component of this
  // successor pair.
  std::shared_ptr<ACTION> getAction() const { return action_ptr_; }

  // Returns copy of shared pointer reference to the state component of this
  // successor pair.
  std::shared_ptr<STATE> getState() const { return state_ptr_; }

protected:
  // Maintains references to this successor pair's action and state components.
  std::shared_ptr<ACTION> action_ptr_;
  std::shared_ptr<STATE> state_ptr_;
};

#endif // __SUCCESSOR_HPP__
