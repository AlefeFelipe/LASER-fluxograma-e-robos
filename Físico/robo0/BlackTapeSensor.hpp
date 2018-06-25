//biblioteca feita para leitura dos sensores de fita preta

#ifndef _BLACKTAPESENSOR_HPP_
#define _BLACKTAPESENSOR_HPP_
#include "robotCte.hpp"
#include <Arduino.h>

class BlackTapeSensor{
    int pin;
    int reading;
public:
    BlackTapeSensor(int name);
    int getReading();
    ~BlackTapeSensor(){}

};

#endif
