////////////////////////////////////////////////////////////////////////////////
// RubikParser.cc
// Copyright (C) 2013  Tristan W. Bonsor
////////////////////////////////////////////////////////////////////////////////
#include "RubikParser.hpp"

const unsigned int RubikParser::MAX_NUM_FACES = 6;

const unsigned int RubikParser::MIN_CARDINAL = 2;
const unsigned int RubikParser::MAX_CARDINAL = 4;

const std::string RubikParser::FRONT_FACE   = "FRONT";
const std::string RubikParser::RIGHT_FACE   = "RIGHT";
const std::string RubikParser::TOP_FACE     = "TOP";
const std::string RubikParser::BOTTOM_FACE  = "BOTTOM";
const std::string RubikParser::LEFT_FACE    = "LEFT";
const std::string RubikParser::BACK_FACE    = "BACK";

std::pair<unsigned int, std::shared_ptr<const RubikState> > RubikParser::parse(std::queue<std::tuple<RubikScanner::TokenType, char, unsigned int> >& tokens) {

  if(tokens.empty()) {
    is_error_ = true;
    error_msg_ = "Parser error: " + std::to_string(current_line_num_) + ": ";
    error_msg_.append("Empty input stream\n");
    return std::pair<unsigned int, std::shared_ptr<RubikState> >(cardinal_,
								 std::shared_ptr<RubikState>(nullptr));
  }

  ++current_line_num_;

  getCardinality(tokens);
  getNewLine(tokens);

  if(is_error_)
    return std::pair<unsigned int, std::shared_ptr<const RubikState> >(cardinal_,
								       std::shared_ptr<RubikState>(nullptr));

  getFaces(tokens);

  if(is_error_)
    return std::pair<unsigned int, std::shared_ptr<const RubikState> >(cardinal_,
								       std::shared_ptr<RubikState>(nullptr));

  RubikState::RubikStateValue state_value(face_index_.at(FRONT_FACE));
  state_value.append(face_index_.at(RIGHT_FACE));
  state_value.append(face_index_.at(TOP_FACE));
  state_value.append(face_index_.at(BOTTOM_FACE));
  state_value.append(face_index_.at(LEFT_FACE));
  state_value.append(face_index_.at(BACK_FACE));

  return std::pair<unsigned int, std::shared_ptr<const RubikState> >(cardinal_,
								     std::shared_ptr<RubikState>(new RubikState(state_value)));
}

void RubikParser::getCardinality(std::queue<std::tuple<RubikScanner::TokenType, char, unsigned int> >& tokens) {

  if(tokens.empty()) {
    is_error_ = true;
    error_msg_ = "Parser error: " + std::to_string(current_line_num_) + ": ";
    error_msg_.append("Failed reading cardinality: End of input\n");
    return;
  }

  RubikScanner::TokenType token_type;
  char symbol;
  
  std::tie(token_type, symbol, current_line_num_) = tokens.front();

  switch(token_type) {
  case RubikScanner::DIGIT:
    cardinal_ = symbol - '0';

    if(cardinal_ < MIN_CARDINAL || cardinal_ > MAX_CARDINAL) {
      is_error_ = true;
      error_msg_ = "Parser error: " + std::to_string(current_line_num_) + ": ";
      error_msg_.append("Invalid cardinality: Valid interval is [");
      error_msg_.append(std::to_string(MIN_CARDINAL) + ",");
      error_msg_.append(std::to_string(MAX_CARDINAL) + "]\n");
      break;
    }

    tokens.pop();
    break;

  default:
    is_error_ = true;
    error_msg_ = "Parser error: " + std::to_string(current_line_num_) + ": ";
    error_msg_.append("Invalid symbol \"");
    error_msg_.append(1, symbol);
    error_msg_.append("\": Expected a value within the interval [");
    error_msg_.append(std::to_string(MIN_CARDINAL) + ",");
    error_msg_.append(std::to_string(MAX_CARDINAL) + "]\n");
    break;
  }
}

void RubikParser::getNewLine(std::queue<std::tuple<RubikScanner::TokenType, char, unsigned int> >& tokens) {

  if(tokens.empty()) {
    is_error_ = true;
    error_msg_ = "Parser error: " + std::to_string(current_line_num_) + ": ";
    error_msg_.append("Failed reading \"\\n\": End of input\n");
    return;
  }

  RubikScanner::TokenType token_type;
  char symbol;
  
  std::tie(token_type, symbol, current_line_num_) = tokens.front();

  switch(token_type) {
  case RubikScanner::NEWLINE:
    tokens.pop();
    break;

  default:
    is_error_ = true;
    error_msg_ = "Parser error: " + std::to_string(current_line_num_) + ": ";
    error_msg_.append("Invalid symbol \"");
    error_msg_.append(1, symbol);
    error_msg_.append("\": Expected \"\\n\"\n");
    break;
  }
}

void RubikParser::getFaces(std::queue<std::tuple<RubikScanner::TokenType, char, unsigned int> >& tokens) {
  for(unsigned int i = 0; i < MAX_NUM_FACES; ++i) {
    getFace(tokens);
    if(is_error_) return;
  }
}

void RubikParser::getFace(std::queue<std::tuple<RubikScanner::TokenType, char, unsigned int> >& tokens) {

  std::string face_id = getFaceId(tokens);

  if(is_error_) return;

  getNewLine(tokens);

  if(is_error_) return;

  std::string face;
  for(unsigned int i = 0; i < cardinal_; ++i) {
    face.append(getRow(tokens));
    if(is_error_) return;
  }

  face_index_.insert(std::make_pair(face_id, face));
}

