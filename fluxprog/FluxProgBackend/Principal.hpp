//biblioteca feita para agregacao dos componentes do fluxprogbackend

#ifndef _PRINCIPAL_HPP_
#define _PRINCIPAL_HPP_
#include "Communication.hpp"
#include "VirtualRobot.hpp"
#include "PhysicalRobot.hpp"

class Principal{
    int feedback;
    Communication *communication;
    VirtualRobot *virtual_robot;
    PhysicalRobot *physicalRobot;

public:
    Principal();
    ~Principal();
};

#endif
