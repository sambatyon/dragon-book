#include "regex-lexer.hh"
#include <stdexcept>
#include <sstream>

RegexLexer::RegexLexer(std::istream &input) : input_(input), peek_(' ') {
}

void RegexLexer::ReadCh() {
  input_.get(peek_);
  if (input_.eof())
    peek_ = '\0';
}

RegexLexer::Token RegexLexer::Scan() {
  ReadCh();
  if (!peek_)
    return Token(kEof, peek_);
  switch (peek_) {
    case '\\': {
      ReadCh();
      switch (peek_) {
        case '\\':
          return Token(kSymbol, '\\');
          break;
        case '(':
          return Token(kSymbol, '(');
          break;
        case ')':
          return Token(kSymbol, ')');
          break;
        case '*':
          return Token(kSymbol, '*');
          break;
        case '|':
          return Token(kSymbol, '|');
          break;
        default: {
          std::stringstream ss;
          ss << "Invalid escaped character '" << peek_ << '\'';
          throw std::runtime_error(ss.str());
        }
      }
      }
      break;
    case '(':
      return Token(kLParen, '(');
      break;
    case ')':
      return Token(kRParen, ')');
      break;
    case '*':
      return Token(kKleene, '*');
      break;
    case '|':
      return Token(kEither, '|');
      break;
    default:
      return Token(kSymbol, peek_);
      break;
  }
}
