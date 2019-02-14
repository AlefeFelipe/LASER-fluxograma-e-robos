#ifndef PROGRAMMINGBLOCK_HPP_
#define PROGRAMMINGBLOCK_HPP_

#include "Block.hpp"

using namespace std;


class ProgrammingBlock : public Block {
    int programming_type;

public:

    ProgrammingBlock();
    ~ProgrammingBlock();
    int getProgrammingType();

};
#endif
