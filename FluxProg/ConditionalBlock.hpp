#ifndef CONDITIONALBLOCK_HPP_
#define CONDITIONALBLOCK_HPP_

#include "ProgrammingBlock.hpp"

using namespace std;


class ConditionalBlock : public ProgrammingBlock {

    int parameter1;
    int parameter2;
    int type_of_sensor;
    Block* previous;
    Block* next_true;
    Block* next_false;

    bool getResponse();

public:

    ConditionalBlock();
    ~ConditionalBlock(){};
    void setParameter1(int p);
    int getParameter1();
    void setParameter2(int p);
    int getParameter2();
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
    Block* getExecutingNext();
};
#endif
