#include "EndBlock.hpp"

EndBlock :: EndBlock() {
    type = 5;
}

EndBlock :: ~EndBlock() {

}
void EndBlock :: setNext1(Block *b) {
    //não possui
}
Block* EndBlock :: getNext1() {
    //não possui
    return NULL;
}
void EndBlock :: setNext2(Block *b) {
    //não possui
}
Block* EndBlock :: getNext2() {
    //não possui
    return NULL;
}
int EndBlock :: getPointIn1X() {
    pointIn1_x = x + 44;
    return pointIn1_x;
}
int EndBlock :: getPointIn1Y() {
    pointIn1_y = y;
    return pointIn1_y;
}
int EndBlock :: getPointIn2X() {
    //nao tem
    return 0;
}
int EndBlock :: getPointIn2Y() {
    //nao tem
    return 0;
}
int EndBlock :: getPointOut1X() {
    //nao tem
    return 0;
}
int EndBlock :: getPointOut1Y() {
    //nao tem
    return 0;
}
int EndBlock :: getPointOut2X() {
    //nao tem
    return 0;
}
int EndBlock :: getPointOut2Y() {
    //nao tem;
    return 0;
}
void EndBlock :: setIn1(bool in) {

}
bool EndBlock :: getIn1() {
    return true;
}
