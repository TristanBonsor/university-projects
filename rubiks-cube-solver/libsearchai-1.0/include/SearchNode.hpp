////////////////////////////////////////////////////////////////////////////////
// SearchNode.hpp
// Copyright (C) 2013  Tristan W. Bonsor
//
// Abstract base class for a (tree)search node data container.
////////////////////////////////////////////////////////////////////////////////
#ifndef __SEARCHNODE_HPP__
#define __SEARCHNODE_HPP__

#include <memory>

// Provides a uniform interface upon which a search agent may search for a
// solution contained within a problem space. Sub-classes will implement all
// methods as they are used by Agent::treeSearch() and Agent::expand() (see 
// Agent.hpp).
//
// Template Parameters:
// SEARCHNODE The sub-class implementing this abstract class.
// STATE      State component of a state successor pair.
// PATHCOST   The type used for tracking quantity in all path cost search
//            logic. Note that this could be a compound type (ie. could contain
//            different running costs).
// OUTSTREAM  A stream data structure used by various components for output.
template<class SEARCHNODE, class ACTION, class STATE, class PATHCOST,
	 class OUTSTREAM>
class SearchNode {
public:
  // Return copy of a shared pointer reference to this node's parent.
  virtual std::shared_ptr<SEARCHNODE> getParent() const = 0;

  // Return copy of a shared pointer reference to this node's action component.
  virtual std::shared_ptr<ACTION> getAction() const = 0;

  // Return copy of a shared pointer reference to this node's state component.
  virtual std::shared_ptr<STATE> getState() const = 0;

  // Returns copy of this node's search path cost.
  virtual PATHCOST getPathCost() const = 0;

  // Returns copy of this node's depth in the search tree.
  virtual unsigned int getDepth() const = 0;

  // Set the parent of this search node.
  virtual void setParent(const std::shared_ptr<SEARCHNODE>& parent_ptr) = 0;

  // Set the action component of this search node.
  virtual void setAction(const std::shared_ptr<ACTION>& action_ptr) = 0;

  // Set the state component of this search node.
  virtual void setState(const std::shared_ptr<STATE>& state_ptr) = 0;

  // Set the path cost of this search node.
  virtual void setPathCost(const PATHCOST& path_cost) = 0;

  // Set the depth of this search node.
  virtual void setDepth(const unsigned int& depth) = 0;

  // Print the contents of this node to an output character stream.
  virtual void printContents(OUTSTREAM& out_stream) = 0;
};

#endif // __SEARCHNODE_HPP__
