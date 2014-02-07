#pragma once

#include <istream>
#include <cstdint>
#include <limits>

class RegexLexer {
 public:
  typedef std::uint32_t token_tag_t;
  static constexpr token_tag_t kLParen = static_cast<token_tag_t>('(');
  static constexpr token_tag_t kRParen = static_cast<token_tag_t>(')');
  static constexpr token_tag_t kKleene = static_cast<token_tag_t>('*');
  static constexpr token_tag_t kEither = static_cast<token_tag_t>('|');
  static constexpr token_tag_t kSymbol = static_cast<token_tag_t>(256);
  static constexpr token_tag_t kEof = std::numeric_limits<token_tag_t>::max();

  struct Token {
    Token() : tag(kEof), value('\0') {}
    Token(token_tag_t the_tag, char the_value) : tag(the_tag)
                                               , value(the_value) {
    }
    token_tag_t tag;
    char value;
  };

  RegexLexer(std::istream &input);
  Token Scan();

 protected:
  void ReadCh();

 private:
  std::istream &input_;
  char peek_;
};
