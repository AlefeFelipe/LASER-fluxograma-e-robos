#ifndef LOOPBLOCK_HPP_
#define LOOPBLOCK_HPP_

#include "ProgrammingBlock.hpp"

using namespace std;


class LoopBlock : public ProgrammingBlock {

    int number_of_loops;
    Block next_loop;
    Block next_program;
    bool limited_loop;

public:

    LoopBlock();
    ~LoopBlock();
    void setLimitedLoop(bool l);
    bool getLimitedLoop();
    void setNumberOfLoops(int n);
    int getNumberOfLoops();
    void decNumberOfLoops();
    void setNextLoop(Block b);
    Block getNextLoop();
    void setNextProgram(Block b);
    Block getNextProgram();

};
#endif
