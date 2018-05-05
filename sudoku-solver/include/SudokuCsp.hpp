////////////////////////////////////////////////////////////////////////////////
// SudokuCsp.hpp
// Copyright (C) 2013  Tristan W. Bonsor
//
// A sudoku constraint satisfaction problem.
////////////////////////////////////////////////////////////////////////////////
#ifndef __SUDOKUCSP_HPP__
#define __SUDOKUCSP_HPP__

#include "Csp.hpp"

class SudokuCsp : public Csp<unsigned int> {
public:

  SudokuCsp(const unsigned int& root)
  : Csp<unsigned int>(std::set<Variable<unsigned int> >(),
		      std::set<unsigned int>(),
		      std::set<Constraint<unsigned int> >())
  , root_(root)
  , side_length_() {

    side_length_ = root * root;

    unsigned int n = side_length_ * side_length_;

    for(unsigned int i = 0; i < n; ++i)
      variable_set_.insert(Variable<unsigned int>(i));

    for(unsigned int i = 1; i <= side_length_; ++i)
      value_set_.insert(i);

    constraint_set_.insert(BoxConstraint(root));
    constraint_set_.insert(ColumnConstraint(root));
    constraint_set_.insert(RowConstraint(root));
  }

  virtual ~SudokuCsp() {}

  // Returns the most constrained unassigned variable.
  virtual Variable<unsigned int>
  selectUnassignedVariable(const AssignmentSet<unsigned int>& assignment_set) const {

    std::set<Variable<unsigned int> > assignments = assignment_set.getCurrentAssignments();

    if(assignments.empty())
      return Variable<unsigned int>((side_length_ * side_length_) / 2);

    std::vector<unsigned int> box_assignment_counts(side_length_, 0),
      column_assignment_counts(side_length_, 0),
      row_assignment_counts(side_length_, 0);

    std::set<Variable<unsigned int> > available_variables = variable_set_;

    for(auto it = assignments.cbegin(); it != assignments.cend(); ++it) {
      available_variables.erase(Variable<unsigned int>(it->getEnumeration()));

      unsigned int ix = it->getEnumeration();
      unsigned int c = ix % side_length_;
      unsigned int r = (ix - c) / side_length_;
      unsigned int b = root_ * (r / root_) + (c / root_);

      ++box_assignment_counts[b];
      ++column_assignment_counts[c];
      ++row_assignment_counts[r];
    }

    unsigned int num_constraints = 0;
    unsigned int max_constraint_count = 0;
    Variable<unsigned int> most_constrained_variable(0);

    for(auto it = available_variables.cbegin();	it != available_variables.cend(); ++it) {
      unsigned int ix = it->getEnumeration();
      unsigned int c = ix % side_length_;
      unsigned int r = (ix - c) / side_length_;
      unsigned int b = root_ * (r / root_) + (c / root_);

      num_constraints = box_assignment_counts[b] + column_assignment_counts[c]
	+ row_assignment_counts[r];

      if(num_constraints >= max_constraint_count) {
	max_constraint_count = num_constraints;
	most_constrained_variable = *it;
	//std::cout << most_constrained_variable.getEnumeration() << "\n";
      }

      num_constraints = 0;
    }

    return most_constrained_variable;
  }

  // Returns the an ordered (by least constraining value) vector of values
  // still available.
  virtual std::vector<unsigned int>
  orderDomainValues(const Variable<unsigned int>&      variable,
		    const AssignmentSet<unsigned int>& assignment_set) const {

    std::set<Variable<unsigned int> > assignments = assignment_set.getCurrentAssignments();

    if(assignments.empty()) {
      std::vector<unsigned int> all_values;

      for(unsigned int i = 1; i <= side_length_; ++i)
	all_values.push_back(i);

      return all_values;
    }

    unsigned int v_i = variable.getEnumeration();
    unsigned int v_x = v_i % side_length_;
    unsigned int v_y = (v_i - v_x) / side_length_;
    unsigned int v_b = root_ * (v_y / root_) + (v_x / root_);

    std::vector<unsigned int> value_use_counts(side_length_, 0);

    std::set<unsigned int> value_set = value_set_;

    // Tally how many times each value has been used.
    // Remove values from value set that can not be assigned to the variable.
    for(auto it = assignments.cbegin(); it != assignments.cend(); ++it) {
      unsigned int value = it->value;

      unsigned int i = it->getEnumeration();
      unsigned int x = i % side_length_;
      unsigned int y = (i - x) / side_length_;
      unsigned int b = root_ * (y / root_) + (x / root_);

      ++value_use_counts[value - 1];

      if(v_x == x || v_y == y || v_b == b)
	value_set.erase(value);
    }

    // Copy values from std::set to std::vector.
    std::vector<unsigned int> ordered_values;

    for(auto it = value_set.cbegin(); it != value_set.cend(); ++it)
      ordered_values.push_back(*it);

    // Insertion sort the values in least constraining order.
    // A value is more constraining if it is used more often.
    for(unsigned int p = 1; p < ordered_values.size(); ++p) {
      unsigned int value = ordered_values[p];

      int i = p - 1;

      while(i >= 0 && (value_use_counts[ordered_values[i]] < value_use_counts[value])) {
	ordered_values[i + 1] = ordered_values[i];
	i = i - 1;
      }

      ordered_values[i + 1] = value;
    }

    return ordered_values;
  }

