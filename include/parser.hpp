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

namespace core {
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

  friend void operator >>(lexer& lexer, parser& parser) {
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
    return parse_addition();
  }

  expression_t parse_addition() {
    auto left = parse_multiplication();
    while (match({ token_type::plus, token_type::minus })) {
      token op = previous();
      auto right = parse_multiplication();
      left = expression_t(new binary(std::move(left), op, std::move(right)));
    }

    return left;
  }

  expression_t parse_multiplication() {
    auto left = parse_exponentation();
    while (match({ token_type::star, token_type::slash, token_type::modulo })) {
      token op = previous();
      auto right = parse_exponentation();
      left = expression_t(new binary(std::move(left), op, std::move(right)));
    }
    return left;
  }

  expression_t parse_exponentation() {
    auto left = parse_unary();
    while (match({ token_type::caret })) {
      token op = previous();
      auto right = parse_exponentation();
      left = expression_t(new binary(std::move(left), op, std::move(right)));
    }

    return left;
  }

  expression_t parse_unary() {
    if (match({ token_type::minus })) {
      token op = previous();
      auto right = parse_unary();
      return expression_t(new unary(op, std::move(right)));
    }

    return parse_primary();
  }

  expression_t parse_primary() {
    if (match({ token_type::integer, token_type::decimal })) {
      return expression_t(new literal(previous()));
    }

    if (match({ token_type::lparen })) {
      auto expr = parse_expression();
      consume(token_type::rparen, "Expected '(' after expression");
      return expression_t(new group(std::move(expr)));
    }

    throw error("Expected a primary expression");
  }

  bool match(std::vector<token_type> types) {
    for (auto& type : types) {
      if (check(type)) {
        next();
        return true;
      }
    }
    return false;
  }

  bool check(token_type type) {
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

  token consume(token_type type, const std::string& message) {
    if (check(type)) return next();
    throw error(message);
  }

  std::runtime_error error(const std::string& message) {
    return std::runtime_error("Parse error: " + message);
  }

  bool eos() {
    return position >= source.size() || peek().type == token_type::eof;
  }
};

} // core

#endif