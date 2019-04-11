//biblioteca feita para controlar o robo virtual

#ifndef _VIRTUAL_ROBOT_HPP_
#define _VIRTUAL_ROBOT_HPP_
#include <cmath>
#include <iostream>
#include "Robot.hpp"
#define M_PI       3.14159265358979323846
extern "C" {
    #include "extApi.h"
    #include "extApiPlatform.h"
}

using namespace std;

class VirtualRobot : public Robot{
    int client_id;
    int robot;
    int motors[N_MOTORS];
    int motor_speed[N_MOTORS];
    float robot_linear_position[ULTRASONIC_SENSOR_COMPONENTS];
    float robot_angle[ANGLE_COMPONENTS];
    int ultrasonic_sensors[N_ULTRASONIC];
    unsigned char is_there_obstacle[N_ULTRASONIC];
    float detected_objet[N_ULTRASONIC][ULTRASONIC_SENSOR_COMPONENTS];
    int detected_object_handle[N_ULTRASONIC];
    float detected_surface[N_ULTRASONIC][ULTRASONIC_SENSOR_COMPONENTS];
    int vision_sensors[N_BLACK_TAPE_SENSOR];
    unsigned char is_there_color[N_BLACK_TAPE_SENSOR];
    float* vision_sensor_data[N_BLACK_TAPE_SENSOR];
    int *aux_vision_sensor[N_BLACK_TAPE_SENSOR];
    void sortCommand();
    void moveForward();
    void turnLeft();
    void turnRight();
    void stop();
    void updateVirtualData();

public:
    VirtualRobot(int *error);
    ~VirtualRobot();
    void updateSensorsReading();
    void setCommand(int _command);
};

#endif
