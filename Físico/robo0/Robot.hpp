//biblioteca feita para movimentacao do robo

#ifndef _ROBOT_HPP_
#define _ROBOT_HPP_
#include "Motor.hpp"
#include "BlackTapeSensor.hpp"
//#include "MyUltrasonic.hpp"
#include "robotCte.h"
#include <Ultrasonic.h>
//#include "ColorSensor.hpp"
#include <Adafruit_TCS34725softi2c.h>
class Robot{
    Motor* motor[2];
    Ultrasonic *ultrasonic[N_ULTRASONIC];
    int lm_speed, rm_speed;
    BlackTapeSensor *black_tape_sensor[N_BLACK_TAPE_SENSOR];
    Adafruit_TCS34725softi2c *color_sensor[N_COLOR_SENSOR];
    uint16_t colors[N_COLOR_SENSOR][COLOR_SENSOR_COMPONENTS];
public:
    Robot();
    void moveFoward();
    void turnLeft();
    void turnRight();
    void stop();
    void moveStraight(long *timeold, int stopped_motor);
    void reduceSpeed(long *timeold, int motor_num, float reason);
    void getReadingBlackTypeSensor(uint8_t *readingBTS);
    void getReadingUltrasonic(unsigned int *detectedObjet_U);
    void getReadingColorSensor(uint16_t reading[N_COLOR_SENSOR][4]);
    ~Robot(){}
};

#endif
