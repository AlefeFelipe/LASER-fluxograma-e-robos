#include "ActionBlock.hpp"

ActionBlock :: ActionBlock() {
    type = ACTION_BLOCK;
    next = NULL;
    function = 0;
}

void ActionBlock :: setFunction(int f) {
    function = f;
}

int ActionBlock :: getFunction() {
    return function;
}

void ActionBlock :: setNext1(Block *b) {
    next = b;
}
Block* ActionBlock :: getNext1() {
    return next;
}
void ActionBlock :: setNext2(Block *b) {
    //não possui
}
Block* ActionBlock :: getNext2() {
    //não possui
    return NULL;
}
void ActionBlock :: setPrevious1(Block *b) {
    previous = b;
}
Block* ActionBlock :: getPrevious1() {
    return previous;
}
void ActionBlock :: setPrevious2(Block *b) {
    //nao tem
}
Block* ActionBlock :: getPrevious2() {
    //nao tem
    return NULL;
}
int ActionBlock :: getPointIn1X() {
    pointIn1_x = x + 47;
    return pointIn1_x;
}
int ActionBlock :: getPointIn1Y() {
    pointIn1_y = y;
    return pointIn1_y;
}
int ActionBlock :: getPointIn2X() {
    //nao tem
    return 0;
}
int ActionBlock :: getPointIn2Y() {
    //nao tem
    return 0;
}
int ActionBlock :: getPointOut1X() {
    pointOut1_x = x + 47;
    return pointOut1_x;
}
int ActionBlock :: getPointOut1Y() {
    pointOut1_y = y + 42;
    return pointOut1_y;
}
int ActionBlock :: getPointOut2X() {
    //nao tem
    return 0;
}
int ActionBlock :: getPointOut2Y() {
    //nao tem;
    return 0;
}
Block* ActionBlock :: getExecutingNext() {
    return next;
}
int ActionBlock :: getCommand() {
    return function;
}
