#include "LoopBlock.hpp"

LoopBlock :: LoopBlock() {
    type = LOOP_BLOCK;
    next_loop = NULL;
    next_program = NULL;
    number_of_loops = 1;
    limited_loop = false;
}

void LoopBlock :: setLimitedLoop(bool l) {
    limited_loop = l;
}

bool LoopBlock :: getLimitedLoop() {
    return limited_loop;
}

void LoopBlock :: setNumberOfLoops(int n) {
    number_of_loops = 2*n;
    number_of_loops_initial = 2*n;
}

int LoopBlock :: getNumberOfLoops() {
    return number_of_loops;
}

void LoopBlock :: decNumberOfLoops() {
    if(limited_loop == true) {
        number_of_loops = number_of_loops - 1;
    }
}

void LoopBlock :: setNext1(Block *b) {
    next_program = b;
}
Block* LoopBlock :: getNext1() {
    return next_program;
}
void LoopBlock :: setNext2(Block *b) {
    next_loop = b;
}
Block* LoopBlock :: getNext2() {
    return next_loop;
}
void LoopBlock :: setPrevious1(Block *b) {
    previous_program = b;
}
Block* LoopBlock :: getPrevious1() {
    return previous_program;
}
void LoopBlock :: setPrevious2(Block *b) {
    previous_loop = b;
}
Block* LoopBlock :: getPrevious2() {
    return previous_loop;
}
int LoopBlock :: getPointIn1X() {
    pointIn1_x = x + 42;
    return pointIn1_x;
}
int LoopBlock :: getPointIn1Y() {
    pointIn1_y = y;
    return pointIn1_y;
}
int LoopBlock :: getPointIn2X() {
    pointIn2_x = x + 79;
    return pointIn2_x;
}
int LoopBlock :: getPointIn2Y() {
    pointIn2_y = y + 88;
    return pointIn2_y;
}
int LoopBlock :: getPointOut1X() {
    pointOut1_x = x + 42;
    return pointOut1_x;
}
int LoopBlock :: getPointOut1Y() {
    pointOut1_y = y + 88;
    return pointOut1_y;
}
int LoopBlock :: getPointOut2X() {
    pointOut2_x = x + 79;
    return pointOut2_x;
}
int LoopBlock :: getPointOut2Y() {
    pointOut2_y = y;
    return pointOut2_y;
}
Block* LoopBlock :: getExecutingNext() {
    decNumberOfLoops();
    if(number_of_loops < 0) {
        return next_program;
    } else {
        return next_loop;
    }
}
void LoopBlock :: reset_loop_variables() {
    number_of_loops = number_of_loops_initial;
}
