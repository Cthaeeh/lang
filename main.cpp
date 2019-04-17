#include <iostream>

#include "Chunk.h"
#include "Debug.h"


int main() {

    Chunk testChunk;
    testChunk.code.push_back(OP_CONSTANT);
    auto constant = testChunk.addConstant(1.2);
    testChunk.code.push_back(constant);

    dissassembleChunk(testChunk,"test chunk");

    return 0;
}