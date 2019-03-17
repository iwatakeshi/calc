#include "include/ast_printer.hpp"
#include "include/grammar/expression.hpp"
#include "include/lexer.hpp"
#include "include/parser.hpp"
#include "include/interpreter.hpp"

#include <iostream>
#include <memory>
using namespace std;

int main() {

  core::lexer lexer;
  core::parser parser;
  core::ast_printer printer;
  core::interpreter interpreter;
  cout << "> ";
  cin >> lexer;

  try {
    lexer >> parser;
    auto expression = parser.parse();
    for (auto& expr : expression) {
      auto result = interpreter.eval(expr.get());
      if (result.second) {
        cout << int(result.first) << endl;
      } else {
        cout << result.first << endl;
      }
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }

  return 0;
}