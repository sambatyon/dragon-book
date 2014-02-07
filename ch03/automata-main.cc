#include "non-deterministic-finite-automata.hh"
#include "deterministic-finite-automata.hh"
#include "regex-lexer.hh"
#include "regex-parser.hh"

#include <iostream>
#include <sstream>

int main(int argc, char const **argv) {
  DFiniteAutomata a({0, 1, 2, 3}, {3}, 0);
  a.AddTransition(0, 'a', 1);
  a.AddTransition(0, 'b', 0);
  a.AddTransition(1, 'a', 1);
  a.AddTransition(1, 'b', 2);
  a.AddTransition(2, 'a', 1);
  a.AddTransition(2, 'b', 3);
  a.AddTransition(3, 'a', 1);
  a.AddTransition(3, 'b', 0);
  std::cout << (a.Simulate("abba") ? "true" : "false") << '\n';
  std::cout << static_cast<char>(255) << '\n';

  std::stringstream ss;
  ss << "(a|b)*abb";
  RegexLexer lexer(ss);
  RegexParser parser(lexer);
  auto nfa = parser.Parse();
  std::cout << *nfa << '\n';

  return 0;
}
