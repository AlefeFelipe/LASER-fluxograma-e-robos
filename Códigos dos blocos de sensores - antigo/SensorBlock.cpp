#include "SensorBlock.hpp"

SensorBlock :: SensorBlock() {

}

SensorBlock :: ~SensorBlock() {

}

void SensorBlock :: setSensorType(int s) {
    sensor_type = s;
}

int SensorBlock :: getSensorType() {
    return sensor_type;
}

void SensorBlock :: setNext(Block b) {
    next = b;
}

Block SensorBlock :: getNext() {
    return next;
}
