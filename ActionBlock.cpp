#include "ActionBlock.hpp"

ActionBlock :: ActionBlock() {
    type = 1;
}

ActionBlock :: ~ActionBlock() {

}

void ActionBlock :: setFunction(int f) {
    function = f;
}

int ActionBlock :: getFunction() {
    return function;
}

void ActionBlock :: setNext(Block b) {
    next = b;
}

Block ActionBlock :: getNext() {
    return next;
}
