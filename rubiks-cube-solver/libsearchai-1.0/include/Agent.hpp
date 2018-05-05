////////////////////////////////////////////////////////////////////////////////
// Agent.hpp
// Copyright (C) 2013  Tristan W. Bonsor
//
// Abstract base class for AI search agents.
////////////////////////////////////////////////////////////////////////////////
#ifndef __AGENT_HPP__
#define __AGENT_HPP__

#include "Fringe.hpp"
#include "Problem.hpp"

// Provides basic framework for sub-class AI search agents.
// Provides canonical tree-search and search frontier expansion methods useful
// for traversing a given search space.
//
// Template Parameters:
// ACTION     Action component of a state successor pair.
// STATE      State component of a state successor pair.
// SEARCHNODE Contains an <Action,State> pair along with other search data;
//            Typically a sub-class of SearchNode (see SearchNode.hpp).
// PATHCOST   The type used for tracking quantity in all path cost search
//            logic. Note that this could be a compound type (ie. could contain
//            different running costs).
// OUTSTREAM  A stream data structure used by various components (such as
//            SearchNode) for output.
template<class ACTION, class STATE, class SEARCHNODE, class PATHCOST,
	 class OUTSTREAM>
class Agent {
public:
  // Maintains a non-const reference to output stream argument.
  Agent(OUTSTREAM& out_stream) : out_stream_(out_stream) {}

  // Return a solution (if one exists) to a given search space problem.
  virtual std::vector<Successor<ACTION, STATE> >
  solve(Problem<ACTION, STATE, SEARCHNODE, PATHCOST>& problem,
	const bool& verbose,
	const int& search_type) = 0;

protected:
  // Reference to output data structure for the lifetime of this object.
  OUTSTREAM& out_stream_;

  // Returns a shared pointer reference to search node containing a goal state.
  // If a goal state is not found, returns shared pointer reference to null. 
  std::shared_ptr<SEARCHNODE>
  treeSearch(Problem<ACTION, STATE, SEARCHNODE, PATHCOST>& problem,
	     Fringe<ACTION, STATE, SEARCHNODE, PATHCOST>& fringe,
	     const bool& verbose) {
    // Insert initial search node.
    fringe.insert(std::shared_ptr<SEARCHNODE>(new SEARCHNODE(std::shared_ptr<SEARCHNODE>(nullptr),
							     std::shared_ptr<ACTION>(new ACTION()),
							     problem.initialState(),
							     PATHCOST(0, 0),
							     0)));

    // Search.
    while(!fringe.isEmpty()) {

      // Get next viable search node.
      std::shared_ptr<SEARCHNODE> node_ptr(fringe.pop());

      if(verbose) node_ptr->printContents(out_stream_);

      // Success.
      if(problem.goalTest(node_ptr->getState())) return node_ptr;

      // Let fringe do ordering of nodes.
      fringe.insertAll(expand(node_ptr, problem));
    }

    // Search goal not found.
    return std::shared_ptr<SEARCHNODE>(nullptr);
  }

  // Provides vector containing search nodes from the search space frontier. If
  // no search nodes are availabe, returns an empty vector.
  std::vector<std::shared_ptr<SEARCHNODE> >
  expand(const std::shared_ptr<SEARCHNODE>& node_ptr,
	 Problem<ACTION, STATE, SEARCHNODE, PATHCOST>& problem) {

    std::vector<std::shared_ptr<SEARCHNODE> > successors;
    std::shared_ptr<SEARCHNODE> new_node_ptr;

    // Build new search nodes encapsulating successor function output.
    for(auto successor: problem.successorFn(node_ptr->getState())) {
      new_node_ptr.reset(new SEARCHNODE);

      new_node_ptr->setParent(node_ptr);
      new_node_ptr->setAction(successor.getAction());
      new_node_ptr->setState(successor.getState());
      new_node_ptr->setPathCost(node_ptr->getPathCost()
				+ problem.stepCost(node_ptr,
						   new_node_ptr->getAction(),
						   new_node_ptr));
      new_node_ptr->setDepth(node_ptr->getDepth() + 1);

      // Build list of candidate search nodes.
      successors.push_back(new_node_ptr);
    }

    return successors;
  }
};

#endif // __AGENT_HPP__
