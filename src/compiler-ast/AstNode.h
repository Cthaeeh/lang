//
// Created by kai on 4/19/19.
//

#ifndef VMTEST_ASTNODE_H
#define VMTEST_ASTNODE_H

#include <Visitor.h>
#include <memory>

class AstNode {
public:

    virtual void accept(Visitor& v) = 0;

    std::weak_ptr<AstNode> parent;
};


#endif //VMTEST_ASTNODE_H
