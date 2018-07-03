#include "Principal.hpp"

Principal::Principal()
{
    bluetooth = new Bluetooth();
    robot = new Robot();
    command = 0;
}

void Principal::start()
{
    long time = millis();
    //robot->moveFoward();
    command = bluetooth->getCommand();
    if(command == 8)
    {
        //Serial.println("andando pra frente");
        robot->moveFoward();
    }
    else if(command == 4)
    {
        Serial.println("andando pra esquerda");
        //robot->turnLeft();
        
    }
    else if(command == 6)
    {
         Serial.println("andando pra direita");
        //robot->turnRight();
    }
    else
    {
        //Serial.println("robo parado");
        robot->stop();
    }    
    //time = millis();
    //while(millis()-time<=2000)
    //{
    //    robot->stop();
    //}
    
}