std::string RubikParser::getFaceId(std::queue<std::tuple<RubikScanner::TokenType, char, unsigned int> >& tokens) {
  std::string face_id;

  if(tokens.empty()) {
    is_error_ = true;
    error_msg_ = "Parser error: " + std::to_string(current_line_num_) + ": ";
    error_msg_.append("Failed reading face ID: End of input\n");
    return face_id;
  }

  RubikScanner::TokenType token_type;
  char symbol;

  std::tie(token_type, symbol, current_line_num_) = tokens.front();

  if(token_type != RubikScanner::LETTER) {
    is_error_ = true;
    error_msg_ = "Parser error: " + std::to_string(current_line_num_) + ": ";
    error_msg_ += "Invalid symbol \"" + symbol;
    error_msg_ += "\": Expected alphabetical letter\n";
    return face_id;
  }

  do {
    face_id.append(1, symbol);
    tokens.pop();

    if(tokens.empty()) {
      is_error_ = true;
      error_msg_ = "Parser error: " + std::to_string(current_line_num_) + ": ";
      error_msg_.append("Failed reading face ID: End of input\n");
      return face_id;
    }
    std::tie(token_type, symbol, current_line_num_) = tokens.front();
  }while(token_type == RubikScanner::LETTER);

  for(unsigned int i = 0; i < face_id.size(); ++i)
    face_id[i] = toupper(face_id[i]);

  validateFaceId(face_id);

  return face_id;
}

std::string RubikParser::getRow(std::queue<std::tuple<RubikScanner::TokenType, char, unsigned int> >& tokens) {
  std::string row;

  if(tokens.empty()) {
    is_error_ = true;
    error_msg_ = "Parser error: " + std::to_string(current_line_num_) + ": ";
    error_msg_.append("Failed reading row: End of input\n");
    return row;
  }

  for(unsigned int i = 0; i < cardinal_; ++i) {
    row.append(1, getColor(tokens));
    if(is_error_) return row;
  }

  getNewLine(tokens);

  return row;
}

char RubikParser::getColor(std::queue<std::tuple<RubikScanner::TokenType, char, unsigned int> >& tokens) {
  if(tokens.empty()) {
    is_error_ = true;
    error_msg_ = "Parser error: " + std::to_string(current_line_num_) + ": ";
    error_msg_.append("Failed reading color: End of input\n");
    return '\0';
  }

  RubikScanner::TokenType token_type;
  char symbol;

  std::tie(token_type, symbol, current_line_num_) = tokens.front();

  if(token_type != RubikScanner::LETTER) {
    is_error_ = true;
    error_msg_ = "Parser error: " + std::to_string(current_line_num_) + ": ";
    error_msg_.append("Invalid symbol \"");
    if(token_type == RubikScanner::NEWLINE)
      error_msg_.append("\\n");
    else
      error_msg_.append(1, symbol);
    error_msg_.append("\": Expected alphabetical letter\n");
    return symbol;
  }

  symbol = toupper(symbol);

  if(symbol == RubikColor::WHITE  ||
     symbol == RubikColor::RED    ||
     symbol == RubikColor::BLUE   ||
     symbol == RubikColor::ORANGE ||
     symbol == RubikColor::GREEN  ||
     symbol == RubikColor::YELLOW) {
    ++color_count_index_[symbol];
  }
  else {
    is_error_ = true;
    error_msg_ = "Parser error: " + std::to_string(current_line_num_) + ": ";
    error_msg_.append("Invalid color \"");
    error_msg_.append(1, symbol);
    error_msg_.append("\": Expected one of \"");
    error_msg_.append(1, RubikColor::WHITE);
    error_msg_.append("\", \"");
    error_msg_.append(1, RubikColor::RED);
    error_msg_.append("\", \"");
    error_msg_.append(1, RubikColor::BLUE);
    error_msg_.append("\", \"");
    error_msg_.append(1, RubikColor::ORANGE);
    error_msg_.append("\", \"");
    error_msg_.append(1, RubikColor::GREEN);
    error_msg_.append("\", or \"");
    error_msg_.append(1, RubikColor::YELLOW);
    error_msg_.append("\"\n");
  }

  if(!is_error_) {
    unsigned int max = cardinal_ * cardinal_;
    if(color_count_index_.at(symbol) > max) {
      is_error_ = true;
      error_msg_ = "Parser error: " + std::to_string(current_line_num_) + ": ";
      error_msg_.append("Invalid color count: Color \"");
      error_msg_.append(1, symbol);
      error_msg_.append("\" occurs more than " + std::to_string(max) + " times: ");
      error_msg_.append("A valid cube has exactly " + std::to_string(max));
      error_msg_.append(" of each color\n");
    }
    else
      tokens.pop();
  }

  return symbol;
}

void RubikParser::validateFaceId(const std::string& face_id) {
  if(face_index_.find(face_id) == face_index_.end()) {
    if(face_id != FRONT_FACE  &&
       face_id != RIGHT_FACE  &&
       face_id != TOP_FACE    &&
       face_id != BOTTOM_FACE &&
       face_id != LEFT_FACE   &&
       face_id != BACK_FACE) {
      is_error_ = true;
      error_msg_ = "Parser error: " + std::to_string(current_line_num_) + ": ";
      error_msg_.append("Invalid face ID \"" + face_id);
      error_msg_.append("\": Expected one of \"" + FRONT_FACE + "\", \"");
      error_msg_.append(RIGHT_FACE + "\", \"" + TOP_FACE + "\", \"");
      error_msg_.append( BOTTOM_FACE + "\", \"" + LEFT_FACE + "\",");
      error_msg_.append(" or \"" + RIGHT_FACE + "\"\n");
    }
  }
  else {
    is_error_ = true;
    error_msg_ = "Parser error: " + std::to_string(current_line_num_) + ": ";
    error_msg_.append("Invalid face ID \"" + face_id + "\": Already in use\n");
  }
}
