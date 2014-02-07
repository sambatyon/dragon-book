#pragma once

#include "finite-automata.hh"

#include <ostream>
#include <map>

class RegexParser;

class NFiniteAutomata : public FiniteAutomata {
 public:
  // Uses the char 0x19 (end of medium) to signal an empty transition.
  // I chose it because I have never seen that being used.
  static constexpr char kEpsilon = static_cast<char>(0x19);

  NFiniteAutomata(state_set_t states, state_set_t accepting_states,
                  state_t initial_state);
  NFiniteAutomata() : FiniteAutomata() {}

  state_set_t Move(state_t current_state, char symbol);
  state_set_t Move(state_set_t current_states, char symbol);

  void AddTransition(state_t from, char symbol, state_t to) override;
  bool RemoveTransition(state_t from, char symbol, state_t to) override;

  bool Simulate(std::string str) override;

  state_set_t EpsilonClosure(state_t state);
  state_set_t EpsilonClosure(state_set_t states);

 private:
  friend class RegexParser;
  friend std::ostream &operator<<(std::ostream &stream,
                                  NFiniteAutomata &automata);
  std::map<std::tuple<state_t, char>, state_set_t> transitions_;
};

std::ostream &operator<<(std::ostream &stream, NFiniteAutomata &automata);

