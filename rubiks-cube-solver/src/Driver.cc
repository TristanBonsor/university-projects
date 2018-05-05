#include <fstream>
#include <iostream>

#include "RubikAgent.hpp"
#include "RubikParser.hpp"
#include "RubikProblem.hpp"

int main() {
  std::fstream fin;
  fin.open("test/test.txt", std::fstream::in);

  std::queue<std::tuple<RubikScanner::TokenType, char, unsigned int> > tokens = RubikScanner::scan(fin);

  fin.close();

  RubikParser parser;

  std::pair<unsigned int, std::shared_ptr<const RubikState> > parse_result = parser.parse(tokens);

  if(parser.isError()) {
    std::cout << parser.getErrorMessage();
    return 1;
  }

  ActionHeuristicCost path_cost(0, 0);

  std::shared_ptr<const RubikAction> action_ptr(new RubikAction);

  unsigned int cardinality = std::get<0>(parse_result);
  std::shared_ptr<const RubikState> initial_state = std::get<1>(parse_result);

  std::shared_ptr<RubikSearchNode> search_node(new RubikSearchNode(std::shared_ptr<RubikSearchNode>(nullptr),
								   action_ptr,
								   initial_state,
								   path_cost,
								   0));

  RubikProblem problem(initial_state, cardinality);

  std::cout << "Cardinality: " << cardinality << std::endl;

  RubikAgent agent(std::cout);

  auto solution = agent.solve(problem, true, 0);

  for(auto it = solution.rbegin(); it != solution.rend(); ++it) {
    std::cout << "****************************************\n";

    auto action_ptr = it->getAction();
    auto state_ptr  = it->getState();
    switch(action_ptr->getType()) {
    case RubikAction::RubikActionType::START:
      std::cout << "Action:         " << "Start\n";
      break;
    case RubikAction::RubikActionType::ROTATE_XY:
      std::cout << "Action:         " << "Rotate row " << action_ptr->getIndex() << "\n";
      break;
    case RubikAction::RubikActionType::ROTATE_YZ:
      std::cout << "Action:         " << "Rotate rank " << action_ptr->getIndex() << "\n";
      break;
    case RubikAction::RubikActionType::ROTATE_ZX:
      std::cout << "Action:         " << "Rotate column " << action_ptr->getIndex() << "\n";
      break;
    default:
      std::cout << "Action:         " << "None\n";
      break;
    }

    switch(action_ptr->getDirection()) {
    case RubikAction::RubikActionDirection::CW:
      std::cout << "Direction:      " << "Clockwise\n";
      break;
    case RubikAction::RubikActionDirection::CCW:
      std::cout << "Direction:      " << "Counter-Clockwise\n";
      break;
    default:
      std::cout << "Direction:      " << "None\n";
    }

    std::cout << "Value:          " << state_ptr->getValue() << "\n";
  }

  return 0;
}
