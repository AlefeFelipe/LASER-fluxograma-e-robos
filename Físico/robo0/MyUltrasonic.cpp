#include "MyUltrasonic.hpp"

MyUltrasonic::MyUltrasonic(int name)
{
    switch (name)
    {
        case 1:
            ultrasonic = new NewPing(L_ULTRASONIC_TRIG, L_ULTRASONIC_ECHO, MAX_DETECTION_DISTANCE);
        break;
        case 2:
            ultrasonic = new NewPing(LM_ULTRASONIC_TRIG, LM_ULTRASONIC_ECHO, MAX_DETECTION_DISTANCE);
        break;
        case 3:
            ultrasonic = new NewPing(M_ULTRASONIC_TRIG, M_ULTRASONIC_ECHO, MAX_DETECTION_DISTANCE);
        break;
        case 4:
            ultrasonic = new NewPing(RM_ULTRASONIC_TRIG, RM_ULTRASONIC_ECHO, MAX_DETECTION_DISTANCE);
        break;
        case 5:
            ultrasonic = new NewPing(R_ULTRASONIC_TRIG, R_ULTRASONIC_ECHO, MAX_DETECTION_DISTANCE);
        break;
    }
    distance = 0;
}

void MyUltrasonic::getDistance()
{
    if (ultrasonic->check_timer())
    {
        distance = ultrasonic->ping_result / US_ROUNDTRIP_CM;
    }
    else
        distance = 0;
}

float MyUltrasonic::isDistanceAvailable()
{
    unsigned long time = millis();
    while(millis()-time < PING_INTERVAL)
      ;
    ultrasonic->ping_timer(reinterpret_cast<void(*)()>(&MyUltrasonic::getDistance));
    ultrasonic->timer_stop();
    return distance;
}


