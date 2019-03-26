#include "include/ast_printer.hpp"
#include "include/interpreter.hpp"
#include "include/lexer.hpp"
#include "include/parser.hpp"
#include "include/repl.hpp"
#include <iostream>
#include <string>

using namespace std;

const string VERSION = "1.0.0-alpha";

int main(int argc, char* argv[]) {

  calc::lexer lexer;
  calc::parser parser;
  calc::ast_printer printer;
  calc::interpreter interpreter;

  calc::repl(argc, argv, VERSION, [&](string input, bool tree) {
    try {
      input >> lexer >> parser;
      for (auto& expr : parser.parse()) {
        if (tree) {
          printer.print(expr.get());
        } else {
          auto result = interpreter.eval(expr.get());
          if (result.second) {
            cout << int(result.first) << endl;
          } else {
            cout << result.first << endl;
          }
        }
      }
    } catch (const std::exception& e) {
      std::cerr << e.what() << '\n';
    }
  });

  return 0;
}