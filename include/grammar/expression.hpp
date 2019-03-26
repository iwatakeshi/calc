#ifndef CALC_GRAMMAR_HPP
#define CALC_GRAMMAR_HPP
#include "../token.hpp"
#include <memory>

namespace calc {

struct unary;
struct binary;
struct literal;
struct group;

struct expression {

  struct visitor {
    virtual void visit_unary(unary&) = 0;
    virtual void visit_binary(binary&) = 0;
    virtual void visit_literal(literal&) = 0;
    virtual void visit_group(group&) = 0;
  };

  virtual ~expression() {}

  virtual void accept(visitor&) = 0;
};

struct unary : public expression {
  unary(token op, std::unique_ptr<expression>&& right) :
      op(op), right(std::move(right)) {};

  virtual void accept(visitor& v) override {
    return v.visit_unary(*this);
  }

  token op;
  std::unique_ptr<expression> right;
};

struct binary : public expression {
  binary(std::unique_ptr<expression>&& left, token op, std::unique_ptr<expression> && right):
  left(std::move(left)), op(op), right(std::move(right)) {}
  
  virtual void accept(visitor& v) override {
    return v.visit_binary(*this);
  }

  token op;
  std::unique_ptr<expression> left;
  std::unique_ptr<expression> right;
};

struct literal : public expression {
  literal(token value): value(value) {}

  virtual void accept(visitor& v) override {
    return v.visit_literal(*this);
  }

  token value;
};

struct group : public expression {
  group(std::unique_ptr<expression>&& grouped): grouped(std::move(grouped)) {}

  virtual void accept(visitor& v) override {
    return v.visit_group(*this);
  }

  std::unique_ptr<expression> grouped;
};

} // calc

#endif