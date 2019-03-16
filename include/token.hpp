#ifndef CALC_TOKEN_HPP
#define CALC_TOKEN_HPP

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
  integer
};

class token {
  private:

  public:
  token();
  ~token();

  token_type type;
  
};

token::token() {
}

token::~token() {
}

} // core

#endif