#ifndef CALC_AST_PRINTER_HPP
#define CALC_AST_PRINTER_HPP
#include "grammar/expression.hpp"
#include <string>
#include <vector>
#include <iostream>

namespace core {
struct ast_printer : public expression::visitor {
  ast_printer(): ast("") {};

  std::string ast;
  virtual void visit_unary(unary& expr) override {
      parenthesize(expr.op.lexeme, { expr.right.get() });
  }
  
  virtual void visit_binary(binary& expr) override {
    parenthesize(expr.op.lexeme, { expr.left.get(), expr.right.get() });
  }
  
  virtual void visit_literal(literal& expr) override {
    ast += expr.value.lexeme;
  }
  
  virtual void visit_group(group& expr) override {
    parenthesize("group",{ expr.grouped.get() });
  }

  void print(expression* expr) {
    expr->accept(*this);
    std::cout << ast << std::endl;
    ast = "";
  }

  private:
    void parenthesize(std::string name, std::vector<expression*> expressions) {
      ast += "(" + name;
      for(auto expr: expressions) {
        ast += " ";
        expr->accept(*this);
      }
      ast += ")";
    }
};
} // core

#endif