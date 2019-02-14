#include "UltrasonicSensorBlock.hpp"

UltrasonicSensorBlock :: UltrasonicSensorBlock() {
    type = 2;
}

UltrasonicSensorBlock :: ~UltrasonicSensorBlock() {

}

int *UltrasonicSensorBlock :: getDistance() {
    return distance;
}
