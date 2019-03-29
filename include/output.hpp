#ifndef CALC_OUTPUT_HPP
#define CALC_OUTPUT_HPP
#include <bitset>
#include <iostream>
#include <ostream>
#include <string>
#include <utility>
namespace calc {
struct output {
  enum type {
    decimal,
    binary,
    octal,
    hex
  };

  output(output::type type) :
      type_(type) {}

  friend void operator>>(std::pair<double, bool> result, output& other) {
    other.value = result.first;
    other.downcast = result.second;
  }

  friend std::ostream& operator<<(std::ostream& os, const output& other) {
    int integer_value = int(other.value);
    double double_value = other.value;

    switch (other.type_) {
    case output::type::decimal:
      if (other.downcast)
        os << integer_value;
      else
        os << double_value;
      break;
    case output::type::binary:
      if (other.downcast)
        os << output::format_binary(std::bitset<sizeof(int) * 8>(integer_value));
      else
        os << output::format_binary(std::bitset<sizeof(double) * 8>(double_value));
      break;
    case output::type::octal:
      if (other.downcast)
        os << std::oct << integer_value;
      else
        os << std::oct << double_value;
      break;
    case output::type::hex:
      if (other.downcast)
        os << std::hex << integer_value;
      else
        os << std::hex << double_value;
      break;
    default:
      break;
    }
    return os;
  }

  friend class std::basic_ostream<char>;
  bool downcast;
  double value;

  private:
  output::type type_;

  // http://www.cplusplus.com/forum/beginner/154369/
  template <size_t size>
  static std::string format_binary(const std::bitset<size>& bits) {
    std::string result;
    const std::string str_bits = bits.to_string(); // convert to a string of zeroes and ones

    // http://en.cppreference.com/w/cpp/string/basic_string/find
    const auto first_digit = str_bits.find('1'); // locate the first '1'

    if (first_digit != std::string::npos)
      result = str_bits.substr(first_digit);
    else
      result = "0"; // all the bits were zeroes
    return result;
  }
};
} // calc

#endif