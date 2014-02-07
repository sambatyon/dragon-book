#pragma once

#include <unordered_set>
#include <cstdint>
#include <limits>
#include <tuple>

typedef std::uint32_t state_t;
typedef std::unordered_set<state_t> state_set_t;

class FiniteAutomata {
 public:
  typedef std::uint32_t state_t;
  typedef std::unordered_set<state_t> state_set_t;

  static constexpr state_t kInvalidState = std::numeric_limits<state_t>::max();

  FiniteAutomata(state_set_t states, state_set_t accepting_states,
                 state_t initial_state);
  FiniteAutomata();
  virtual ~FiniteAutomata();

  state_t initial_state() const;
  void set_initial_state(state_t state);

  state_set_t &states();
  state_set_t const &states() const;

  state_set_t &accepting_states();
  state_set_t const &accepting_states() const;

  virtual bool Simulate(std::string str) = 0;
  virtual void AddTransition(state_t from, char symbol, state_t to) = 0;
  virtual bool RemoveTransition(state_t from, char symbol, state_t to) = 0;

 protected:
  state_set_t states_;
  state_set_t accepting_states_;
  state_t initial_state_;
};
