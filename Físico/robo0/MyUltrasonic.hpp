//biblioteca feita para leitura dos ultrassons

#ifndef _MYULTRASONIC_HPP_
#define _MYULTRASONIC_HPP_
#include <Ultrasonic.h>
#include <NewPing.h>
#include "robotCte.hpp"

class MyUltrasonic{
    NewPing* ultrasonic;
    float distance;
public:
    MyUltrasonic(int name);
    void getDistance();
    float isDistanceAvailable();
    ~MyUltrasonic(){}

};

#endif
