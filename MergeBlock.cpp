#include "MergeBlock.hpp"

MergeBlock :: MergeBlock() {
    type = 9;
}

MergeBlock :: ~MergeBlock() {

}

void MergeBlock :: setNext(Block b) {
    next = b;
}

Block MergeBlock :: getNext() {
    return next;
}
