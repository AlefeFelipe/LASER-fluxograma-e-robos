#include "Block.hpp"

Block :: Block() {
    x = 0;
    y = 0;
    width = 0;
    height = 0;
    selected = false;
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
    return 0;
}
void Block :: setWidth(int w) {
    width = w;
}
int Block :: getWidth() {
    return width;
}
void Block :: setHeight(int h) {
    height = h;
}
int Block :: getHeight() {
    return height;
}
