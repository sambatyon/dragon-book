#include "non-deterministic-finite-automata.hh"

#include <queue>

NFiniteAutomata::NFiniteAutomata(state_set_t states,
                                 state_set_t accepting_states,
                                 state_t initial_state)
    : FiniteAutomata(states, accepting_states, initial_state) {
}

void NFiniteAutomata::AddTransition(state_t from, char symbol, state_t to) {
  auto key = std::make_tuple(from, symbol);
  transitions_[key].insert(to);
}

bool NFiniteAutomata::RemoveTransition(state_t from, char symbol, state_t to) {
  auto key = std::make_tuple(from, symbol);
  auto it = transitions_.find(key);
  if (it == std::end(transitions_) ||
      it->second.find(to) == std::end(it->second))
    return false;
  it->second.erase(to);
  if (it->second.empty())
    transitions_.erase(it);
  return true;
}

state_set_t NFiniteAutomata::Move(state_t current_state, char symbol) {
  auto key = std::make_tuple(current_state, symbol);
  auto res = transitions_.find(key);
  if (res == std::end(transitions_))
    return state_set_t();
  return res->second;
}

state_set_t NFiniteAutomata::Move(state_set_t current_states, char symbol) {
  state_set_t res;
  for (auto &s : current_states) {
    auto sets = Move(s, symbol);
    res.insert(std::begin(sets), std::end(sets));
  }
  return res;
}

state_set_t NFiniteAutomata::EpsilonClosure(state_t state) {
  state_set_t visited;
  std::queue<state_t> to_visit;
  to_visit.push(state);
  while (!to_visit.empty()) {
    auto current = to_visit.front();
    to_visit.pop();
    if (visited.find(current) != std::end(visited))
      continue;
    auto candidates = Move(current, kEpsilon);
    for (auto &c : candidates)
      to_visit.push(c);
    visited.insert(current);
  }
  return visited;
}

state_set_t NFiniteAutomata::EpsilonClosure(state_set_t states) {
  state_set_t res;
  for (auto &s : states) {
    auto sets = EpsilonClosure(s);
    res.insert(std::begin(sets), std::end(sets));
  }
  return res;
}

bool NFiniteAutomata::Simulate(std::string str) {
  auto reachable = EpsilonClosure(initial_state_);
  for (auto &c : str) {
    reachable = EpsilonClosure(Move(reachable, c));
    if (reachable.empty())
      return false;
  }
  for (auto &s : reachable) {
    if (accepting_states_.find(s) != std::end(accepting_states_))
      return true;
  }
  return false;
}

std::ostream &operator<<(std::ostream &stream, NFiniteAutomata &automata) {
  stream << "{\n  \"states\" : [";
  bool first = true;
  for (auto &state : automata.states()) {
    if (!first) {
      stream << ", ";
    }
    stream << state;
      first = false;
  }
  stream << "],\n  \"accepting\" : [";
  first = true;
  for (auto &state : automata.accepting_states()) {
    if (!first) {
      stream << ", ";
    }
    stream << state;
    first = false;
  }
  stream << "],\n  \"transitions\" : [";
  first = true;
  for (auto &trans : automata.transitions_) {
    if (!first) {
      stream << ", ";
    }
    first = false;
    stream << "\n     [" << std::get<0>(trans.first) << ", '";
    auto symbol = std::get<1>(trans.first);
    stream << static_cast<char>(symbol == '\x19' ? 238 : symbol) << "', [";
    bool first_to = true;
    for (auto &to : trans.second) {
      if (!first_to) {
        stream << ", ";
      }
      first_to = false;
      stream << to;
    }
    stream << "]]";
  }
  stream << "\n  ],\n  \"initial\" : " << automata.initial_state() << "\n}";
  return stream;
}
