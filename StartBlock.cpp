#include "StartBlock.hpp"

StartBlock :: StartBlock() {
    type = 6;
    next = NULL;
}

StartBlock :: ~StartBlock() {

}

void StartBlock :: setNext1(Block *b) {
    next = b;
}
Block* StartBlock :: getNext1() {
    return next;
}
void StartBlock :: setNext2(Block *b) {

}
Block* StartBlock :: getNext2() {
    return NULL;
}
void StartBlock :: setPrevious1(Block *b) {
    //nao tem
}
Block* StartBlock :: getPrevious1() {
    //nao tem
    return NULL;
}
void StartBlock :: setPrevious2(Block *b) {
    //nao tem
}
Block* StartBlock :: getPrevious2() {
    //nao tem
    return NULL;
}
int StartBlock :: getPointIn1X() {
    //nao tem
    return 0;
}
int StartBlock :: getPointIn1Y() {
    //nao tem
    return 0;
}
int StartBlock :: getPointIn2X() {
    //nao tem
    return 0;
}
int StartBlock :: getPointIn2Y() {
    //nao tem
    return 0;
}
int StartBlock :: getPointOut1X() {
    pointOut1_x = x + 44;
    return pointOut1_x;
}
int StartBlock :: getPointOut1Y() {
    pointOut1_y = y + 34;
    return pointOut1_y;
}
int StartBlock :: getPointOut2X() {
    //nao tem
    return 0;
}
int StartBlock :: getPointOut2Y() {
    //nao tem
    return 0;
}
void StartBlock :: setIn1(bool in) {

}
bool StartBlock :: getIn1() {
    return false;
}
