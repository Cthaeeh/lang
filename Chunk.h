//
// Created by kai on 4/17/19.
//

#ifndef VMTEST_CHUNK_H
#define VMTEST_CHUNK_H

#include <vector>
#include <stdint.h>


enum OpCode : uint8_t {
    OP_RETURN,
    OP_CONSTANT
};

typedef std::vector<uint8_t> Code;
typedef double Value;
typedef std::vector<Value> Values;

struct Chunk{
    Code code;
    Values constants;

    uint8_t addConstant(Value value){
        constants.push_back(value);
        return constants.size() - 1;
    }
};

#endif //VMTEST_CHUNK_H
