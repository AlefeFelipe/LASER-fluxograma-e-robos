#ifndef LOOPBLOCK_HPP_
#define LOOPBLOCK_HPP_

#include "ProgrammingBlock.hpp"

using namespace std;


class LoopBlock : public ProgrammingBlock {

    int number_of_loops, number_of_loops_initial;
    Block* next_loop;
    Block* next_program;
    Block* previous_program;
    Block* previous_loop;
    bool limited_loop;
    int unit;
    int ten;
    bool setting_unit;

    void decNumberOfLoops();

public:

    LoopBlock();
    ~LoopBlock(){};
    void setLimitedLoop(bool l);
    bool getLimitedLoop();
    void setNumberOfLoops(int n);
    int getNumberOfLoops();
    void setNext1(Block *b);
    Block* getNext1();
    void setPrevious1(Block *b);
    Block* getPrevious1();
    void setPrevious2(Block *b);
    Block* getPrevious2();
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
    Block* getExecutingNext();
    void reset_loop_variables();
    void setUnit(int u);
    int getUnit();
    void setTen(int t);
    int getTen();
    void setValue(int v);
};
#endif
