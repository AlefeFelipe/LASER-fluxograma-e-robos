#include "StartBlock.hpp"

StartBlock :: StartBlock() {
    type = 6;
}

StartBlock :: ~StartBlock() {

}

void StartBlock :: setNext(Block b) {
    next = b;
}

Block StartBlock :: getNext() {
    return next;
}
