#include "regex-parser-ast.hh"

RegexParserAst::RegexParserAst(RegexLexer &lexer) : lexer_(lexer)
                                                  , lookahead_()
                                                  , position_id_gen_(0) {
  Move();
}

void RegexParserAst::Move() {
  lookahead_ = lexer_.Scan();
}

AstNodePtr RegexParserAst::Parse() {
  return Regexes(); // if the tag after this return is different from kEof,
                    // there was an error
}

AstNodePtr RegexParserAst::Regex() {
  switch (lookahead_.tag) {
    case RegexLexer::kEof:
      // This should not happen
      return AstNodePtr(); // nothing to be done
      break;
    case RegexLexer::kLParen:
      return CheckModifiers(ParenRegex());
      break;
    case RegexLexer::kSymbol: {

      auto res = std::make_shared<AstNodeSymbol>(lookahead_.value,
                                                 position_id_gen_++);

      Move();
      return CheckModifiers(res);
    } break;
    case RegexLexer::kRParen:
    case RegexLexer::kKleene:
    case RegexLexer::kEither:
    default:
      throw std::runtime_error("syntax error");
      break;
  }
}

AstNodePtr RegexParserAst::CheckModifiers(AstNodePtr ast) {
  AstNodePtr res = ast;
  if (lookahead_.tag == RegexLexer::kKleene) {
    Move();
    res = ApplyKleene(ast);
  } else if (lookahead_.tag == RegexLexer::kEither) {
    Move();
    auto right_hand = Regex();
    res = ApplyOr(res, right_hand);
  }
  return res;
}

AstNodePtr RegexParserAst::Regexes(bool could_have_end_paren) {
  if (lookahead_.tag == RegexLexer::kEof)
    return AstNodePtr();
  auto first = Regex();
  // If we reach this place from a parenthesis, it can be done after the first
  // regex and then the next symbol is a closing parenthesis. Any other symbol
  // should have been consumed in the previous call to regex. If there is
  // something like a** then there's a syntax error and the next call to
  // regex will handle it.
  if (lookahead_.tag == RegexLexer::kEof ||
       (could_have_end_paren && lookahead_.tag == RegexLexer::kRParen))
    return first;
  auto second = Regexes();
  return ApplyConcat(first, second);
}

AstNodePtr RegexParserAst::ParenRegex() {
  Match(RegexLexer::kLParen);
  auto res = Regexes(true);
  // There must be no empty regex in a parenthesis
  if (!res)
      throw std::runtime_error("syntax error");
  Match(RegexLexer::kRParen);
  return res;
}

AstNodePtr RegexParserAst::ApplyKleene(AstNodePtr ast) {
  return std::make_shared<AstNodeStar>(ast);
}

AstNodePtr RegexParserAst::ApplyOr(AstNodePtr left, AstNodePtr right) {
  return std::make_shared<AstNodeEither>(left, right);
}

AstNodePtr RegexParserAst::ApplyConcat(AstNodePtr left, AstNodePtr right) {
  return std::make_shared<AstNodeCat>(left, right);
}

void RegexParserAst::Match(const RegexLexer::token_tag_t tag) {
    if (lookahead_.tag == tag)
        Move();
    else
        throw std::runtime_error("syntax error");
}
