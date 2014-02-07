#include "finite-automata.hh"

#include <stdexcept>

FiniteAutomata::FiniteAutomata(state_set_t states, state_set_t accepting_states,
                               state_t initial_state)
    : states_(states)
    , accepting_states_(accepting_states)
    , initial_state_(initial_state) {
  if (states_.find(initial_state_) == std::end(states_))
    throw std::invalid_argument("Initial State is not in the state's set");
  for (auto &state : accepting_states_)
    if (states_.find(state) == std::end(states_))
      throw std::invalid_argument("Accepting state not in state's set");
}

FiniteAutomata::FiniteAutomata() : states_(), accepting_states_()
                                 , initial_state_() {

}

FiniteAutomata::~FiniteAutomata() {
}

state_t FiniteAutomata::initial_state() const {
  return initial_state_;
}

void FiniteAutomata::set_initial_state(state_t state) {
  if (states_.find(state) == std::end(states_))
    throw std::invalid_argument("Initial State is not in the state's set");
  initial_state_ = state;
}

state_set_t &FiniteAutomata::states() {
  return states_;
}

state_set_t const &FiniteAutomata::states() const {
  return states_;
}

state_set_t &FiniteAutomata::accepting_states() {
  return accepting_states_;
}

state_set_t const &FiniteAutomata::accepting_states() const {
  return accepting_states_;
}
