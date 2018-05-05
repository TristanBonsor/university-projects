////////////////////////////////////////////////////////////////////////////////
// RubikAction.hpp
// Copyright (C) 2013  Tristan W. Bonsor
//
// RubikAction class declaration.
////////////////////////////////////////////////////////////////////////////////
#ifndef __RUBIKACTION_HPP__
#define __RUBIKACTION_HPP__

#include <string>
#include <tuple>

// Access specific tuple positions.
#define RUBIK_ACTION_TYPE_INDEX   0
#define RUBIK_ACTION_DIRECT_INDEX 1
#define RUBIK_ACTION_INDEX_INDEX  2
#define RUBIK_ACTION_COST_INDEX   3

// Encapsulates action components that apply to moves available with an nxnxn
// rubik's cube. These include the type of actions (rotations in the XY, YZ, or
// ZX planes), the direction of rotation (clockwise or counter-clockwise),
// index of the cube that the rotation occurs on (column, rank, or row), and
// the cost of the rotation (uniform cost of 1 in this case).
class RubikAction {
public:
  // Indexes the columns, ranks, or rows of a rubik's cube.
  typedef unsigned int RubikActionIndex;

  typedef int RubikActionCost;

  // Hashable ID for each action - useful for initializing look up table with
  // all possible actions (6n) to save on allocation overhead.
  typedef std::string RubikActionId;

  // Enumerate rotation types - useful for initializing look up table with
  // all possible actions (6n) to save on allocation overhead.
  enum RubikActionType : int { START = 0, ROTATE_XY, ROTATE_YZ, ROTATE_ZX };
  static const int MAX_NUM_RUBIK_ACTION_TYPE = 4;

  // Enumerate rotation direction types - useful for initializing look up table
  // with all possible actions (6n) to save on allocation overhead.
  enum RubikActionDirection : int { NONE = 0, CW, CCW };
  static const int MAX_NUM_RUBIK_ACTION_DIRECTION = 3;

  // Creates immutable default start action (for pairing with the problem's
  // initial state).
  RubikAction()
    : rubik_action_tuple_(RubikActionType::START, RubikActionDirection::NONE, 0, 0)
    , rubik_action_id_("0000") {}

  // Creates immutable action with copies of all specified rotation components.
  RubikAction(const RubikActionType& type,
	      const RubikActionDirection& direct,
	      const RubikActionIndex& index,
	      const RubikActionCost& cost)
    : rubik_action_tuple_(type, direct, index, cost) {

    // Initialize action id for look up table.
    rubik_action_id_ = std::to_string(type) + std::to_string(direct)
      + std::to_string(index) + std::to_string(cost);
  }

  // Returns type of action (rotation in the XY, YZ, or ZX planes).
  RubikActionType getType() const {
    return std::get<RUBIK_ACTION_TYPE_INDEX>(rubik_action_tuple_);
  }

  // Returns type of rotation direction (clockwise or counter-clockwise).
  RubikActionDirection getDirection() const {
    return std::get<RUBIK_ACTION_DIRECT_INDEX>(rubik_action_tuple_);
  }

  // Returns zero-based index indicating column, rank, or row of the
  // rubik's cube that this rotation occurs on.
  RubikActionIndex getIndex() const {
    return std::get<RUBIK_ACTION_INDEX_INDEX>(rubik_action_tuple_);
  }

  // Return the cost of the action.
  RubikActionCost getCost() const {
    return std::get<RUBIK_ACTION_COST_INDEX>(rubik_action_tuple_);
  }

  // Returns the ID of the action.
  RubikActionId getId() const { return rubik_action_id_; }

protected:
  // Contains copies of all components of a given action.
  std::tuple<RubikActionType, RubikActionDirection, RubikActionIndex, RubikActionCost>
  rubik_action_tuple_;

  // This should uniquely identify this action.
  RubikActionId rubik_action_id_;
};

#endif // __ACTION_HPP__
