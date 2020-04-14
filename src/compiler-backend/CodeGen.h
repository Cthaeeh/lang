//
// Created by kai on 4/21/19.
//

#ifndef VMTEST_CODEGEN_H
#define VMTEST_CODEGEN_H


#include <Expr.h>
#include "../compiler-bytecode/Chunk.h"

namespace aeeh {
namespace backend {

Chunk generate(const ast::Expr &expr);

}
}

#endif //VMTEST_CODEGEN_H
