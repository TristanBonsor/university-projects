////////////////////////////////////////////////////////////////////////////////
// Csp.hpp
// Copyright (C) 2013  Tristan W. Bonsor
//
// A basic constraint satisfaction problem framework with backtracking search.
//
// Variable class declaration and definition.
// AssignmentSet class declaration and definition.
// Constraint abstract base class declaration.
// Csp abstract base class declaration and definition.
// BacktrackingSearch class declaration and definition.
////////////////////////////////////////////////////////////////////////////////
#ifndef __CSP_HPP__
#define __CSP_HPP__

#include <iostream>

#include <set>
#include <unordered_map>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
// Variable
//
// Represents a given variable used within a constraint satisfaction problem.
// This is a basic type utilized by AssignmentSet, Constraint, Csp, and
// BacktrackingSearch objects.
//
// Assumes type VALUE has a default constructor and copy constructor.
////////////////////////////////////////////////////////////////////////////////
template<class VALUE>
class Variable {
public:

  Variable(const unsigned int& enumeration_)
  : value()
  , enumeration(enumeration_) {}

  Variable(const Variable& variable)
  : value(variable.value)
  , enumeration(variable.enumeration) {}

  // For the std::set default comparison object (less<T> - see <functional>).
  bool operator<(const Variable<VALUE>& rhs) const;

  // Returns the variable enumeration.
  unsigned int getEnumeration() const;

  VALUE value;

private:

  unsigned int enumeration;
};

template<class VALUE>
bool Variable<VALUE>::operator<(const Variable<VALUE>& rhs) const {

  return (enumeration < rhs.enumeration);
}

template<class VALUE>
unsigned int Variable<VALUE>::getEnumeration() const {

  return enumeration;
}

////////////////////////////////////////////////////////////////////////////////
// AssignmentSet
//
// Represents an assignment set used for solving a constraint satisfaction
// problem.
//
// Assumes type VALUE has a copy constructor.
////////////////////////////////////////////////////////////////////////////////
template<class VALUE>
class AssignmentSet {
public:

  AssignmentSet() : assignments_() {}

  AssignmentSet(const AssignmentSet<VALUE>& assignment_set)
  : assignments_(assignment_set.assignments_) {}

  AssignmentSet<VALUE>& operator=(const AssignmentSet<VALUE>& rhs);

  // Returns the cardinality of the assignment set.
  unsigned int size() const;

  // Add a given variable to the assignment set. Assumes variable has been
  // assigned a value.
  void add(const Variable<VALUE>& variable);

  // Remove a given variable if enumeration and value match the argument
  // variable.
  void remove(const Variable<VALUE>& variable);

  // Return a copy of the current set of assigned variables.
  std::set<Variable<VALUE> > getCurrentAssignments() const;

private:

  std::unordered_map<unsigned int, Variable<VALUE> > assignments_;
};

template<class VALUE>
AssignmentSet<VALUE>& AssignmentSet<VALUE>::operator=(const AssignmentSet<VALUE>& rhs) {

  if(this != &rhs) {
    assignments_ = rhs.assignments_;
  }

  return *this;
}

template<class VALUE>
unsigned int AssignmentSet<VALUE>::size() const { return assignments_.size(); }

template<class VALUE>
void AssignmentSet<VALUE>::add(const Variable<VALUE>& variable) {

  auto it = assignments_.find(variable.getEnumeration());

  if(it == assignments_.end())
    assignments_.emplace(variable.getEnumeration(), variable);
}

template<class VALUE>
void AssignmentSet<VALUE>::remove(const Variable<VALUE>& variable) {

  auto it = assignments_.find(variable.getEnumeration());

  if(it != assignments_.end() && it->second.value == variable.value)
    assignments_.erase(variable.getEnumeration());
}

template<class VALUE>
std::set<Variable<VALUE> > AssignmentSet<VALUE>::getCurrentAssignments() const {

  std::set<Variable<VALUE> > assignment_set;

  for(auto it = assignments_.cbegin(); it != assignments_.cend(); ++it)
    assignment_set.insert(it->second);

  return assignment_set;
}

////////////////////////////////////////////////////////////////////////////////
// Constraint
//
// Abstract base class that represents a given constraint for a constraint
// satisfaction problem. Sub-class this class for each constraint that is
// required; these will be contained in a Csp object's constraint set.
////////////////////////////////////////////////////////////////////////////////
template<class VALUE>
class Constraint {
public:

  Constraint() : enumeration_() { enumeration_ = ++enumeration_counter_; }

  virtual ~Constraint() {}

  virtual bool operator<(const Constraint<VALUE>& rhs) const;

  // Returns true if a given variable/value assignment satisfies this
  // constraint. 
  virtual bool operator()(const Variable<VALUE>&      variable,
			  const VALUE&                value,
			  const AssignmentSet<VALUE>& assignment_set) const;

protected:

  static unsigned int enumeration_counter_;

  unsigned int enumeration_;
};

template<class VALUE>
unsigned int Constraint<VALUE>::enumeration_counter_ = 0;

template<class VALUE>
bool Constraint<VALUE>::operator<(const Constraint<VALUE>& rhs) const {

  return (enumeration_ < rhs.enumeration_);
}

