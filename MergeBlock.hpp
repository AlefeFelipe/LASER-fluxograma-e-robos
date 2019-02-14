#ifndef MERGEBLOCK_HPP_
#define MERGEBLOCK_HPP_

#include "ProgrammingBlock.hpp"

using namespace std;


class MergeBlock : public ProgrammingBlock {

    Block next;

public:

    MergeBlock();
    ~MergeBlock();
    void setNext(Block b);
    Block getNext();

};
#endif
