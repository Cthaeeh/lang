//
// Created by kai on 4/17/19.
//

#include "Debug.h"
#include <iostream>
#include <iomanip>

void dissassembleChunk(const Chunk &chunk, const std::string &name)
{
    std::cout << "|===== " << name << " =====|" << std::endl;
    std::cout << "| line |" <<  " opCode " << "| info |" << std::endl;

    // TODO write this more beautiful
    for (auto i = 0; i < chunk.code()->size() ; i++) {
        std::cout << "| "<< std::setw(4) << std::setfill('0') << chunk.lineNumberOf(i) << " | ";
        switch(chunk.code()->at(i)){
            case OP_RETURN:
                std::cout << "OP_RETURN" << std::endl;
                break;
            case OP_CONSTANT: {
                i++;
                auto constant = chunk.code()->at(i);
                std::cout << "OP_CONSTANT " << chunk.constants()->at(constant) << std::endl;
                break;
            }
            default:
                std::cout << "Unknown opcode " << chunk.code()->at(i) << std::endl;
        }
    }
}
