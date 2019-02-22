#include "Block.hpp"

Block :: Block() {
    x = 0;
    y = 0;
    width = 0;
    height = 0;
    selected = false;
    dragging = false;
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

void Block :: setDragging(bool s) {
    dragging = s;
}

bool Block :: getDragging() {
    return dragging;
}

void Block :: setSelected(bool s) {
    selected = s;
}
bool Block :: getSelected() {
    return selected;
}

void Block :: setOutSelected(bool s) {
    out_selected = s;
}

bool Block :: getOutSelected() {
    return out_selected;
}

void Block :: setInSelected(bool s) {
    in_selected = s;
}

bool Block :: getInSelected() {
    return in_selected;
}

int Block :: getType() {
    return type;
}

void Block :: setTypeOfSensor(int t) {

}
int Block :: getTypeOfSensor() {
    return 0;
}

void Block :: setFunction(int f) {

}
int Block :: getFunction() {
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
