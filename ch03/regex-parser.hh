#pragma once

#include "non-deterministic-finite-automata.hh"
#include "regex-lexer.hh"

#include <memory>

typedef std::shared_ptr<NFiniteAutomata> NFiniteAutomataPtr;

//! This class is not thread safe. In fact, call parse once and then it's
//! garbage
class RegexParser {
 public:
  RegexParser(RegexLexer &lexer);
  NFiniteAutomataPtr Parse();

 private:
  RegexLexer &lexer_;
  RegexLexer::Token lookahead_;
  state_t state_id_gen_;

  void Move();
  void Match(const RegexLexer::token_tag_t tag);

  NFiniteAutomataPtr Regex();
  NFiniteAutomataPtr Regexes(bool could_have_end_paren = false);
  NFiniteAutomataPtr ParenRegex();

  NFiniteAutomataPtr CheckModifiers(NFiniteAutomataPtr automata);
  NFiniteAutomataPtr ApplyKleene(NFiniteAutomataPtr automata);
  NFiniteAutomataPtr ApplyOr(NFiniteAutomataPtr left, NFiniteAutomataPtr right);
  NFiniteAutomataPtr ApplyConcat(NFiniteAutomataPtr left,
                                 NFiniteAutomataPtr right);

};
