////////////////////////////////////////////////////////////////////////////////
// RubikSearchNode.hpp
// Copyright (C) 2013  Tristan W. Bonsor
//
// RubikSearchNode class declaration.
////////////////////////////////////////////////////////////////////////////////
#ifndef __RUBIKSEARCHNODE_HPP__
#define __RUBIKSEARCHNODE_HPP__

#include <ostream>

#include "SearchNode.hpp"
#include "RubikDefines.hpp"

// Search node specialized for use in solving Rubik's cube problems.
class RubikSearchNode : SearchNode<RubikSearchNode,
				   const RubikAction,
				   const RubikState,
				   ActionHeuristicCost,
				   std::ostream> {
public:
  // Default.
  RubikSearchNode()
    : parent_ptr_()
    , action_ptr_()
    , state_ptr_(),
      path_cost_(),
      depth_() {}

  RubikSearchNode(const std::shared_ptr<RubikSearchNode>& parent_ptr,
		  const std::shared_ptr<const RubikAction>& action_ptr,
		  const std::shared_ptr<const RubikState>& state_ptr,
		  const ActionHeuristicCost& path_cost,
		  const unsigned int& depth)
    : parent_ptr_(parent_ptr)
    , action_ptr_(action_ptr)
    , state_ptr_(state_ptr)
    , path_cost_(path_cost)
    , depth_(depth) {}

  // Standard getters and setters follow. See also printContents (below).

  virtual std::shared_ptr<RubikSearchNode> getParent() const {
    return parent_ptr_;
  }

  virtual std::shared_ptr<const RubikAction> getAction() const {
    return action_ptr_;
  }

  virtual std::shared_ptr<const RubikState> getState() const {
    return state_ptr_;
  }

  virtual ActionHeuristicCost getPathCost() const {
    return path_cost_;
  }

  virtual unsigned int getDepth() const { return depth_; }

  virtual void setParent(const std::shared_ptr<RubikSearchNode>& parent_ptr) {
    parent_ptr_ = parent_ptr;
  }

  virtual void setAction(const std::shared_ptr<const RubikAction>& action_ptr) {
    action_ptr_ = action_ptr;
  }

  virtual void setState(const std::shared_ptr<const RubikState>& state_ptr) {
    state_ptr_ = state_ptr;
  }

  virtual void setPathCost(const ActionHeuristicCost& path_cost) {
    path_cost_ = path_cost;
  }

  virtual void setDepth(const unsigned int& depth) {
    depth_ = depth;
  }

  virtual void printContents(std::ostream& out_stream) {
    switch(action_ptr_->getType()) {
    case RubikAction::RubikActionType::START:
      out_stream << "Action:         " << "Start\n";
      break;
    case RubikAction::RubikActionType::ROTATE_XY:
      out_stream << "Action:         " << "Rotate row " << action_ptr_->getIndex() << "\n";
      break;
    case RubikAction::RubikActionType::ROTATE_YZ:
      out_stream << "Action:         " << "Rotate rank " << action_ptr_->getIndex() << "\n";
      break;
    case RubikAction::RubikActionType::ROTATE_ZX:
      out_stream << "Action:         " << "Rotate column " << action_ptr_->getIndex() << "\n";
      break;
    default:
      out_stream << "Action:         " << "None\n";
      break;
    }

    switch(action_ptr_->getDirection()) {
    case RubikAction::RubikActionDirection::CW:
      out_stream << "Direction:      " << "Clockwise\n";
      break;
    case RubikAction::RubikActionDirection::CCW:
      out_stream << "Direction:      " << "Counter-Clockwise\n";
      break;
    default:
      out_stream << "Direction:      " << "None\n";
    }

    out_stream << "Action Cost:    " << path_cost_.getActionCost() << "\n";
    out_stream << "Heuristic Cost: " << path_cost_.getHeuristicCost() << "\n";
    out_stream << "Value:          " << state_ptr_->getValue() << "\n";
  }

protected:
  // Reference to this node's parent node with in the search tree.
  std::shared_ptr<RubikSearchNode> parent_ptr_;

  // Action component of the (Action, State) pair.
  std::shared_ptr<const RubikAction> action_ptr_;

  // State component of the (Action, State) pair.
  std::shared_ptr<const RubikState> state_ptr_;

  // Cost to reach this node from start of search tree.
  ActionHeuristicCost path_cost_;

  // Current depth that this node resides at within the search tree.
  unsigned int depth_;
};

#endif // __RUBIKSEARCHNODE_HPP__
