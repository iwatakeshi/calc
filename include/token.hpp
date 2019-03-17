#ifndef CALC_TOKEN_HPP
#define CALC_TOKEN_HPP
#include <ostream>
#include <string>

namespace core {

enum token_type {
  // Operators

  // '+'
  plus,
  // '-'
  minus,
  // '/'
  slash,
  // '*'
  star,
  // '^'
  caret,
  // '('
  lparen,
  // ')'
  rparen,

  // Literals

  // floating point numbers
  decimal,
  // whole numbers
  integer,

  // Program
  eof = -1
};

struct token {
  token() {}

  token(token_type type, std::string lexeme, double literal) :
      type(type),
      lexeme(lexeme),
      literal(literal){};

  ~token() {}

  friend std::ostream& operator<<(std::ostream& os, const token& other) {

    if (other.type == decimal) {
      os << "( " << other.literal << " )";
    } else if (other.type == integer) {
      os << "( " << int(other.literal) << " )";
    } else {
      os << "( " << other.lexeme << " ) ";
    }

    return os;
  }

  token_type type;
  std::string lexeme;
  double literal;
};

} // core

#endif