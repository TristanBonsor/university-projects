////////////////////////////////////////////////////////////////////////////////
// RubikDefines.hpp
// Copyright (C) 2013  Tristan W. Bonsor
//
// Convenience header.
////////////////////////////////////////////////////////////////////////////////
#ifndef __RUBIKDEFINES_HPP__
#define __RUBIKDEFINES_HPP__

#include "RubikAction.hpp"
#include "RubikPathCost.hpp"
#include "RubikHeuristic.hpp"

typedef RubikPathCost<RubikAction::RubikActionCost, RubikHeuristic::RubikHeuristicCost> ActionHeuristicCost;

#endif // __RUBIKDEFINES_HPP__
