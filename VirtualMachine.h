//
// Created by kai on 4/18/19.
//

#ifndef VMTEST_VIRTUALMACHINE_H
#define VMTEST_VIRTUALMACHINE_H


#include "Chunk.h"


enum class InterpretResult{
    OK,
    COMPILE_ERROR,
    RUNTIMER_ERROR,
};

class VirtualMachine {

public:
    VirtualMachine();

    InterpretResult interpret(const Chunk &chunk);

    InterpretResult run(const Chunk &chunk);

private:

    std::vector<Value> stack_;

    uint8_t* ip_;
};


#endif //VMTEST_VIRTUALMACHINE_H
