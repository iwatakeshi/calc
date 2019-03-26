#ifndef CALC_LEX_HPP
#define CALC_LEX_HPP
#include "token.hpp"
#include <cctype>
#include <exception>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

namespace calc {
class lexer {
  private:
  std::string source;
  int position;
  int start_position;

  public:
  lexer() :
      source(""),
      position(0),
      start_position(0) {}
  lexer(std::string source) :
      source(source), position(0),
      start_position(0) {}

  ~lexer() {}

  std::vector<token> tokens;

  typedef typename std::vector<token>::iterator iterator;
  typedef typename std::vector<token>::const_iterator const_iterator;

  friend std::istream& operator>>(std::istream& istream, lexer& other) {
    other.position = 0;
    other.start_position = 0;
    if (!other.tokens.empty()) {
      other.tokens.clear();
    }
    std::getline(istream, other.source);
    return istream;
  }

  friend lexer& operator>>(const std::string& source, lexer& other) {
    other.reset();
    other.source = source;
    return other;
  }

  friend std::ostream& operator<<(std::ostream& ostream, const lexer& other) {

    return ostream;
  }

  void reset() {
    position = 0;
    start_position = 0;
    if (!tokens.empty()) {
      tokens.clear();
    }
  }

  void scan() {
    while (!eos()) {
      start_position = position;
      scan_tokens();
    }
    tokens.push_back(token(token::eof, "eof", 0));
  }

  iterator begin() {
    return tokens.begin();
  }

  const_iterator begin() const {
    return tokens.begin();
  }

  iterator end() {
    return tokens.end();
  }

  const_iterator end() const {
    return tokens.end();
  }

  std::size_t size() {
    return tokens.size();
  }

  private:
  void scan_tokens() {
    char ch = next();
    switch (ch) {
    case '(':
      add_token(token::lparen);
      break;
    case ')':
      add_token(token::rparen);
      break;
    case '+':
      add_token(token::plus);
      break;
    case '-':
      add_token(token::minus);
      break;
    case '*':
      add_token(token::star);
      break;
    case '/':
      add_token(token::slash);
      break;
    case '%':
      add_token(token::modulo);
      break;
    case '^':
      add_token(token::caret);
      break;
    case ' ':
    case '\r':
    case '\t':
      break;
    default:
      if (std::isdigit(ch)) {
        bool zero = (ch == '0');
        if (zero) {
          next();
        }
        if (zero && std::tolower(peek()) == 'b') {
          next();
          scan_binary();
        } else if (zero && std::tolower(peek()) == 'o') {
          next();
          scan_octal();
        } else if (zero && std::tolower(peek()) == 'x') {
          next();
          scan_hex();
        } else {
          scan_number();
        }

      } else {
        throw error("Syntax error: Unexpected character", ch);
      }
      break;
    }
  }

  void scan_number() {
    token::token_type type = token::integer;
    while (std::isdigit(peek()))
      next();

    // Determine whether we are dealing with
    if (peek() == '.' && std::isdigit(peek(1))) {
      type = token::decimal;
      // Consume '.'
      next();

      while (std::isdigit(peek()))
        next();
    }

    char ch = peek();
    if (ch == 'e') {
      // Consume 'e'
      next();

      if ((peek() == '+' || peek() == '-')) {
        ch = peek();

        if (peek() == '-') {
          type = token::decimal;
        }

        // Consume '+' || Consume '-'
        next();
      }

      if (std::isdigit(peek())) {
        while (std::isdigit(peek()))
          next();
      } else {
        throw error("Lex error: Expected a numerical value after", ch);
      }
    }

    return add_token(type, std::stod(source.substr(start_position, position - start_position)));
  }

  void scan_binary() {
    while (peek() >= '0' && peek() <= '1')
      next();
    return add_token(token::binary, std::stod(source.substr(start_position, position - start_position)));
  }

  void scan_octal() {
    while (peek() >= '0' && peek() <= '7')
      next();
    return add_token(token::octal, std::stod(source.substr(start_position, position - start_position)));
  }

  void scan_hex() {

    while (std::isdigit(peek()) || std::tolower(peek()) >= 'a' && std::tolower(peek()) <= 'f')
      next();
    return add_token(token::hex, std::stod(source.substr(start_position, position - start_position)));
  }

  char peek() {
    if (eos()) return '\0';
    return source[position];
  }

  char peek(int seek) {
    if (eos()) return '\0';
    if (position + seek >= source.length()) {
      return '\0';
    }
    return source[position + seek];
  }

  char next() {
    position++;
    return source[position - 1];
  }

  char next(int seek) {
    while((seek--) > 0) {
      next();
    }
  }

  bool eos() {
    return position >= source.length();
  }

  void add_token(token::token_type type) {
    add_token(type, 0);
  }

  void add_token(token::token_type type, double literal) {
    const std::string lexeme = source.substr(start_position, position - start_position);
    tokens.push_back(token(type, lexeme, literal));
  }

  std::runtime_error error(std::string message, char ch) {
    return std::runtime_error(message + " " + "'" + ch + "'");
  }
};

} // calc

#endif