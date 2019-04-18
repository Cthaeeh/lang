#include <iostream>

#include "Chunk.h"
#include "Debug.h"
#include "VirtualMachine.h"


int main() {

    Chunk testChunk;
    testChunk.appendConstant(1.2, 123);
    testChunk.appendSimpleOp(OP_NEGATE,124);
    dissassembleChunk(testChunk,"test chunk");

    VirtualMachine vm;
    vm.interpret(testChunk);

    return 0;
}