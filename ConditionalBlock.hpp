#ifndef CONDITIONALBLOCK_HPP_
#define CONDITIONALBLOCK_HPP_

#include "ProgrammingBlock.hpp"

using namespace std;


class ConditionalBlock : public ProgrammingBlock {

    float parameter1;
    float parameter2;
    Block next_true;
    Block next_false;

public:

    ConditionalBlock();
    ~ConditionalBlock();
    void setParameter1(float p);
    float getParameter1();
    void setParameter2(float p);
    float getParameter2();
    bool getResponse();
    void setNextTrue(Block b);
    Block getNextTrue();
    void setNextFalse(Block b);
    Block getNextFalse();

};
#endif
