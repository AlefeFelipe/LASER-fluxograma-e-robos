#ifndef CONDITIONALBLOCK_HPP_
#define CONDITIONALBLOCK_HPP_

#include "ProgrammingBlock.hpp"

using namespace std;


class ConditionalBlock : public ProgrammingBlock {

    float parameter1;
    float parameter2;
    int type_of_sensor;
    Block* previous;
    Block* next_true;
    Block* next_false;

public:

    ConditionalBlock();
    ~ConditionalBlock();
    void setParameter1(float p);
    float getParameter1();
    void setParameter2(float p);
    float getParameter2();
    bool getResponse();
    void setPrevious(Block b);
    Block getPrevious();
    void setNext1(Block *b);
    Block* getNext1();
    void setNext2(Block *b);
    Block* getNext2();
    void setPrevious1(Block *b);
    Block* getPrevious1();
    void setPrevious2(Block *b);
    Block* getPrevious2();
    void setTypeOfSensor(int t);
    int getTypeOfSensor();
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
