#include "include/lexer.hpp"
#include <iostream>
using namespace std;


int main() {

  core::lexer lexer;

  cout << "> ";
  cin >> lexer;

  lexer.scan();

  for(auto x : lexer) {
    cout << x << endl;
  }

  return 0;
}