#include "ColorSensorBlock.hpp"

ColorSensorBlock :: ColorSensorBlock() {
    type = 4;
}

ColorSensorBlock :: ~ColorSensorBlock() {

}

int *ColorSensorBlock :: getReading() {
    return reading;
}
