//biblioteca feita para leitura dos ultrassons

#ifndef _MYULTRASONIC_HPP_
#define _MYULTRASONIC_HPP_
//#include <Ultrasonic.h>
#include <NewPing.h>
#include "robotCte.hpp"



    void getDistance();
    float* isDistanceAvailable();
    void initUltrasonics();
    void oneSensorCycle() ;

#endif