template<class VALUE>
bool Constraint<VALUE>::operator()(const Variable<VALUE>&      variable,
				   const VALUE&                value,
				   const AssignmentSet<VALUE>& assignment_set) const {

  std::set<Variable<VALUE> > assignments = assignment_set.getCurrentAssignments();

  for(auto it = assignments.cbegin(); it != assignments.cend(); ++it) {
    if(variable.getEnumeration() == it->getEnumeration()) return true;
    if(value == it->value) return true;
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////
// Csp
//
// Abstract base class that represents a constraint satisfaction problem.
// Sub-class this class to implement a specific csp; this usually consists of
// two steps:
//            1. Implement selectUnassignedVariable() and orderDomainValues()
//               methods for use by a Backtrackingsearch object. This is where
//               you may implement some types of search heuristics.
//
//            2. Supply valid and complete variable, value, and constraint sets
//               as specified by the Csp constructor. Constraint sub-class
//               objects must be implemented for each constraint needed to
//               represent a given constraint satisfaction problem.
////////////////////////////////////////////////////////////////////////////////
template<class VALUE>
class Csp {
public:

  Csp(const std::set<Variable<VALUE> >&   variable_set,
      const std::set<VALUE>&              value_set,
      const std::set<Constraint<VALUE> >& constraint_set)
    : variable_set_(variable_set)
    , value_set_(value_set)
    , constraint_set_(constraint_set) {}

  virtual ~Csp() {}

  // Returns true if the given variable and value pairing is consistant with
  // the current assignment set, as dictated by the CSP's constraint set.
  // Returns false if at least one of the constraints is not satisfied.
  virtual bool isConsistant(const Variable<VALUE>&      variable,
			    const VALUE&                value,
			    const AssignmentSet<VALUE>& assignment_set) const;

  // Returns true if number the cardinality of the assignment set is
  // equal to the cardinality of this CSP's variable set.
  virtual bool isComplete(const AssignmentSet<VALUE>& assignment_set) const;

  // Returns the next suitable variable for assignment.
  virtual Variable<VALUE>
  selectUnassignedVariable(const AssignmentSet<VALUE>& assignment_set) const = 0;

  // Returns the set of values available for assignment.
  virtual std::vector<VALUE>
  orderDomainValues(const Variable<VALUE>&      variable,
		    const AssignmentSet<VALUE>& assignment_set) const = 0;

protected:

  std::set<Variable<VALUE> >   variable_set_;
  std::set<VALUE>              value_set_;
  std::set<Constraint<VALUE> > constraint_set_;
};

template<class VALUE>
bool Csp<VALUE>::isConsistant(const Variable<VALUE>&      variable,
			      const VALUE&                value,
			      const AssignmentSet<VALUE>& assignment_set) const {

  for(auto c = constraint_set_.cbegin(); c != constraint_set_.cend(); ++c)
    if(!(*c)(variable, value, assignment_set)) return false;

  return true;
}

template<class VALUE>
bool Csp<VALUE>::isComplete(const AssignmentSet<VALUE>& assignment_set) const {

  return (assignment_set.size() == variable_set_.size());
}

////////////////////////////////////////////////////////////////////////////////
// BacktrackingSearch
//
// Represents a generalized backtracking search agent for constraint
// satisfaction problems.
////////////////////////////////////////////////////////////////////////////////
template<class VALUE>
class BacktrackingSearch {
public:

  // Returns a solution (a complete and consistant assignment set) or failure
  // (an empty assignment set) to a constraint satisfaction problem.
  AssignmentSet<VALUE> operator()(const AssignmentSet<VALUE>& assignment_set,
				  const Csp<VALUE>&           csp) const;

private:

  // Utilized by operator(), returns as per operator(). This method passes the
  // assignment set by non-const reference however.
  AssignmentSet<VALUE> recursiveBacktracking(AssignmentSet<VALUE>& assignment_set,
					     const Csp<VALUE>&     csp) const;
};

template<class VALUE>
AssignmentSet<VALUE> BacktrackingSearch<VALUE>::operator()(const AssignmentSet<VALUE>& assignment_set,
							   const Csp<VALUE>&           csp) const {

  AssignmentSet<VALUE> initial_state = assignment_set;
  return recursiveBacktracking(initial_state, csp);
}

template<class VALUE>
AssignmentSet<VALUE> BacktrackingSearch<VALUE>::recursiveBacktracking(AssignmentSet<VALUE>& assignment_set,
								      const Csp<VALUE>&     csp) const {

  if(csp.isComplete(assignment_set)) return assignment_set; // Success.

  Variable<VALUE> variable = csp.selectUnassignedVariable(assignment_set);

  std::vector<VALUE> values = csp.orderDomainValues(variable, assignment_set);

  // Find consistant value for a given variable.
  for(auto it = values.cbegin(); it != values.cend(); ++it) {
    if(csp.isConsistant(variable, *it, assignment_set)) {
      // Variable/value is consistant.
      variable.value = *it;

      assignment_set.add(variable);

      // Continue assigning consistant values to unassigned variables, if
      // possible.
      AssignmentSet<VALUE> result = recursiveBacktracking(assignment_set, csp);

      if(csp.isComplete(result)) return result; // Success.

      // Further assignments did not work out...
      assignment_set.remove(variable);
    }
  }

  // Failure.
  return AssignmentSet<VALUE>();
}

#endif // __CSP_HPP__
