#include <iostream>

#include "Chunk.h"
#include "Debug.h"


int main() {

    Chunk testChunk;
    testChunk.appendConstant(1.2, 123);

    dissassembleChunk(testChunk,"test chunk");

    return 0;
}