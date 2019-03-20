//biblioteca feita para mandar comandos ao robo fisico

#ifndef _PHYSICAL_ROBOT_HPP_
#define _PHYSICAL_ROBOT_HPP_
#include "fluxProgBackendCte.h"
#include "rs232.h"
#include "Robot.hpp"
#include <iostream>
#include <ctime>

class PhysicalRobot : public Robot{
    void sendCommand();
    void receiveData();
public:
    PhysicalRobot(int *error);
    ~PhysicalRobot();
    void setCommand(int _command);
    void updateSensorsReading();

};

#endif
