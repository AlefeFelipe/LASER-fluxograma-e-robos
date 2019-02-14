#ifndef ULTRASONICSENSORBLOCK_HPP_
#define ULTRASONICSENSORBLOCK_HPP_

#include "SensorBlock.hpp"

using namespace std;


class UltrasonicSensorBlock : public SensorBlock {
    int distance[];

public:

    UltrasonicSensorBlock();
    ~UltrasonicSensorBlock();
    int *getDistance();

};
#endif
