#include "BlackTapeSensor.hpp"

BlackTapeSensor::BlackTapeSensor(int name)
{
    switch(name)
    {
        case 1:
            pin = L_BLACK_TAPE_SENSOR;
        break;
        case 2:
            pin = LM_BLACK_TAPE_SENSOR;
        break;     
        case 3:
            pin = M_BLACK_TAPE_SENSOR;
        break;
        case 4:
            pin = RM_BLACK_TAPE_SENSOR;
        break;
        case 5:
            pin = R_BLACK_TAPE_SENSOR;
        break;
    }
    reading = 0;
}

int BlackTapeSensor::getReading()
{
    reading = !digitalRead(pin);
    //Serial.println(pin);
    return reading;
}
