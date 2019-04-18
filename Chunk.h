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
    OP_CONSTANT
};

typedef std::shared_ptr<std::vector<uint8_t>> Code;
typedef double Value;
typedef std::shared_ptr<std::vector<Value>> Values;

class Chunk{

public:
    Chunk();

    uint8_t addConstant(Value value);

    Code code() const;

    Values constants() const;

private:

    Code code_;
    Values constants_;
};

#endif //VMTEST_CHUNK_H
