#ifndef CALC_LEX_HPP
#define CALC_LEX_HPP
#include "token.hpp"
#include <cctype>
#include <exception>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <vector>

namespace core {
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
    if(!other.tokens.empty()) {
      other.tokens.clear();
    }
    std::getline(istream, other.source);
    return istream;
  }

  friend std::ostream& operator<<(std::ostream& ostream, const lexer& other) {

    return ostream;
  }

  void scan() {
    while (!eos()) {
      start_position = position;
      scan_tokens();
    }
    tokens.push_back(token(eof, "eof", 0));
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
      add_token(lparen);
      break;
    case ')':
      add_token(rparen);
      break;
    case '+':
      add_token(plus);
      break;
    case '-':
      add_token(minus);
      break;
    case '*':
      add_token(star);
      break;
    case '/':
      add_token(slash);
      break;
    case '%':
      add_token(modulo);
      break;
    case '^':
      add_token(caret);
      break;
    case ' ':
    case '\r':
    case '\t':
      break;
    default:
      if (std::isdigit(ch)) {
        scan_number();
      } else {
        throw error("Syntax error: Unexpected character", ch);
      }
      break;
    }
  }

  void scan_number() {
    token_type type = token_type::integer;
    while (std::isdigit(peek()))
      next();

    // Determine whether we are dealing with
    if (peek() == '.' && std::isdigit(peek(1))) {
      type = token_type::decimal;
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
        // Consume '+' || Consume '-'
        next();
      }

      if (std::isdigit(peek())) {
        while (std::isdigit(peek()))
          next();
      } else {
        throw error("Syntax error: Expected a numerical value after", ch);
      }
    }

    return add_token(type, std::stod(source.substr(start_position, position - start_position)));
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

  bool eos() {
    return position >= source.length();
  }

  void add_token(token_type type) {
    add_token(type, 0);
  }

  void add_token(token_type type, double literal) {
    const std::string lexeme = source.substr(start_position, position - start_position);
    tokens.push_back(token(type, lexeme, literal));
  }

  std::runtime_error error(std::string message, char ch) {
    return std::runtime_error(message + " " + "'" + ch + "'");
  }
};

} // core

#endif