#ifndef CALC_REPL_HPP
#define CALC_REPL_HPP
#include "output.hpp"
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace calc {

struct REPL {


  void operator()(int argc, char* argv[], const std::string v, std::function<void(std::string, bool, output)> callback) {
    std::vector<std::string> args(argv, argv + argc);
    bool repeat = false;
    bool tree = false;
    output::type type = output::decimal;
    std::string input;

    if (contains(args, "--help") || contains(args, "-h")) {
      usage();
    }

    if (contains(args, "--version") || contains(args, "-v")) {
      version(v);
    }

    if (contains(args, "--repl") || contains(args, "-r")) {
      repeat = true;
    }

    if (contains(args, "--tree") || contains(args, "-t")) {
      tree = true;
    }

    if (contains(args, "--output-binary") || contains(args, "-b")) {
      type = output::binary;
    }

    if (contains(args, "--output-octal") || contains(args, "-o")) {
      type = output::octal;
    }

    if (contains(args, "--output-hex") || contains(args, "-x")) {
      type = output::hex;
    }

    auto invalid_options = filter(args, [&](std::string option) {
      return !is_option(option) && option != args[0];
    });

    if (invalid_options.size() > 0) {
      std::cout << "calc: "
                << "invalid option " << to_string(invalid_options) << std::endl;
      usage();
    }

    if (repeat) {
      while (true) {
        std::getline(std::cin, input);
        if (should_quit(input)) exit(0);
        callback(input, tree, output(type));
      }
    } else {
      std::getline(std::cin, input);
      if (should_quit(input)) exit(0);
      callback(input, tree, output(type));
    }
  }

  bool contains(const std::vector<std::string>& v, const std::string& flag) {
    return std::find(v.begin(), v.end(), flag) != v.end();
  }

  std::vector<std::string> filter(const std::vector<std::string>& v, std::function<bool(std::string)> callback) {
    std::vector<std::string> temp;
    for (auto& element : v) {
      if (callback(element)) {
        temp.push_back(element);
      }
    }
    return temp;
  }

  std::string to_string(const std::vector<std::string>& v) {
    std::string temp;
    for (auto& element : v) {
      temp += element;
      temp += " ";
    }
    return temp;
  }

  void usage() {
    std::cout << "usage: "
              << "calc "
              << "[options]" << std::endl
              << "-h  --help           print this usage and exit" << std::endl
              << "-v  --version        print version information and exit" << std::endl
              << "-r  --repl           use read-eval-print loop mode" << std::endl
              << "-t  --tree           print the syntax tree" << std::endl
              << "-b  --output-binary  print the result in binary" << std::endl
              << "-o  --output-octal   print the result in octal" << std::endl
              << "-x  --output-hex     print the result in hexidecimal" << std::endl;

    exit(0);
  }

  void version(const std::string& v) {
    std::cout << "calc " << v << std::endl
              << "MIT License" << std::endl
              << "Copyright 2019 Takeshi Iwana" << std::endl;
    exit(0);
  }

  bool should_quit(std::string input) {
    if (input == "quit" || input == "exit" || input == "q") {
      return true;
    }
    return false;
  }

  bool is_option(std::string option) {
    if (option == "--help" || option == "-h") {
      return true;
    }

    if (option == "--version" || option == "-v") {
      return true;
    }

    if (option == "--repl" || option == "-r") {
      return true;
    }

    if (option == "--tree" || option == "-t") {
      return true;
    }

    if (option == "--output-binary" || option == "-b") {
      return true;
    }

    if (option == "--output-octal" || option == "-o") {
      return true;
    }

    if (option == "--output-hex" || option == "-x") {
      return true;
    }

    return false;
  }
} repl;

} // calc

#endif