////////////////////////////////////////////////////////////////////////////////
// RubikProblem.hpp
// Copyright (C) 2013  Tristan W. Bonsor
//
// RubikProblem class declaration.
////////////////////////////////////////////////////////////////////////////////
#ifndef __RUBIKPROBLEM_HPP__
#define __RUBIKPROBLEM_HPP__

#include <algorithm>
#include <queue>

#include "Problem.hpp"
#include "RubikSearchNode.hpp"

// Rubik's cube problem space class.
// Utilized by RubikAgent.
class RubikProblem : public Problem<const RubikAction,
				    const RubikState,
				    RubikSearchNode,
				    ActionHeuristicCost> {
public:
  // Construct a Rubik's cube problem of cardinality nxnxn with an initial
  // state.
  RubikProblem(const std::shared_ptr<const RubikState>& init_state_ptr,
	       const unsigned int& cardinality)
    : Problem(init_state_ptr)
    , cardinality_(cardinality)
    , rubik_action_index_ptr_(new std::unordered_map<RubikAction::RubikActionId,
						     std::shared_ptr<const RubikAction> >)
    , rubik_state_index_ptr_(new std::unordered_map<RubikState::RubikStateValue,
						    std::shared_ptr<const RubikState> >)
    , discovered_node_index_ptr_(new std::unordered_map<RubikState::RubikStateValue,
							std::shared_ptr<RubikSearchNode> >) {

    // Initialize action index.
    std::shared_ptr<const RubikAction> action_ptr;

    for(unsigned int ix = 0; ix < cardinality_; ++ix) {
      for(int type = RubikAction::RubikActionType::ROTATE_XY;
	  type < RubikAction::MAX_NUM_RUBIK_ACTION_TYPE;
	  ++type) {
	for(int direct = RubikAction::RubikActionDirection::CW;
	    direct < RubikAction::MAX_NUM_RUBIK_ACTION_DIRECTION;
	    ++direct) {
	  action_ptr.reset(new RubikAction((RubikAction::RubikActionType)type,
					   (RubikAction::RubikActionDirection)direct,
					   (RubikAction::RubikActionIndex)ix,
					   1));
	  rubik_action_index_ptr_->emplace(action_ptr->getId(), action_ptr);
	}
      }
    }

    // Store initial state (shared reference) in the state index.
    rubik_state_index_ptr_->emplace(init_state_ptr_->getValue(),
				    init_state_ptr_);
  }

  // Return a vector of action/state pairs (successor objects) that are
  // reachable from a given state (cube configuration).
  // This vector will contain 6 * cardinality successor objects.
  virtual std::vector<Successor<const RubikAction, const RubikState> >
  successorFn(const std::shared_ptr<const RubikState>& state_ptr) {

    std::vector<Successor<const RubikAction, const RubikState> > successors;

    // Iterate through every possible action and generate and store
    // corresponding state (cube configuration).
    for(auto action_iter = rubik_action_index_ptr_->cbegin();
	action_iter != rubik_action_index_ptr_->cend();
	++action_iter)
      successors.push_back(Successor<const RubikAction, const RubikState>(action_iter->second,
									  generateNextState(state_ptr,
											    action_iter->second)));

    return successors;
  }

  // Return the step cost to transition from one state to another via
  // a given action.
  virtual ActionHeuristicCost
  stepCost(const std::shared_ptr<RubikSearchNode>& orig_node_ptr,
	   const std::shared_ptr<const RubikAction>& action_ptr,
	   const std::shared_ptr<RubikSearchNode>& new_node_ptr) const {

    // Get total action cost up to the new search node. 
    RubikAction::RubikActionCost tmp_cost = orig_node_ptr->getPathCost().getActionCost()
      + action_ptr->getCost();

    // Check if original search node with this state already exists.
    auto node_iter = discovered_node_index_ptr_->find(orig_node_ptr->getState()->getValue());

    // Does not exist, insert.
    if(node_iter == discovered_node_index_ptr_->end())
      discovered_node_index_ptr_->emplace(orig_node_ptr->getState()->getValue(),
					  orig_node_ptr);
    // Exists but orig node has cheaper action cost, replace.
    else if(orig_node_ptr->getPathCost().getActionCost() < node_iter->second->getPathCost().getActionCost())
      discovered_node_index_ptr_->at(orig_node_ptr->getState()->getValue()) = orig_node_ptr;

    // Check if new search node with this state already exists.
    node_iter = discovered_node_index_ptr_->find(new_node_ptr->getState()->getValue());

    // Insert new valid search node.
    if(node_iter == discovered_node_index_ptr_->end()
       || tmp_cost < new_node_ptr->getPathCost().getActionCost()) {
      new_node_ptr->setParent(orig_node_ptr);

      RubikHeuristic::RubikHeuristicCost h_cost = tmp_cost
	+ RubikHeuristic::rubikHeuristic(new_node_ptr->getState()->getValue(),
					 cardinality_)
	- orig_node_ptr->getPathCost().getHeuristicCost();

      // THe cost of the new search node.
      return ActionHeuristicCost(action_ptr->getCost(), h_cost);
    }

    // Return a "null" cost.
    return ActionHeuristicCost();
  }

  // Returns true if Rubik's cube is solved (ie. state is a valid
  // win configuration.
  virtual bool
  goalTest(const std::shared_ptr<const RubikState>& state_ptr) const {
    const RubikState::RubikStateValue state_value = state_ptr->getValue();

    const unsigned int num_colors_per_face = cardinality_ * cardinality_;

    for(unsigned int f = 0; f < 6; ++f) {
      RubikState::RubikStateValue face = state_value.substr(f * num_colors_per_face,
							    num_colors_per_face);
      for(unsigned int i = 0; i < num_colors_per_face - 1; ++i)
	if(face[i] != face[i+1]) return false;
    }

    return true;
  }

protected:
  // ***NOTE: Faces are ordered FRONT, RIGHT, TOP, BOTTOM, LEFT, BACK or
  // ============================>0=====>1====>2=====>3=====>4====>5
  // Within a given state value. Each face is cardinality^2 in length.

  // Face cycles for the three rotation planes.
  static const int FACE_LIST_SIZE = 5;
  const int face_list_xy[FACE_LIST_SIZE] = {0, 4, 5, 1, 0};
  const int face_list_yz[FACE_LIST_SIZE] = {1, 3, 4, 2, 1};
  const int face_list_zx[FACE_LIST_SIZE] = {0, 2, 5, 3, 0};

  // nxnxn.
  const unsigned int cardinality_;

  // Lots of pointer copies instead of lots action and state object
  // copies.
  std::shared_ptr<std::unordered_map<RubikAction::RubikActionId, std::shared_ptr<const RubikAction> > > rubik_action_index_ptr_;
  std::shared_ptr<std::unordered_map<RubikState::RubikStateValue, std::shared_ptr<const RubikState> > > rubik_state_index_ptr_;

  // Store all discovered search nodes.
  std::shared_ptr<std::unordered_map<RubikState::RubikStateValue, std::shared_ptr<RubikSearchNode> > > discovered_node_index_ptr_;

  // Returns a shared pointer reference to the state that
  // results from the given state and action input.
  std::shared_ptr<const RubikState>
  generateNextState(const std::shared_ptr<const RubikState>& orig_state_ptr,
		    const std::shared_ptr<const RubikAction>& action_ptr) {

    const RubikAction::RubikActionType action_type = action_ptr->getType();
    const RubikAction::RubikActionDirection action_direct = action_ptr->getDirection();
    const RubikAction::RubikActionIndex action_index = action_ptr->getIndex();

    RubikState::RubikStateValue new_state_value;

    if(action_index >= cardinality_) return orig_state_ptr;

    switch(action_direct) {
    case RubikAction::RubikActionDirection::NONE:
      return orig_state_ptr;
      break;
    default:
      switch(action_type) {
      case RubikAction::RubikActionType::ROTATE_XY:
	new_state_value = rotateXY(orig_state_ptr->getValue(),
				   action_direct, action_index);
	break;
      case RubikAction::RubikActionType::ROTATE_YZ:
	new_state_value = rotateYZ(orig_state_ptr->getValue(),
				   action_direct, action_index);
	break;
      case RubikAction::RubikActionType::ROTATE_ZX:
	new_state_value = rotateZX(orig_state_ptr->getValue(),
				   action_direct, action_index);
	break;
      default:
	return orig_state_ptr;
	break;
      }
      break;
    }

    // Add new state to index if not already.
    auto state_index_iter = rubik_state_index_ptr_->find(new_state_value);

    if(state_index_iter == rubik_state_index_ptr_->end()) {
      std::shared_ptr<const RubikState> new_state_ptr(new RubikState(new_state_value));
      rubik_state_index_ptr_->emplace(new_state_value,
				      new_state_ptr);
      return new_state_ptr;
    }

    return state_index_iter->second;
  }

  // Rotates a given "row" parallel to the XY plane.
  // Row indexing runs 0 to cardinality - 1, top to bottom.
  RubikState::RubikStateValue
  rotateXY(const RubikState::RubikStateValue& orig_state_value,
	   const RubikAction::RubikActionDirection& action_direct,
	   const RubikAction::RubikActionIndex& action_index) {

    const int* face_list = face_list_xy;

    int num_colors_per_face = cardinality_ * cardinality_;

    int floor_cur, floor_prev;

    std::queue<char> color_store;

    RubikState::RubikStateValue new_state_value = orig_state_value;

    // Shift rows and columns around relevant cube faces.
    if(action_direct == RubikAction::RubikActionDirection::CW) {
      floor_cur = face_list[FACE_LIST_SIZE-1] * num_colors_per_face
	+ action_index * cardinality_;

      for(unsigned int i = 0; i < cardinality_; ++i)
	color_store.push(new_state_value[floor_cur+i]);

      for(unsigned int f = FACE_LIST_SIZE - 2; f > 0; --f) {
	floor_prev = floor_cur;
	floor_cur = face_list[f] * num_colors_per_face
	  + action_index * cardinality_;

	for(unsigned int i = 0; i < cardinality_; ++i)
	  new_state_value[floor_prev+i] = new_state_value[floor_cur+i];
      }

      floor_prev = floor_cur;

      for(unsigned int i = 0; i < cardinality_; ++i) {
	new_state_value[floor_prev+i] = color_store.front();
	color_store.pop();
      }

      // Remember to rotate relevant faces as well.
      if(action_index == cardinality_ - 1)
	new_state_value.replace(3 * num_colors_per_face,
				num_colors_per_face,
				rotateFace(new_state_value.substr(3 * num_colors_per_face,
								  num_colors_per_face),
					   RubikAction::RubikActionDirection::CCW));
    }
    else if(action_direct == RubikAction::RubikActionDirection::CCW) {
      floor_cur = face_list[0] * num_colors_per_face
	+ action_index * cardinality_;

      for(unsigned int i = 0; i < cardinality_; ++i)
	color_store.push(new_state_value[floor_cur+i]);

      for(unsigned int f = 1; f < FACE_LIST_SIZE - 1; ++f) {
	floor_prev = floor_cur;
	floor_cur = face_list[f] * num_colors_per_face
	  + action_index * cardinality_;

	for(unsigned int i = 0; i < cardinality_; ++i)
	  new_state_value[floor_prev+i] = new_state_value[floor_cur+i];
      }

      floor_prev = floor_cur;

      for(unsigned int i = 0; i < cardinality_; ++i) {
	new_state_value[floor_prev+i] = color_store.front();
	color_store.pop();
      }

      // Remember to rotate relevant faces as well.
      if(action_index == cardinality_ - 1)
	new_state_value.replace(3 * num_colors_per_face,
				num_colors_per_face,
				rotateFace(new_state_value.substr(3 * num_colors_per_face,
								  num_colors_per_face),
					   RubikAction::RubikActionDirection::CW));
    }

    // Remember to rotate relevant faces as well.
    if(action_index == 0)
      new_state_value.replace(2 * num_colors_per_face,
			      num_colors_per_face,
			      rotateFace(new_state_value.substr(2 * num_colors_per_face,
								num_colors_per_face),
					 action_direct));

    return new_state_value;
  }

  // Rotates a given "rank" parallel to the YZ plane.
  // Rank indexing runs 0 to cardinality - 1, front to back.
  RubikState::RubikStateValue
  rotateYZ(const RubikState::RubikStateValue& orig_state_value,
	   const RubikAction::RubikActionDirection& action_direct,
	   const RubikAction::RubikActionIndex& action_index) {

    const int* face_list = face_list_yz;

    unsigned int num_colors_per_face = cardinality_ * cardinality_;

    int floor_cur, floor_prev;

    std::queue<char> color_store;

    RubikState::RubikStateValue new_state_value = orig_state_value;

    // Shift rows and columns around relevant cube faces.
    if(action_direct == RubikAction::RubikActionDirection::CW) {
      floor_cur = face_list[FACE_LIST_SIZE-1] * num_colors_per_face
	+ action_index;

      for(unsigned int i = 0; i < num_colors_per_face; i += cardinality_)
	color_store.push(new_state_value[floor_cur+i]);

      for(unsigned int face_cur = FACE_LIST_SIZE - 2;
	  face_cur > 0;
	  --face_cur) {

	unsigned int face_prev = face_cur + 1;

	floor_prev = floor_cur;

	if(face_list[face_prev] == 1 && face_list[face_cur] == 2) {
	  floor_cur = face_list[face_cur] * num_colors_per_face
	    + (cardinality_ - 1 - action_index) * cardinality_;

	  for(unsigned int i = 0; i < cardinality_; ++i)
	    new_state_value[floor_prev+i*cardinality_]
	      = new_state_value[floor_cur+i];
	}
	else if(face_list[face_prev] == 2 && face_list[face_cur] == 4) {
	  floor_cur = face_list[face_cur] * num_colors_per_face
	    + (cardinality_ - 1 - action_index);

	  for(unsigned int i = 0; i < cardinality_; ++i)
	    new_state_value[floor_prev+i]
	      = new_state_value[floor_cur+(cardinality_-1-i)*cardinality_];
	}
	else if(face_list[face_prev] == 4 && face_list[face_cur] == 3) {
	  floor_cur = face_list[face_cur] * num_colors_per_face
	    + action_index * cardinality_;

	  for(unsigned int i = 0; i < cardinality_; ++i)
	    new_state_value[floor_prev+i*cardinality_]
	      = new_state_value[floor_cur+i];
	}
      }

      floor_prev = floor_cur;

      for(unsigned int i = 0; i < cardinality_; ++i) {
	new_state_value[floor_prev+cardinality_-1-i] = color_store.front();
	color_store.pop();
      }

      // Remember to rotate relevant faces as well.
      if(action_index == cardinality_ - 1)
	new_state_value.replace(5 * num_colors_per_face,
				num_colors_per_face,
				rotateFace(new_state_value.substr(5 * num_colors_per_face,
								  num_colors_per_face),
					   RubikAction::RubikActionDirection::CCW));
    }
    else if(action_direct == RubikAction::RubikActionDirection::CCW) {
      floor_cur = face_list[0] * num_colors_per_face + action_index;

      for(unsigned int i = 0; i < num_colors_per_face; i += cardinality_)
	color_store.push(new_state_value[floor_cur+i]);

      for(unsigned int face_cur = 1; face_cur < FACE_LIST_SIZE - 1; ++face_cur) {
	unsigned int face_prev = face_cur - 1;

	floor_prev = floor_cur;

	if(face_list[face_prev] == 1 && face_list[face_cur] == 3) {
	  floor_cur = face_list[face_cur] * num_colors_per_face
	    + action_index * cardinality_;

	  for(unsigned int i = 0; i < cardinality_; ++i)
	    new_state_value[floor_prev+i*cardinality_]
	      = new_state_value[floor_cur+cardinality_-1-i];
	}
	else if(face_list[face_prev] == 3 && face_list[face_cur] == 4) {
	  floor_cur = face_list[face_cur] * num_colors_per_face
	    + (cardinality_ - 1 - action_index);

	  for(unsigned int i = 0; i < cardinality_; ++i)
	    new_state_value[floor_prev+i]
	      = new_state_value[floor_cur+i*cardinality_];
	}
	else if(face_list[face_prev] == 4 && face_list[face_cur] == 2) {
	  floor_cur = face_list[face_cur] * num_colors_per_face
	    + (cardinality_ - 1 - action_index) * cardinality_;

	  for(unsigned int i = 0; i < cardinality_; ++i)
	    new_state_value[floor_prev+(cardinality_-1-i)*cardinality_]
	      = new_state_value[floor_cur+i];
	}
      }

      floor_prev = floor_cur;

      for(unsigned int i = 0; i < cardinality_; ++i) {
	new_state_value[floor_prev+i] = color_store.front();
	color_store.pop();
      }

      // Remember to rotate relevant faces as well.
      if(action_index == cardinality_ - 1)
	new_state_value.replace(5 * num_colors_per_face,
				num_colors_per_face,
				rotateFace(new_state_value.substr(5 * num_colors_per_face,
								  num_colors_per_face),
					   RubikAction::RubikActionDirection::CW));
    }

    // Remember to rotate relevant faces as well.
    if(action_index == 0)
      new_state_value.replace(0 * num_colors_per_face,
			      num_colors_per_face,
			      rotateFace(new_state_value.substr(0 * num_colors_per_face,
								num_colors_per_face),
					 action_direct));

    return new_state_value;
  }

  // Rotates a given "column" parallel to the ZX plane.
  // Column indexing runs 0 to cardinality - 1, left to right.
  RubikState::RubikStateValue
  rotateZX(const RubikState::RubikStateValue& orig_state_value,
	   const RubikAction::RubikActionDirection& action_direct,
	   const RubikAction::RubikActionIndex& action_index) {

    const int* face_list = face_list_zx;

    unsigned int num_colors_per_face = cardinality_ * cardinality_;

    int floor_cur, floor_prev;

    std::queue<char> color_store;

    RubikState::RubikStateValue new_state_value = orig_state_value;

    // Shift rows and columns around relevant cube faces.
    if(action_direct == RubikAction::RubikActionDirection::CW) {
      floor_cur = face_list[FACE_LIST_SIZE-1] * num_colors_per_face
	+ action_index;

      for(unsigned int i = 0; i < num_colors_per_face; i += cardinality_)
	color_store.push(new_state_value[floor_cur+i]);

      for(unsigned int face_cur = FACE_LIST_SIZE - 2;
	  face_cur > 0;
	  --face_cur) {

	unsigned int face_prev = face_cur + 1;

	floor_prev = floor_cur;

	if(face_list[face_prev] == 0 && face_list[face_cur] == 3) {
	  floor_cur = face_list[face_cur] * num_colors_per_face
	    + action_index;

	  for(unsigned int i = 0; i < cardinality_; ++i)
	    new_state_value[floor_prev+i*cardinality_]
	      = new_state_value[floor_cur+i*cardinality_];
	}
	else if(face_list[face_prev] == 3 && face_list[face_cur] == 5) {
	  floor_cur = face_list[face_cur] * num_colors_per_face
	    + (cardinality_ - 1 - action_index);

	  for(unsigned int i = 0; i < cardinality_; ++i)
	    new_state_value[floor_prev+i*cardinality_]
	      = new_state_value[floor_cur+(cardinality_-1-i)*cardinality_];
	}
	else if(face_list[face_prev] == 5 && face_list[face_cur] == 2) {
	  floor_cur = face_list[face_cur] * num_colors_per_face
	    + action_index;

	  for(unsigned int i = 0; i < cardinality_; ++i)
	    new_state_value[floor_prev+(cardinality_-1-i)*cardinality_]
	      = new_state_value[floor_cur+i*cardinality_];
	}
      }

      floor_prev = floor_cur;

      for(unsigned int i = 0; i < cardinality_; ++i) {
	new_state_value[floor_prev+i*cardinality_] = color_store.front();
	color_store.pop();
      }

      // Remember to rotate relevant faces as well.
      if(action_index == 0)
	new_state_value.replace(4 * num_colors_per_face,
				num_colors_per_face,
				rotateFace(new_state_value.substr(4 * num_colors_per_face,
								  num_colors_per_face),
					   RubikAction::RubikActionDirection::CCW));
    }
    else if(action_direct == RubikAction::RubikActionDirection::CCW) {
      floor_cur = face_list[0] * num_colors_per_face + action_index;

      for(unsigned int i = 0; i < num_colors_per_face; i += cardinality_)
	color_store.push(new_state_value[floor_cur+i]);

      for(unsigned int face_cur = 1;
	  face_cur < FACE_LIST_SIZE - 1;
	  ++face_cur) {

	unsigned int face_prev = face_cur - 1;

	floor_prev = floor_cur;

	if(face_list[face_prev] == 0 && face_list[face_cur] == 2) {
	  floor_cur = face_list[face_cur] * num_colors_per_face + action_index;

	  for(unsigned int i = 0; i < cardinality_; ++i)
	    new_state_value[floor_prev+i*cardinality_]
	      = new_state_value[floor_cur+i*cardinality_];
	}
	else if(face_list[face_prev] == 2 && face_list[face_cur] == 5) {
	  floor_cur = face_list[face_cur] * num_colors_per_face
	    + (cardinality_ - 1 - action_index);

	  for(unsigned int i = 0; i < cardinality_; ++i)
	    new_state_value[floor_prev+i*cardinality_]
	      = new_state_value[floor_cur+(cardinality_-1-i)*cardinality_];
	}
	else if(face_list[face_prev] == 5 && face_list[face_cur] == 3) {
	  floor_cur = face_list[face_cur] * num_colors_per_face + action_index;

	  for(unsigned int i = 0; i < cardinality_; ++i)
	    new_state_value[floor_prev+(cardinality_-1-i)*cardinality_]
	      = new_state_value[floor_cur+i*cardinality_];
	}
      }

      floor_prev = floor_cur;

      for(unsigned int i = 0; i < cardinality_; ++i) {
	new_state_value[floor_prev+i*cardinality_] = color_store.front();
	color_store.pop();
      }

      // Remember to rotate relevant faces as well.
      if(action_index == 0)
	new_state_value.replace(4 * num_colors_per_face,
				num_colors_per_face,
				rotateFace(new_state_value.substr(4 * num_colors_per_face,
								  num_colors_per_face),
					   RubikAction::RubikActionDirection::CW));
    }

    // Remember to rotate relevant faces as well.
    if(action_index == cardinality_ - 1)
	new_state_value.replace(1 * num_colors_per_face,
				num_colors_per_face,
				rotateFace(new_state_value.substr(1 * num_colors_per_face,
								  num_colors_per_face),
					   action_direct));

    return new_state_value;
  }

  // Returns the rotated contents (about its center) of a given face.
  RubikState::RubikStateValue
  rotateFace(const RubikState::RubikStateValue& face,
	     const RubikAction::RubikActionDirection& direct) {

    RubikState::RubikStateValue new_face = face;

    int num_rotation_layers = cardinality_ / 2;

    RubikState::RubikStateValue top_row, bottom_row, left_col, right_col;

    // Work from the outside border inwards.
    for(int layer = 0; layer < num_rotation_layers; ++layer) {
      // Top row.
      int top_row_floor = layer * (cardinality_ + 1);
      int top_row_ceiling = top_row_floor + cardinality_ - 2 * layer;

      top_row = face.substr(top_row_floor, top_row_ceiling - top_row_floor);

      // Bottom row.
      int bottom_row_floor = cardinality_ * (cardinality_ - 1)
	- layer * (cardinality_ - 1);
      int bottom_row_ceiling = bottom_row_floor + cardinality_ - 2 * layer;

      bottom_row = face.substr(bottom_row_floor,
			       bottom_row_ceiling - bottom_row_floor);

      // Left column.
      int left_col_floor = top_row_floor;
      int left_col_ceiling = left_col_floor
	+ cardinality_ * (cardinality_ - 2 * layer - 1) + 1;
      for(int i = left_col_floor; i < left_col_ceiling; i += cardinality_)
	left_col.append(1, face[i]);

      // Right column.
      int right_col_floor = cardinality_ * (layer + 1) - layer - 1;
      int right_col_ceiling = right_col_floor
	+ cardinality_ * (cardinality_ - 2 * layer - 1) + 1;
      for(int i = right_col_floor; i < right_col_ceiling; i += cardinality_)
	right_col.append(1, face[i]);

      if(direct == RubikAction::RubikActionDirection::CW) {
	// Reverse left column.
	std::reverse(left_col.begin(), left_col.end());

	// Reverse right column.
	std::reverse(right_col.begin(), right_col.end());

	// Replace top row with left column.
	new_face.replace(top_row_floor, top_row_ceiling - top_row_floor, left_col);

	// Replace bottom row with right column.
	new_face.replace(bottom_row_floor,
			 bottom_row_ceiling - bottom_row_floor,
			 right_col);

	// Replace left column with bottom row.
	int j = 0;
	for(int i = left_col_floor; i < left_col_ceiling; i += cardinality_)
	  new_face[i] = bottom_row[j++];

	// Replace right column with top row.
	j = 0;
	for(int i = right_col_floor; i < right_col_ceiling; i += cardinality_)
	  new_face[i] = top_row[j++];
      }
      else if(direct == RubikAction::RubikActionDirection::CCW) {
	// Reverse top row.
	std::reverse(top_row.begin(), top_row.end());

	// Reverse bottom row.
	std::reverse(bottom_row.begin(), bottom_row.end());

	// Replace top row with right column.
	new_face.replace(top_row_floor, top_row_ceiling - top_row_floor, right_col);

	// Replace bottom row with left column.
	new_face.replace(bottom_row_floor,
			 bottom_row_ceiling - bottom_row_floor,
			 left_col);

	// Replace left column with top row.
	int j = 0;
	for(int i = left_col_floor; i < left_col_ceiling; i += cardinality_)
	  new_face[i] = top_row[j++];

	// Replace right column with bottom row.
	j = 0;
	for(int i = right_col_floor; i < right_col_ceiling; i += cardinality_)
	  new_face[i] = bottom_row[j++];
      }

      top_row.clear();
      bottom_row.clear();
      left_col.clear();
      right_col.clear();
    }

    return new_face;
  }
};

#endif // __RUBIKPROBLEM_HPP__
