#include "LoopBlock.hpp"

LoopBlock :: LoopBlock() {
    type = 7;
}

LoopBlock :: ~LoopBlock() {

}

void LoopBlock :: setLimitedLoop(bool l) {
    limited_loop = l;
}

bool LoopBlock :: getLimitedLoop() {
    return limited_loop;
}

void LoopBlock :: setNumberOfLoops(int n) {
    number_of_loops = n;
}

int LoopBlock :: getNumberOfLoops() {
    return number_of_loops;
}

void LoopBlock :: decNumberOfLoops() {
    number_of_loops = number_of_loops - 1;
}

void LoopBlock :: setNextLoop(Block b) {
    next_loop = b;
}

Block LoopBlock :: getNextLoop() {
    return next_loop;
}

void LoopBlock :: setNextProgram(Block b) {
    next_program = b;
}

Block LoopBlock :: getNextProgram() {
    return next_program;
}
