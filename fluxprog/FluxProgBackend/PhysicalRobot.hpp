//biblioteca feita para mandar comandos ao robo fisico

#ifndef _PHYSICAL_ROBOT_HPP_
#define _PHYSICAL_ROBOT_HPP_
#include "fluxProgBackendCte.h"
#include "rs232.h"

class PhysicalRobot{
    void sendCommand();
public:
    PhysicalRobot();
    ~PhysicalRobot();

};

#endif
