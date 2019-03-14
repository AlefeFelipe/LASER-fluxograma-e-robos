#ifndef ENDBLOCK_HPP_
#define ENDBLOCK_HPP_

#include "ProgrammingBlock.hpp"

using namespace std;


class EndBlock : public ProgrammingBlock {

public:

    EndBlock();
    ~EndBlock();
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
