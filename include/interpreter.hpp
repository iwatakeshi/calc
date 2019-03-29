#ifndef CALC_INTERPRETER_HPP
#define CALC_INTERPRETER_HPP
#include "grammar/expression.hpp"
#include "token.hpp"
#include <cmath>
#include <iostream>
#include <stack>
#include <string>
#include <utility>
#include <vector>

namespace calc {
struct interpreter : public expression::visitor {
  interpreter() {}
  // https://stackoverflow.com/a/32378819
  std::stack<token> stack;

  virtual void visit_unary(unary& expr) override {
    visit(expr.right.get());
    token right = stack.top();
    stack.pop();
    if (expr.op.type == token::minus) {
      stack.push(token(right.type, std::to_string(-(right.literal)), -(right.literal)));
    } else if (expr.op.type == token::bitwise_not) {
      stack.push(token(right.type, std::to_string((~(int(right.literal)))), (~(int(right.literal)))));
    } else {
      stack.push(right);
    }
  }

  virtual void visit_binary(binary& expr) override {
    visit(expr.left.get());
    visit(expr.right.get());
    token right = stack.top();
    stack.pop();
    token left = stack.top();
    stack.pop();

    switch (expr.op.type) {
    case token::plus:
      push(left, expr.op, right, left.literal + right.literal);
      break;
    case token::minus:
      push(left, expr.op, right, left.literal - right.literal);
      break;
    case token::star:
      push(left, expr.op, right, left.literal * right.literal);
      break;
    case token::slash:
      push(left, expr.op, right, left.literal / right.literal);
      break;
    case token::modulo:
      push(left, expr.op, right, (int(left.literal) % int(right.literal)));
      break;
    case token::caret:
      push(left, expr.op, right, std::pow(left.literal, right.literal));
      break;
    case token::bitwise_and:
      push(left, expr.op, right, int(left.literal) & int(right.literal));
      break;
    case token::bitwise_or:
      push(left, expr.op, right, int(left.literal) | int(right.literal));
      break;
    case token::bitwise_xor:
      push(left, expr.op, right, int(left.literal) ^ int(right.literal));
      break;
    default:
      break;
    }
  }

  virtual void visit_literal(literal& expr) override {
    stack.push(expr.value);
  }

  virtual void visit_group(group& expr) override {
    visit(expr.grouped.get());
  }

  std::pair<double, bool> eval(expression* expr) {
    expr->accept(*this);
    token value = stack.top();
    stack.pop();
    return std::make_pair(value.literal, value.type == token::integer);
  }

  private:
  void push(token left, token op, token right, double value) {
    if (is_integer(left, right) && op.type != token::slash) {
      stack.push(token(token::integer, std::to_string(value), value));
    } else {
      stack.push(token(token::decimal, std::to_string(value), value));
    }
  }

  void visit(expression* expr) {
    expr->accept(*this);
  }

  bool is_integer(token left, token right) {
    return left.type == token::integer && right.type == token::integer;
  }
};
} // calc

#endif