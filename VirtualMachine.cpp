//
// Created by kai on 4/18/19.
//

#include <iostream>
#include "VirtualMachine.h"

VirtualMachine::VirtualMachine()
    :stack_({})
{

}

InterpretResult VirtualMachine::interpret(const Chunk &chunk)
{
    ip_ = chunk.code()->data();

    return run(chunk);
}


InterpretResult VirtualMachine::run(const Chunk &chunk){

#define READ_BYTE() (*ip_++)
#define READ_CONSTANT() ((*chunk.constants())[READ_BYTE()])

    for (;;) {
        uint8_t instruction = READ_BYTE();
        // TODO dissassembleInstruction for debugging
        switch (instruction) {
            case OP_RETURN: {
                std::cout << "On top of the stack: " <<  stack_.back() << std::endl;
                return InterpretResult::OK;
            }
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                stack_.push_back(constant);
                break;
            }
            case OP_NEGATE: stack_.back() = -stack_.back(); break;

        }
    }
#undef READ_BYTE
#undef READ_CONSTANT
}
