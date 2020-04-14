//
// Created by kai on 4/21/19.
//

#include "CodeGen.h"
#include "Expr.h"

namespace aeeh {
namespace backend {
namespace detail {

// TODO move this to some utility header.
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>; // not needed as of C++20

Chunk generate(const ast::Expr& expr) {

  auto result = Chunk();
  return result;
  //std::visit(overloaded {
  //          [&result](const auto &e) { result = generate(e); },
  //      }, expr);
  //return result;
}
//
//Chunk generate(const ast::BinaryExpr &el) {
//
//  auto chunk = generate(*el.left);
//  auto chunk = generate(*el.left);
//
//  el.left->accept(*this);
//  el.right->accept(*this);
//
//  switch (el.op.type()) {
//  case Token::PLUS:
//    chunk.appendSimpleOp(OP_ADD, el.op.line());
//    break;
//  case Token::MINUS:
//    chunk.appendSimpleOp(OP_SUBTRACT, el.op.line());
//    break;
//  case Token::STAR:
//    chunk.appendSimpleOp(OP_MULTIPLY, el.op.line());
//    break;
//  case Token::SLASH:
//    chunk.appendSimpleOp(OP_DIVIDE, el.op.line());
//    break;
//  default:
//    throw std::runtime_error(
//        "Invalid operand of binary expr -> parser what did u do ?");
//  }
//}
//
//Chunk generate(const ast::UnaryExpr &el) {
//  auto chunk = generate(*el.right);
//  switch (el.op.type()) {
//  case Token::MINUS:
//    chunk.appendSimpleOp(OP_NEGATE, el.op.line());
//    break;
//  default:
//    // TODO solve this without exceptions.
//    throw std::runtime_error(
//        "Invalid operand of binary expr -> parser what did u do ?");
//  }
//  return chunk;
//}
//
//Chunk generate(const ast::LiteralExpr &el) {
//  auto chunk = Chunk();
//  chunk.appendConstant(std::stod(el.literal.lexeme()), el.literal.line());
//  return Chunk();
//}

} // namespace detail

Chunk generate(const ast::Expr &expr) {
  return detail::generate(expr);
}

} // namespace backend
} // namespace aeeh
