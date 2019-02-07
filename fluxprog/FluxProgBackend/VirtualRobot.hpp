//biblioteca feita para controlar o robo virtual

#ifndef _VIRTUAL_ROBOT_HPP_
#define _VIRTUAL_ROBOT_HPP_
#include <cmath>
#include "remoteApi/extApi.h"
#include "remoteApi/extApiPlatform.h"
#include "fluxProgBackendCte.h"

class VirtualRobot : public Robot{
    int client_id;
    int robot;
    int motors[N_MOTORS];
    int motor_speed[N_MOTORS]
    float robot_angle[ANGLE_COMPONENTS];
    int ultrasonic_sensors[N_ULTRASONIC];
    unsigned char is_there_obstacle[N_ULTRASONIC];
    float detected_objet[N_ULTRASONIC][ULTRASONIC_SENSOR_COMPONENTS];
    int detected_object_handle[N_ULTRASONIC];
    float detected_surface[N_ULTRASONIC][ULTRASONIC_SENSOR_COMPONENTS];
    int vision_sensors[N_BLACK_TAPE_SENSOR];
    unsigned char is_there_color[N_BLACK_TAPE_SENSOR];
    float data_vision_sensor[N_BLACK_TAPE_SENSOR][VISION_SENSOR_COMPONENTS];
    int *aux_vision_sensor;

protected:
    void sortCommand();
    void moveForward();
    void turnLeft();
    void turnRight();

public:
    VirtualRobot();
    ~VirtualRobot();
};

#endif
