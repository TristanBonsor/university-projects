////////////////////////////////////////////////////////////////////////////////
// RubikPathCost.hpp
// Copyright (C) 2013  Tristan W. Bonsor
//
// RubikPathCost class declaration.
////////////////////////////////////////////////////////////////////////////////
#ifndef __RUBIKPATHCOST_HPP__
#define __RUBIKPATHCOST_HPP__

// Stores different path costs: action_cost_ reflects the cost of all edges
// traversed in the search graph while heuristic_cost_ reflects the prospective
// cost of reaching a goal state.
template<class G, class H>
class RubikPathCost {
public:
  // Constructs a "null" path cost.
  RubikPathCost()
    : action_cost_()
    , heuristic_cost_()
    , is_valid_cost_(false) {}

  RubikPathCost(const G& action_cost, const H& heuristic_cost)
    : action_cost_(action_cost)
    , heuristic_cost_(heuristic_cost)
    , is_valid_cost_(true) {}

  RubikPathCost<G, H> operator+(const RubikPathCost<G, H>& rhs) const {
    if(!is_valid_cost_ || !rhs.is_valid_cost_)
      return RubikPathCost<G, H>();
    return RubikPathCost<G, H>(action_cost_ + rhs.action_cost_,
			       heuristic_cost_ + rhs.heuristic_cost_);
  }

  G getActionCost() const { return action_cost_; }

  H getHeuristicCost() const { return heuristic_cost_; }

  bool isValidCost() const { return is_valid_cost_; }

protected:
  G action_cost_;
  H heuristic_cost_;

  bool is_valid_cost_;
};

#endif // __RUBIKPATHCOST_HPP__
