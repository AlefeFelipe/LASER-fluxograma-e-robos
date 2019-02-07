//biblioteca feita para captar leitura e mandar comandos para o robo

#ifndef _ROBOT_HPP_
#define _ROBOT_HPP_
#include "fluxProgBackendCte.h"

class Robot{
protected:
    float ultrasonic_sensor_reading [N_ULTRASONIC];
    int black_type_sensor_reading [N_BLACK_TAPE_SENSOR];
    int color_sensor_reading [N_COLOR_SENSOR][COLOR_SENSOR_COMPONENTS];
    int command;

public:
    Robot();
    ~Robot();
    void setCommand(int _command);
    int isFinished();
    float *getUltrasonicReading();
    int *getUltrasonicReading();
    int **getColorReading();
    void updateSensorsReading();
};

#endif
