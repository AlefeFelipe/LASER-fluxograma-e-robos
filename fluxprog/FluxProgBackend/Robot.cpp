#include "Robot.hpp"

Robot::Robot()
{
    updateSensorsReading();
    command = 0;
}

void Robot::setCommand(int _command)
{
    command = _command;
}

int Robot::isFinished()
{
    return command;
}

int *Robot::getUltrasonicReading()
{
    return ultrasonic_sensor_reading;
}

int *Robot::getBlackTypeReading()
{
    return black_type_sensor_reading;
}

//int **Robot::getColorReading()
//{
//    return color_sensor_reading;
//}

void Robot::updateSensorsReading()
{
    int i;
    for (i = 0; i < N_ULTRASONIC; i++)
    {
        ultrasonic_sensor_reading[i] = 0;
    }
    for (i = 0; i < N_BLACK_TAPE_SENSOR; i++)
    {
        black_type_sensor_reading[i] = 0;
    }
    /*for (i = 0; i < N_COLOR_SENSOR; i++)
    {
        for (j = 0; j < COLOR_SENSOR_COMPONENTS; j++)
        {
            color_sensor_reading[i][j] = 0
        }
    }*/
}
