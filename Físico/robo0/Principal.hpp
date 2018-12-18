//biblioteca para gerar loop principal.
#ifndef _PRINCIPAL_HPP_
#define _PRINCIPAL_HPP_
#include "Robot.hpp"
#include "Bluetooth.hpp"

class Principal{
    Robot robot;
    Bluetooth bluetooth;
    uint8_t command;
    uint8_t readingBTS [N_BLACK_TAPE_SENSOR];
    uint16_t reading_color_sensor [N_COLOR_SENSOR][4];
    unsigned int detectedObjet_U [N_ULTRASONIC];
public:
    Principal();
    void start();
    ~Principal(){}
};

#endif
