////////////////////////////////////////////////////////////////////////////////
// Fringe.hpp
// Copyright (C) 2013  Tristan W. Bonsor
//
// Abstract base class for a sorting container of search nodes. Used by
// Agent::treeSearch() and Agent::expand() (see Agent.hpp).
////////////////////////////////////////////////////////////////////////////////
#ifndef __FRINGE_HPP__
#define __FRINGE_HPP__

#include <memory>
#include <vector>

// Provides ordering and 'temporary housing' for search nodes that have yet to
// be inspected.
//
// Template Parameters
// ACTION     Action component of a state successor pair.
// STATE      State component of a state successor pair.
// SEARCHNODE Contains an <Action,State> pair along with other search data;
//            Typically a sub-class of SearchNode (see SearchNode.hpp).
// PATHCOST   The type used for tracking quantity in all path cost search
//            logic. Note that this could be a compound type (ie. could contain
//            different running costs).
template<class ACTION, class STATE, class SEARCHNODE, class PATHCOST>
class Fringe {
public:
  // Returns true if empty, false otherwise.
  virtual bool isEmpty() const = 0;

  // Insert an individual search node shared pointer reference.
  virtual void insert(const std::shared_ptr<SEARCHNODE>& node_ptr) = 0;

  // Insert all search node shared pointer references contained in vector.
  virtual void
  insertAll(const std::vector<std::shared_ptr<SEARCHNODE> >& node_list) = 0;

  // Remove the next available search node shared pointer reference and
  // return its value.
  virtual std::shared_ptr<SEARCHNODE> pop() = 0;
};

#endif // __FRINGE_HPP__
