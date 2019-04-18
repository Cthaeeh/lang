#include <iostream>

#include "Chunk.h"
#include "Debug.h"
#include "VirtualMachine.h"


int main() {

    Chunk testChunk;
    testChunk.appendConstant(1.2, 123);
    testChunk.appendConstant(1.2, 123);
    testChunk.appendSimpleOp(OP_ADD, 123);

    dissassembleChunk(testChunk,"test chunk");

    VirtualMachine vm;
    vm.interpret(testChunk);

    return 0;
}