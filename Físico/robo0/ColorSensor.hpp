//biblioteca feita para leitura do sensor de cor

#ifndef _COLORSENSOR_HPP_
#define _COLORSENSOR_HPP_
#include <Wire.h>
#include "Adafruit_TCS34725softi2c.h"
#include "robotCte.hpp"

class ColorSensor{
    uint16_t colors[4];
    int sda_pin;
    int scl_pin;
    Adafruit_TCS34725softi2c *sensor;

public:
    ColorSensor(int name, int gain);
    uint16_t* getColors();
    ~ColorSensor(){}
};
#endif
