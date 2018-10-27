//biblioteca feita para movimentacao do robo

#ifndef _ROBOT_HPP_
#define _ROBOT_HPP_
#include "Motor.hpp"
#include "BlackTapeSensor.hpp"
#include "MyUltrasonic.hpp"
#include "robotCte.hpp"

class Robot{
    Motor* motor[2];
    int lm_speed, rm_speed;
public:
    BlackTapeSensor* black_tape_sensor[N_BLACK_TAPE_SENSOR];
    //MyUltrasonic* ultrasonic[N_ULTRASONIC];
    Robot();
    void moveFoward();
    void turnLeft();
    void turnRight();
    void stop();
    void moveStraight(long* timeold, int stopped_motor);
    void reduceSpeed(long *timeold, int motor_num, float reason);
    uint8_t* getReadingBlackTypeSensor();
    float* getReadingUltrasonic();
    ~Robot(){}
};

#endif
