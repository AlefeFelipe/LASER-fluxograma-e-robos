#include "ConditionalBlock.hpp"

ConditionalBlock :: ConditionalBlock() {
    type = 8;
    pointIn1_x = x + 60;
    pointIn1_y = y;
    pointOut1_x = x + 119;
    pointOut1_y = y + 35;
    pointOut2_x = x + 60;
    pointOut2_y = y + 69;
    next_true = NULL;
    next_false = NULL;
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

void ConditionalBlock :: setPrevious(Block b) {
    previous = b;
}

Block ConditionalBlock :: getPrevious() {
    return previous;
}

void ConditionalBlock :: setNext1(Block *b) {
    next_true = b;
    cout<<"setou o next corretamente"<<endl;
    cout<<"origem: "<<getID()<<" ---- destino: "<<b->getID()<<endl;
}

Block* ConditionalBlock :: getNext1() {
    return next_true;
}

void ConditionalBlock :: setNext2(Block *b) {
    next_false = b;
}

Block* ConditionalBlock :: getNext2() {
    return next_false;
}

void ConditionalBlock :: setTypeOfSensor(int t) {
    type_of_sensor = t;
}

int ConditionalBlock :: getTypeOfSensor() {
    return type_of_sensor;
}

int ConditionalBlock :: getPointIn1X() {
    pointIn1_x = x + 60;
    return pointIn1_x;
}
int ConditionalBlock :: getPointIn1Y() {
    pointIn1_y = y;
    return pointIn1_y;
}
int ConditionalBlock :: getPointIn2X() {
    //nao tem
    return 0;
}
int ConditionalBlock :: getPointIn2Y() {
    //nao tem
    return 0;
}
int ConditionalBlock :: getPointOut1X() {
    pointOut1_x = x + 60;
    return pointOut1_x;
}
int ConditionalBlock :: getPointOut1Y() {
    pointOut1_y = y + 69;
    return pointOut1_y;
}
int ConditionalBlock :: getPointOut2X() {
    pointOut2_x = x + 119;
    return pointOut2_x;
}
int ConditionalBlock :: getPointOut2Y() {
    pointOut2_y = y + 35;
    return pointOut2_y;
}
void ConditionalBlock :: setIn1(bool in) {

}
bool ConditionalBlock :: getIn1() {
    return true;
}
