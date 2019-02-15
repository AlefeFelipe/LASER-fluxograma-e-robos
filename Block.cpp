#include "Block.hpp"

Block :: Block() {

}

Block :: ~Block() {

}

void Block :: setX(int nx) {
    x = nx;
}

int Block :: getX() {
    return x;
}

void Block :: setY(int ny) {
    y = ny;
}

int Block :: getY() {
    return y;
}

void Block :: setSelected(bool s) {
    selected = s;
}

bool Block :: getSelected() {
    return selected;
}

int Block :: getType() {
    return type;
}

void Block :: setTypeOfSensor(int t) {

}
int Block :: getTypeOfSensor() {
    cout<<"block"<<endl;
    return 0;
}
