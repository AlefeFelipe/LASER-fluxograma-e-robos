//biblioteca para gerar loop principal.
#ifndef _PRINCIPAL_HPP_
#define _PRINCIPAL_HPP_
#include "Robot.hpp"
#include "Bluetooth.hpp"

class Principal{
    Robot* robot;
    Bluetooth* bluetooth;
public:
    Principal();
    void start();
    ~Principal(){}
};

#endif
