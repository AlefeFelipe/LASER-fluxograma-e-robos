#ifndef STARTBLOCK_HPP_
#define STARTBLOCK_HPP_

#include "ProgrammingBlock.hpp"

using namespace std;


class StartBlock : public ProgrammingBlock {

    Block next;

public:

    StartBlock();
    ~StartBlock();
    void setNext(Block b);
    Block getNext();

};
#endif
