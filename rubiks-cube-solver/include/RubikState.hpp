////////////////////////////////////////////////////////////////////////////////
// RubikState.hpp
// Copyright (C) 2013  Tristan W. Bonsor
//
// RubikState class declaration.
////////////////////////////////////////////////////////////////////////////////
#ifndef __RUBIKSTATE_HPP__
#define __RUBIKSTATE_HPP__

#include <string>

class RubikState {
public:
  typedef std::string RubikStateValue;

  RubikState(const RubikStateValue& value)
    : value_(value) {}

  RubikStateValue getValue() const {
    return value_;
  }

protected:
  std::string value_;
};

#endif // __RUBIKSTATE_HPP__
