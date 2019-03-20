#ifndef COLORSENSORBLOCK_HPP_
#define COLORSENSORBLOCK_HPP_

#include "SensorBlock.hpp"

using namespace std;


class ColorSensorBlock : public SensorBlock {
    int reading[];

public:

    ColorSensorBlock();
    ~ColorSensorBlock();
    int *getReading();

};
#endif
