#ifndef LOADFILE_HPP_
#define LOADFILE_HPP_

#include <fstream>
#include "fluxProgCte.h"
#include "Block.hpp"
#include "StartBlock.hpp"
#include "ActionBlock.hpp"
#include "ConditionalBlock.hpp"
#include "EndBlock.hpp"
#include "LoopBlock.hpp"
#include "MergeBlock.hpp"

class LoadFile {

    ifstream in;

public:

    LoadFile();
    ~LoadFile();
    void load();


};
#endif
