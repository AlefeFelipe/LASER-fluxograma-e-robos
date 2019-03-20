//biblioteca feita para agregacao dos componentes do fluxprogbackend

#ifndef _FLUXPROGBACKEND_HPP_
#define _FLUXPROGBACKEND_HPP_
#include "Communication.hpp"
#include "VirtualRobot.hpp"
#include "PhysicalRobot.hpp"
#include "fluxProgBackendCte.h"

class FluxProgBackend{
    int feedback;
    Communication *communication;
    VirtualRobot *virtual_robot;
    PhysicalRobot *physical_robot;

public:
    FluxProgBackend();
    ~FluxProgBackend();
    void start();
    void connect();
};

#endif
