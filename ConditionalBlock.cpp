#include "ConditionalBlock.hpp"

ConditionalBlock :: ConditionalBlock() {
    type = 8;
}

ConditionalBlock :: ~ConditionalBlock() {

}

void ConditionalBlock :: setParameter1(float p) {
    parameter1 = p;
}

float ConditionalBlock :: getParameter1() {
    return parameter1;
}

void ConditionalBlock :: setParameter2(float p) {
    parameter2 = p;
}

float ConditionalBlock :: getParameter2() {
    return parameter2;
}

bool ConditionalBlock :: getResponse() {
    if(parameter1 < parameter2) {
        return true;
    } else {
        return false;
    }
}

void ConditionalBlock :: setNextTrue(Block b) {
    next_true = b;
}

Block ConditionalBlock :: getNextTrue() {
    return next_true;
}

void ConditionalBlock :: setNextFalse(Block b) {
    next_false = b;
}

Block ConditionalBlock :: getNextFalse() {
    return next_false;
}
