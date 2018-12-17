//biblioteca para controle dos motores

#ifndef _MOTOR_HPP_
#define _MOTOR_HPP_
#include "robotCte.hpp"
#include <Arduino.h>
#include <AFMotor.h>

void pulse_counter();

class Motor : AF_DCMotor{
    int pin_encoder;
    unsigned int resolution;
    
public:
    
    Motor(int _name);
    int getRPM(long timeold);
    ~Motor(){}
    void move(bool direction, int speed);
    void stop();

};
void pulse_counter(Motor* motor);

#endif
