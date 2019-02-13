#ifndef ACTIONBLOCK_HPP_
#define ACTIONBLOCK_HPP_

#include "Block.hpp"

using namespace std;


class ActionBlock : public Block {
    int function;
    Block next;

public:

    ActionBlock();
    ~ActionBlock();
    void setFunction(int f);
    int getFunction();
    void setNext(Block b);
    Block getNext();

};
#endif
