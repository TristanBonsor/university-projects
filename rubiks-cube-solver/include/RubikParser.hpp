////////////////////////////////////////////////////////////////////////////////
// RubikParser.hpp
// Copyright (C) 2013  Tristan W. Bonsor
//
// RubikParser class declaration.
//
// EBNF For Input Files
// ====================
// RubikCube = Cardinality , NewLine , Faces ;
// Cardinality = "2" | "3" | "4" ;
// NewLine = "\n" ;
// Faces = 6 * { Face } ;
// Face = FaceId , NewLine , Cardinality * { Row } ;
// FaceId = "FRONT" | "RIGHT" | "TOP" | "BOTTOM" | "LEFT" | "BACK" ;
// Row = Cardinality * { Color } , NewLine ;
// Color = "W" | "R" | "B" | "O" | "G" | "Y" ;
//
////////////////////////////////////////////////////////////////////////////////
#ifndef __RUBIKPARSER_HPP__
#define __RUBIKPARSER_HPP__

#include <memory>
#include <string>
#include <unordered_map>

#include "RubikDefines.hpp"
#include "RubikScanner.hpp"

// Parses tokens generated by RubikScanner::scan().
class RubikParser {
public:
  RubikParser()
    : cardinal_(0)
    , current_line_num_(0)
    , is_error_(false)
    , error_msg_()
    , face_index_()
    , color_count_index_({
	{RubikColor::WHITE, 0},
	  {RubikColor::RED, 0},
	    {RubikColor::BLUE, 0},
	      {RubikColor::ORANGE, 0},
		{RubikColor::GREEN, 0},
		  {RubikColor::YELLOW, 0}}) {}

  // Returns an ordered pair containing the cardinality value of the cube (ie.
  // 3 for 3x3x3) and a shared pointer reference to the initial state of the
  // Rubik's cube.
  std::pair<unsigned int, std::shared_ptr<const RubikState> >
  parse(std::queue<std::tuple<RubikScanner::TokenType, char, unsigned int> >& tokens);

  // Returns true if an error was encountered parsing the input file.
  bool isError() const { return is_error_; }

  // Returns a description of the last parsing error that occured.
  std::string getErrorMessage() const { return error_msg_; }

private:
  static const unsigned int MAX_NUM_FACES;

  static const unsigned int MIN_CARDINAL;
  static const unsigned int MAX_CARDINAL;

  static const std::string FRONT_FACE;
  static const std::string RIGHT_FACE;
  static const std::string TOP_FACE;
  static const std::string BOTTOM_FACE;
  static const std::string LEFT_FACE;
  static const std::string BACK_FACE;

  // Parse cube cardinality.
  void getCardinality(std::queue<std::tuple<RubikScanner::TokenType, char, unsigned int> >& tokens);

  // Parse new line.
  void getNewLine(std::queue<std::tuple<RubikScanner::TokenType, char, unsigned int> >& tokens);

  // Parse all cube faces.
  void getFaces(std::queue<std::tuple<RubikScanner::TokenType, char, unsigned int> >& tokens);

  // Parse an individual cube face.
  void getFace(std::queue<std::tuple<RubikScanner::TokenType, char, unsigned int> >& tokens);

  // Parse the id of the face ("FRONT", "LEFT", etc.).
  std::string getFaceId(std::queue<std::tuple<RubikScanner::TokenType, char, unsigned int> >& tokens);

  // Parse a row of the cube face.
  std::string getRow(std::queue<std::tuple<RubikScanner::TokenType, char, unsigned int> >& tokens);

  // Parse a color in a row on a cube face.
  char getColor(std::queue<std::tuple<RubikScanner::TokenType, char, unsigned int> >& tokens);

  // Ensure there are not two differing instances of the same face.
  void validateFaceId(const std::string& face_id);

  unsigned int cardinal_;
  unsigned int current_line_num_;
  bool is_error_;
  std::string error_msg_;
  std::unordered_map<std::string, std::string> face_index_;
  std::unordered_map<char, unsigned int> color_count_index_;
};

#endif // __RUBIKPARSER_HPP__
