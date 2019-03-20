#include "BlackSensorBlock.hpp"

BlackSensorBlock :: BlackSensorBlock() {
    type = 3;
}

BlackSensorBlock :: ~BlackSensorBlock() {

}

int *BlackSensorBlock :: getReading() {
    return reading;
}