  // Checks if given variable/value assignment is consistant with current assignment.
  // Also performs forward checking if candidate assignment is valid.
  virtual bool isConsistant(const Variable<unsigned int>&      variable,
			    const unsigned int&                value,
			    const AssignmentSet<unsigned int>& assignment_set) const {

    if(!Csp<unsigned int>::isConsistant(variable, value, assignment_set))
      return false;

    Variable<unsigned int> candidate_variable(variable);
    candidate_variable.value = value;

    AssignmentSet<unsigned int> candidate_assignment_set(assignment_set);
    candidate_assignment_set.add(candidate_variable);

    std::set<Variable<unsigned int> > candidate_assignments = candidate_assignment_set.getCurrentAssignments();

    std::set<Variable<unsigned int> > available_variables = variable_set_;

    for(auto it = candidate_assignments.cbegin(); it != candidate_assignments.cend(); ++it)
      available_variables.erase(Variable<unsigned int>(it->getEnumeration()));

    for(auto it = available_variables.cbegin(); it != available_variables.cend(); ++it) {
      int forward_check_ok;

      auto iv = value_set_.cbegin();

      do {
	forward_check_ok = constraint_set_.size();
	if(iv == value_set_.cend()) return false;
	for(auto ic = constraint_set_.cbegin(); ic != constraint_set_.cend(); ++ic)
	  if((*ic)(*it, *iv, candidate_assignment_set))
	    --forward_check_ok;
      }while(forward_check_ok);
    }

    return true;
  }

private:

  class BoxConstraint : public Constraint<unsigned int> {
  public:

    BoxConstraint(const unsigned int& root)
    : Constraint<unsigned int>()
    , root_(root)
    , side_length_() {

      side_length_ = root * root;
    }

    virtual ~BoxConstraint() {}

    virtual bool operator()(const Variable<unsigned int>&      variable,
			    const unsigned int&                value,
			    const AssignmentSet<unsigned int>& assignment_set) const {

      unsigned int v_i = variable.getEnumeration();
      unsigned int v_x = v_i % root_;
      unsigned int v_y = ((v_i - (v_i % side_length_)) / side_length_) % root_;

      std::set<Variable<unsigned int> > assignments = assignment_set.getCurrentAssignments();

      for(auto it = assignments.cbegin(); it != assignments.cend(); ++it) {
	unsigned int b_i = it->getEnumeration();
	unsigned int b_x = (b_i % side_length_) / root_;
	unsigned int b_y = ((b_i - (b_i % side_length_)) / side_length_) / root_;

	if((v_x == b_x && v_y == b_y) && value == it->value) return false;
      }

      return true;
    }

  private:

    unsigned int root_, side_length_;
  };

  class ColumnConstraint : public Constraint<unsigned int> {
  public:

    ColumnConstraint(const unsigned int& root)
    : Constraint<unsigned int>()
    , root_(root)
    , side_length_() {

      side_length_ = root * root;
    }

    virtual ~ColumnConstraint() {}

    virtual bool operator()(const Variable<unsigned int>&      variable,
			    const unsigned int&                value,
			    const AssignmentSet<unsigned int>& assignment_set) const {

      unsigned int v_x = variable.getEnumeration() % side_length_;

      std::set<Variable<unsigned int> > assignments = assignment_set.getCurrentAssignments();

      for(auto it = assignments.cbegin(); it != assignments.cend(); ++it) {
	unsigned int c_x = it->getEnumeration() % side_length_;

	if(v_x == c_x && value == it->value) return false;
      }

      return true;
    }

  private:

    unsigned int root_, side_length_;
  };

  class RowConstraint : public Constraint<unsigned int> {
  public:

    RowConstraint(const unsigned int& root)
    : Constraint<unsigned int>()
    , root_(root)
    , side_length_() {

      side_length_ = root * root;
    }

    virtual ~RowConstraint() {}

    virtual bool operator()(const Variable<unsigned int>&      variable,
			    const unsigned int&                value,
			    const AssignmentSet<unsigned int>& assignment_set) const {

      unsigned int v_i = variable.getEnumeration();
      unsigned int v_y = (v_i - (v_i % side_length_)) / side_length_;

      std::set<Variable<unsigned int> > assignments = assignment_set.getCurrentAssignments();

      for(auto it = assignments.cbegin(); it != assignments.cend(); ++it) {
	unsigned int r_i = it->getEnumeration();
	unsigned int r_y = (r_i - (r_i % side_length_)) / side_length_;

	if(v_y == r_y && value == it->value) return false;
      }

      return true;
    }

  private:

    unsigned int root_, side_length_;
  };

  unsigned int root_, side_length_;
};

#endif // __SUDOKUCSP_HPP__
