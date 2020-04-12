//
// Created by kai on 4/20/19.
//

#include "NotSoPrettyPrinter.h"

namespace aeeh {
namespace ast {

// TODO move this to some utility header.
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>; // not needed as of C++20

std::string print(const Expr &expr) {
  auto result = std::string();
  std::visit(overloaded {
            [&result](const auto &e) { result = print(e); },
        }, expr);
  return result;
}

std::string print(const BinaryExpr &el) {
  return "[left: " + print(*(el.left)) + " op: " + toString(el.op) +
         " right: " + print(*(el.rigth)) + " ]";
}

std::string print(const UnaryExpr &el) {
  return "[right: " + print(*(el.right)) + " op: " + toString(el.op) + " ]";
}

std::string print(const LiteralExpr &el) {
  return "[literal: " + toString(el.literal) + "]";
}

} // namespace ast
} // namespace aeeh
