//biblioteca feita para leitura dos ultrassons

#ifndef _MYULTRASONIC_HPP_
#define _MYULTRASONIC_HPP_
#include <Ultrasonic.h>
#include "robotCte.hpp"

class MyUltrasonic{
    Ultrasonic* ultrasonic;
    float distance;
public:
    MyUltrasonic(int name);
    float getDistance();
    ~MyUltrasonic(){}

};

#endif
