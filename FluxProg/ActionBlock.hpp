#ifndef ACTIONBLOCK_HPP_
#define ACTIONBLOCK_HPP_

#include "Block.hpp"
#include "fluxProgCte.h"
using namespace std;


class ActionBlock : public Block {
    int function;
    Block* next;
    Block* previous;

public:

    ActionBlock();
    ~ActionBlock();
    void setFunction(int f);
    int getFunction();
    void setNext1(Block *b);
    Block* getNext1();
    void setNext2(Block *b);
    Block* getNext2();
    void setPrevious1(Block *b);
    Block* getPrevious1();
    void setPrevious2(Block *b);
    Block* getPrevious2();
    int getPointIn1X();
    int getPointIn1Y();
    int getPointIn2X();
    int getPointIn2Y();
    int getPointOut1X();
    int getPointOut1Y();
    int getPointOut2X();
    int getPointOut2Y();
    Block* getExecutingNext();
    int getCommand();
};
#endif
