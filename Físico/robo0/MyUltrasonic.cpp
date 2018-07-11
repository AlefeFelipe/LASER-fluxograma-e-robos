#include "MyUltrasonic.hpp"

MyUltrasonic::MyUltrasonic(int name)
{
    switch (name)
    {
        case 1:
            ultrasonic = new Ultrasonic(L_ULTRASONIC_TRIG, L_ULTRASONIC_ECHO);
        break;
        case 2:
            ultrasonic = new Ultrasonic(LM_ULTRASONIC_TRIG, LM_ULTRASONIC_ECHO);
        break;
        case 3:
            ultrasonic = new Ultrasonic(M_ULTRASONIC_TRIG, M_ULTRASONIC_ECHO);
        break;
        case 4:
            ultrasonic = new Ultrasonic(RM_ULTRASONIC_TRIG, RM_ULTRASONIC_ECHO);
        break;
        case 5:
            ultrasonic = new Ultrasonic(R_ULTRASONIC_TRIG, R_ULTRASONIC_ECHO);
        break;
    }
    distance = 0;
}

float MyUltrasonic::getDistance()
{
    unsigned long time;
    time = ultrasonic->timing();
    distance = ultrasonic->convert(time, Ultrasonic::CM);
    //Serial.println(distance);
    return distance;
}
