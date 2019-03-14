#ifndef STARTBLOCK_HPP_
#define STARTBLOCK_HPP_

#include "ProgrammingBlock.hpp"

using namespace std;


class StartBlock : public ProgrammingBlock {

    Block* next;

public:

    StartBlock();
    ~StartBlock();
    void setNext1(Block *b);
    Block* getNext1();
    void setNext2(Block *b);
    Block* getNext2();
    int getPointIn1X();
    int getPointIn1Y();
    int getPointIn2X();
    int getPointIn2Y();
    int getPointOut1X();
    int getPointOut1Y();
    int getPointOut2X();
    int getPointOut2Y();
    void setIn1(bool in);
    bool getIn1();
};
#endif
