//biblioteca feita para movimentacao do robo

#ifndef _ROBOT_HPP_
#define _ROBOT_HPP_
#include "Motor.hpp"
#include "BlackTapeSensor.hpp"
//#include "MyUltrasonic.hpp"
#include "robotCte.hpp"
#include <Ultrasonic.h>
//#include "ColorSensor.hpp"
#include <Adafruit_TCS34725softi2c.h>
class Robot{
    Motor* motor[2];
    Ultrasonic ultrasonic[N_ULTRASONIC] = {
        Ultrasonic(L_ULTRASONIC_TRIG, L_ULTRASONIC_ECHO, ULTRASONIC_TIMEOUT),
        Ultrasonic(LM_ULTRASONIC_TRIG, LM_ULTRASONIC_ECHO, ULTRASONIC_TIMEOUT),
        Ultrasonic(M_ULTRASONIC_TRIG, M_ULTRASONIC_ECHO, ULTRASONIC_TIMEOUT),
        Ultrasonic(RM_ULTRASONIC_TRIG, RM_ULTRASONIC_ECHO, ULTRASONIC_TIMEOUT),
        Ultrasonic(R_ULTRASONIC_TRIG, R_ULTRASONIC_ECHO, ULTRASONIC_TIMEOUT)
    };
    int lm_speed, rm_speed;
    BlackTapeSensor* black_tape_sensor[N_BLACK_TAPE_SENSOR];
    Adafruit_TCS34725softi2c color_sensor[N_COLOR_SENSOR] = {
        Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X, L_COLOR_SENSOR_SDA, L_COLOR_SENSOR_SCL),
        Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X, R_COLOR_SENSOR_SDA, R_COLOR_SENSOR_SCL)
    };
    uint16_t colors[N_COLOR_SENSOR][4];
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
