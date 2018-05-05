////////////////////////////////////////////////////////////////////////////////
// RubikScanner.hpp
// Copyright (C) 2013  Tristan W. Bonsor
//
// RubikScanner class declaration.
////////////////////////////////////////////////////////////////////////////////
#ifndef __RUBIKSCANNER_HPP__
#define __RUBIKSCANNER_HPP__

#include <istream>
#include <queue>
#include <tuple>

class RubikScanner {
public:
  enum TokenType : int { UNKNOWN, DIGIT, LETTER, NEWLINE };

  static std::queue<std::tuple<TokenType, char, unsigned int> >
  scan(std::istream& in);
};

#endif // __RUBIKSCANNER_HPP__
