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

void Block :: setSelected(int s) {
    selected = s;
}

int Block :: getSelected() {
    return selected;
}
