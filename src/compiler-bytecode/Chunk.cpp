//
// Created by kai on 4/17/19.
//

#include "Chunk.h"


Chunk::Chunk()
:code_(std::make_shared<std::vector<uint8_t>>()),
 constants_(std::make_shared<std::vector<Value>>())
{

}

void Chunk::appendSimpleOp(OpCode opCode, int lineNumber)
{
    appendOp(opCode, lineNumber);
}

void Chunk::appendConstant(Value value, int lineNumber)
{
    appendOp(OP_CONSTANT, lineNumber);
    constants_->push_back(value);
    auto constantPos = constants_->size() -1;
    appendOp(constantPos, lineNumber);
}

void Chunk::clear()
{
    lineNumbers_.clear();
    code_->clear();
    constants_->clear();
}

int Chunk::lineNumberOf(Code::size_type opCodePos) const
{
    return lineNumbers_.at(opCodePos);
}

void Chunk::appendOp(uint8_t opCode, int lineNumber)
{
    code_->push_back(opCode);

    // TODO better encoding
    auto opCodePos = code_->size() - 1;
    lineNumbers_.push_back(lineNumber);
}

std::shared_ptr<Code> Chunk::code() const
{
    return code_;
}

std::shared_ptr<Values> Chunk::constants() const
{
    return constants_;
}
