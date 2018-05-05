////////////////////////////////////////////////////////////////////////////////
// RubikScanner.cc
// Copyright (C) 2013  Tristan W. Bonsor
////////////////////////////////////////////////////////////////////////////////
#include <cctype>

#include "RubikScanner.hpp"

static void insertToken(std::queue<std::tuple<RubikScanner::TokenType, char, unsigned int> >& tokens,
			const RubikScanner::TokenType& type,
			const char& c,
			const unsigned int& n);

std::queue<std::tuple<RubikScanner::TokenType, char, unsigned int> > RubikScanner::scan(std::istream& in) {

  std::queue<std::tuple<TokenType, char, unsigned int> > tokens;

  char c;
  unsigned int n = 1;

  in.get(c);
  while(in.good()) {
    if(isdigit(c)) insertToken(tokens, TokenType::DIGIT, c, n);
    else if(isalpha(c)) insertToken(tokens, TokenType::LETTER, c, n);
    else if(c == '\n') insertToken(tokens, TokenType::NEWLINE, c, n++);
    else if(!isspace(c)) insertToken(tokens, TokenType::UNKNOWN, c, n);
    in.get(c);
  }

  return tokens;
}

void insertToken(std::queue<std::tuple<RubikScanner::TokenType, char, unsigned int> >& tokens,
		 const RubikScanner::TokenType& type,
		 const char& c,
		 const unsigned int& n) {
  tokens.push(std::tuple<RubikScanner::TokenType, char, unsigned int>(type, c, n));
}
