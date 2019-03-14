#include "MergeBlock.hpp"

MergeBlock :: MergeBlock() {
    type = 9;
    next = NULL;
}

MergeBlock :: ~MergeBlock() {

}


void MergeBlock :: setNext1(Block *b) {
    next = b;
}
Block* MergeBlock :: getNext1() {
    return next;
}
void MergeBlock :: setNext2(Block *b) {
    //não possui
}
Block* MergeBlock :: getNext2() {
    //não possui
    return NULL;
}
int MergeBlock :: getPointIn1X() {
    pointIn1_x = x + 2;
    return pointIn1_x;
}
int MergeBlock :: getPointIn1Y() {
    pointIn1_y = y + 2;
    return pointIn1_y;
}
int MergeBlock :: getPointIn2X() {
    pointIn2_x = x + 29;
    return pointIn2_x;
}
int MergeBlock :: getPointIn2Y() {
    pointIn2_y = y + 2;
    return pointIn2_y;
}
int MergeBlock :: getPointOut1X() {
    pointOut1_x = x + 15;
    return pointOut1_x;
}
int MergeBlock :: getPointOut1Y() {
    pointOut1_y = y + 24;
    return pointOut1_y;
}
int MergeBlock :: getPointOut2X() {
    //nao tem
    return 0;
}
int MergeBlock :: getPointOut2Y() {
    //nao tem
    return 0;
}
void MergeBlock :: setIn1(bool in) {
    in_1 = in;
}
bool MergeBlock :: getIn1() {
    return in_1;
}
