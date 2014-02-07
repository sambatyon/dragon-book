#include "deterministic-finite-automata.hh"

DFiniteAutomata::DFiniteAutomata(state_set_t states,
                                 state_set_t accepting_states,
                                 state_t initial_state)
    : FiniteAutomata(states, accepting_states, initial_state) {
}

state_t DFiniteAutomata::Move(state_t current_state, char symbol) {
  auto key = std::make_tuple(current_state, symbol);
  auto res = transitions_.find(key);
  if (res == std::end(transitions_))
    throw FiniteAutomata::kInvalidState;
  return res->second;
}

void DFiniteAutomata::AddTransition(state_t from, char symbol, state_t to) {
  auto key = std::make_tuple(from, symbol);
  transitions_[key] = to;
}

bool DFiniteAutomata::RemoveTransition(state_t from, char symbol, state_t to) {
  auto key = std::make_tuple(from, symbol);
  auto it = transitions_.find(key);
  if (it == std::end(transitions_) || it->second != to)
    return false;
  transitions_.erase(it);
  return true;
}

bool DFiniteAutomata::Simulate(std::string str) {
  auto current_state = initial_state_;
  for (auto &symbol : str) {
      current_state = Move(current_state, symbol);
      if (current_state == FiniteAutomata::kInvalidState)
        return false;
  }
  return accepting_states_.find(current_state) != std::end(accepting_states_);
}
