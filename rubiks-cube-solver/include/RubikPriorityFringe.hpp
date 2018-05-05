////////////////////////////////////////////////////////////////////////////////
// RubikPriorityFringe.hpp
// Copyright (C) 2013  Tristan W. Bonsor
//
// RubikPriorityFringe class declaration.
////////////////////////////////////////////////////////////////////////////////
#ifndef __RUBIKPRIORITYFRINGE_HPP__
#define __RUBIKPRIORITYFRINGE_HPP__

#include <queue>

#include "Fringe.hpp"
#include "RubikSearchNode.hpp"

// Container class meant for A* problem space search for Rubik's cube problem.
// Utilized by RubikAgent.
class RubikPriorityFringe : public Fringe<const RubikAction,
					  const RubikState,
					  RubikSearchNode,
					  ActionHeuristicCost> {
public:
  // Comparison class for PathCost objects. Used for ordering priority queue.
  class NodeCompare {
  public:
    bool operator()(const std::shared_ptr<RubikSearchNode>& lhs,
		    const std::shared_ptr<RubikSearchNode>& rhs) const {
      return (lhs->getPathCost().getHeuristicCost() >= rhs->getPathCost().getHeuristicCost());
    }
  };

  RubikPriorityFringe() : p_queue_() {}

  virtual bool isEmpty() const { return p_queue_.empty(); }

  // Insert shared pointer reference to search node for inspection ordering.
  virtual void insert(const std::shared_ptr<RubikSearchNode>& node_ptr) {
    ActionHeuristicCost node_cost = node_ptr->getPathCost();

    // The state contained by this node is already discovered at a cheaper
    // culmulative action cost.
    if(!node_cost.isValidCost()) return;

    p_queue_.push(node_ptr);
  }

  // Insert vector of shared pointer references to search nodes for inspection
  // ordering.
  virtual void insertAll(const std::vector<std::shared_ptr<RubikSearchNode> >& node_list) {
    for(auto node_ptr: node_list) insert(node_ptr);
  }

  // Remove shared pointer reference to search node and remove it from the
  // search fringe.
  virtual std::shared_ptr<RubikSearchNode> pop() {
    auto node_ptr = p_queue_.top();
    p_queue_.pop();
    return node_ptr;
  }

protected:
  std::priority_queue<std::shared_ptr<RubikSearchNode>, std::vector<std::shared_ptr<RubikSearchNode> >, NodeCompare> p_queue_;
};

#endif // __RUBIKPRIORITYFRINGE_HPP__
