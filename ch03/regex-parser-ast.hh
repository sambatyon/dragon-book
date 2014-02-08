#pragma once

#include "non-deterministic-finite-automata.hh"
#include "regex-lexer.hh"

#include <memory>

typedef std::shared_ptr<NFiniteAutomata> NFiniteAutomataPtr;

class AstNode;
typedef std::shared_ptr<AstNode> AstNodePtr;

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

enum AstNodeType {
  eAstNodeSymbol,
  eAstNodeStar,
  eAstNodeEiter,
  eAstNodeCat
};

// This part should have visitors and all those things

// consider sending these classes somwhere else

struct AstNode {
  AstNode(AstNodeType the_type) : type(the_type) {}
  virtual ~AstNode() {}
  AstNodeType type;

};

struct AstNodeSymbol : public AstNode {
  AstNodeSymbol() : AstNode(eAstNodeSymbol), symbol('\x19'), position(0) {}

  char symbol;
  std::uint32_t position;
};

struct AstNodeStar : public AstNode {
  AstNodeStar() : AstNode(eAstNodeStar), ast() {}
  AstNodePtr ast;
};

struct AstNodeEither : public AstNode {
  AstNodeEither() : AstNode(eAstNodeStar), left(), right() {}
  AstNodePtr left;
  AstNodePtr right;
};

struct AstNodeCat : public AstNode {
  AstNodeCat() : AstNode(eAstNodeCat), left(), right() {}
  AstNodePtr left;
  AstNodePtr right;
};
