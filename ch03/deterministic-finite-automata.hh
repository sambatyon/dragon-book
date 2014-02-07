#pragma once

#include "finite-automata.hh"

#include <map>

class DFiniteAutomata : public FiniteAutomata {
 public:
  DFiniteAutomata(state_set_t states, state_set_t accepting_states,
                  state_t initial_state);
  DFiniteAutomata() : FiniteAutomata() {}

  state_t Move(state_t current_state, char symbol);
  bool Simulate(std::string str) override;

  void AddTransition(state_t from, char symbol, state_t to) override;
  bool RemoveTransition(state_t from, char symbol, state_t to) override;

 private:
  std::map<std::tuple<state_t, char>, state_t> transitions_;
};
