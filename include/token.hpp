#ifndef CALC_TOKEN_HPP
#define CALC_TOKEN_HPP
#include <ostream>
#include <string>

namespace calc {

struct token {
  enum token_type : int;
  token() {}
  token(token_type type, std::string lexeme, double literal) :
      type(type),
      lexeme(lexeme),
      literal(literal){};

  ~token() {}

  friend std::ostream& operator<<(std::ostream& os, const token& other) {

    if (other.type == decimal || other.type == integer) {
      os << "( token: " << other.literal << " )";
    } else {
      os << "( token: " << other.lexeme << " ) ";
    }

    return os;
  }

  enum token_type : int {
    // Operators

    // '+'
    plus,
    // '-'
    minus,
    // '/'
    slash,
    // '*'
    star,
    // '%'
    modulo,
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
    // binary
    binary,
    // hexidecimal
    hex,
    // octal
    octal,
    // Program

    // end of file/stream
    eof = -1
  };

  token_type type;
  std::string lexeme;
  double literal;
};

} // calc

#endif