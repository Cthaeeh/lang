//
// Created by kai on 4/17/19.
//

#include "Chunk.h"


Chunk::Chunk()
:code_(std::make_shared<std::vector<uint8_t>>()),
 constants_(std::make_shared<std::vector<Value>>())
{

}

uint8_t Chunk::addConstant(Value value){
    constants_->push_back(value);
    return constants_->size() - 1;
}

Code Chunk::code() const
{
    return code_;
}

Values Chunk::constants() const
{
    return constants_;
}
