#pragma once

#include "non-deterministic-finite-automata.hh"
#include "regex-lexer.hh"
#include "regex-ast.hh"

#include <memory>

typedef std::shared_ptr<NFiniteAutomata> NFiniteAutomataPtr;

//! This class is not thread safe. In fact, call parse once and then it's
//! garbage
class RegexParserAst {
 public:
  RegexParserAst(RegexLexer &lexer);
  AstNodePtr Parse();

 private:
  RegexLexer &lexer_;
  RegexLexer::Token lookahead_;
  std::uint32_t position_id_gen_;

  void Move();
  void Match(const RegexLexer::token_tag_t tag);

  AstNodePtr Regex();
  AstNodePtr Regexes(bool could_have_end_paren = false);
  AstNodePtr ParenRegex();

  AstNodePtr CheckModifiers(AstNodePtr ast);
  AstNodePtr ApplyKleene(AstNodePtr ast);
  AstNodePtr ApplyOr(AstNodePtr left, AstNodePtr right);
  AstNodePtr ApplyConcat(AstNodePtr left, AstNodePtr right);
};
