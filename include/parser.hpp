#ifndef CALC_PARSER_HPP
#define CALC_PARSER_HPP
#include "grammar/expression.hpp"
#include "lexer.hpp"
#include "token.hpp"
#include <algorithm>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Resources: http://pages.cs.wisc.edu/~fischer/cs536.s08/course.hold/html/NOTES/3.CFG.html

namespace calc {
class parser {
  using expression_t = std::unique_ptr<expression>;

  private:
  std::vector<token> source;
  int position;

  public:
  parser() :
      position(0) {}
  parser(const std::vector<token>& source) :
      source(source), position(0) {}
  ~parser() {}

  friend void operator>>(lexer& lexer, parser& parser) {
    parser.reset();
    lexer.scan();
    parser.source = lexer.tokens;
  }

  std::vector<expression_t> parse() {
    std::vector<expression_t> expressions;
    while (!eos()) {
      expressions.emplace_back(parse_expression());
    }
    return expressions;
  }

  void reset() {
    position = 0;
    if (!source.empty()) {
      source.clear();
    }
  }

  private:
  expression_t parse_expression() {
    return parse_bitwise_or();
  }

  expression_t parse_bitwise_or() {
    auto left = parse_bitwise_xor();
    while (match({ token::bitwise_or })) {
      token op = previous();
      auto right = parse_bitwise_xor();
      left = expression_t(new binary(std::move(left), op, std::move(right)));
    }

    return left;
  }

  expression_t parse_bitwise_xor() {
    auto left = parse_bitwise_and();
    while (match({ token::bitwise_xor })) {
      token op = previous();
      auto right = parse_bitwise_and();
      left = expression_t(new binary(std::move(left), op, std::move(right)));
    }

    return left;
  }

  expression_t parse_bitwise_and() {
    auto left = parse_addition();
    while (match({ token::bitwise_and })) {
      token op = previous();
      auto right = parse_addition();
      left = expression_t(new binary(std::move(left), op, std::move(right)));
    }

    return left;
  }

  expression_t parse_addition() {
    auto left = parse_multiplication();
    while (match({ token::plus, token::minus })) {
      token op = previous();
      auto right = parse_multiplication();
      left = expression_t(new binary(std::move(left), op, std::move(right)));
    }

    return left;
  }

  expression_t parse_multiplication() {
    auto left = parse_exponentation();
    while (match({ token::star, token::slash, token::modulo })) {
      token op = previous();
      auto right = parse_exponentation();
      left = expression_t(new binary(std::move(left), op, std::move(right)));
    }
    return left;
  }

  expression_t parse_exponentation() {
    auto left = parse_unary();
    while (match({ token::caret })) {
      token op = previous();
      auto right = parse_exponentation();
      left = expression_t(new binary(std::move(left), op, std::move(right)));
    }

    return left;
  }

  expression_t parse_unary() {
    if (match({ token::minus, token::bitwise_not })) {
      token op = previous();
      auto right = parse_unary();
      return expression_t(new unary(op, std::move(right)));
    }

    return parse_primary();
  }

  expression_t parse_primary() {
    if (match({ token::integer, token::decimal, token::binary, token::octal, token::hex })) {
      return expression_t(new literal(previous()));
    }

    if (match({ token::lparen })) {
      auto expr = parse_expression();
      consume(token::rparen, "Expected '(' after expression");
      return expression_t(new group(std::move(expr)));
    }

    throw error("Expected a primary expression");
  }

  bool match(std::vector<token::token_type> types) {
    for (auto& type : types) {
      if (check(type)) {
        next();
        return true;
      }
    }
    return false;
  }

  bool check(token::token_type type) {
    if (eos()) return false;
    return peek().type == type;
  }

  token peek() {
    return source[position];
  }

  token previous() {
    return source[position - 1];
  }

  token next() {
    if (!eos()) position++;
    return previous();
  }

  token consume(token::token_type type, const std::string& message) {
    if (check(type)) return next();
    throw error(message);
  }

  std::runtime_error error(const std::string& message) {
    return std::runtime_error("Parse error: " + message);
  }

  bool eos() {
    return position >= source.size() || peek().type == token::eof;
  }
};

} // calc

#endif