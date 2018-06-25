#include "Principal.hpp"

Principal::Principal()
{
    bluetooth = new Bluetooth();
    robot = new Robot();
}

void Principal::start()
{
    long time = millis();
    robot->moveFoward();
    if(bluetooth->getCommand() == 8)
    {
        robot->moveFoward();
    }
    else if(bluetooth->getCommand() == 4)
    {
        robot->turnLeft();
    }
    else if(bluetooth->getCommand() == 6)
    {
        robot->turnRight();
    }
    time = millis();
    while(millis()-time<=2000)
    {
        robot->stop();
    }
    
}
