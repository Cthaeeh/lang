//
// Created by kai on 4/17/19.
//

#ifndef VMTEST_CHUNK_H
#define VMTEST_CHUNK_H

#include <vector>
#include <stdint.h>
#include <memory>


enum OpCode : uint8_t {
    OP_RETURN,
    OP_CONSTANT,
    OP_NEGATE,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE
};

typedef std::vector<uint8_t> Code;

typedef double Value;
typedef std::vector<Value> Values;


class Chunk{

public:
    Chunk();

    void appendSimpleOp(OpCode opCode, int lineNumber);

    void appendConstant(Value value, int lineNumber);

    int lineNumberOf(Code::size_type opCodePos) const;

    std::shared_ptr<Code> code() const;

    std::shared_ptr<Values> constants() const;

private:

    void appendOp(uint8_t opCode, int lineNumber);

    void saveLineNumber(int lineNumber, std::vector<uint8_t>::size_type opCodePos);

    // TODO better encode them
    std::vector<int> lineNumbers_;
    std::shared_ptr<Code> code_;
    std::shared_ptr<Values> constants_;
};

#endif //VMTEST_CHUNK_H
