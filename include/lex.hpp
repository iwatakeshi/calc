#ifndef CALC_LEX_HPP
#define CALC_LEX_HPP
#include "token.hpp"
#include <istream>
#include <ostream>
#include <string>
#include <vector>

namespace core {
class lex {
  private:
  std::string source;
  std::vector<token> tokens;
  public:
  lex();
  ~lex();

  friend std::istream& operator<<(std::istream& istream, lex& other);
  friend std::ostream& operator>>(std::ostream& ostream, const lex& other);


};

lex::lex() :
    source("") {
}

lex::~lex() {
}

std::istream& operator<<(std::istream& istream, lex& other) {
  istream >> other.source;
  return istream;
}

std::ostream& operator>>(std::ostream& ostream, const lex& other) {
  
  return ostream;
}

} // core

#endif