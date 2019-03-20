#ifndef BLACKSENSORBLOCK_HPP_
#define BLACKSENSORBLOCK_HPP_

#include "SensorBlock.hpp"

using namespace std;


class BlackSensorBlock : public SensorBlock {
    int reading[];

public:

    BlackSensorBlock();
    ~BlackSensorBlock();
    int *getReading();

};
#endif
