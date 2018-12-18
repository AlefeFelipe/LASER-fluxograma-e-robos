#include "Principal.hpp"

Principal::Principal()
{
    bluetooth = Bluetooth();
    robot = Robot();
    command = 0;
}

void Principal::start()
{
        long time = millis();
        command = bluetooth.getCommand();
        if(command == 8)
        {
            //Serial.println("andando pra frente");
            robot.moveFoward();
        }
        else if(command == 4)
        {
            //Serial.println("andando pra esquerda");
            robot.turnLeft();
            
        }
        else if(command == 6)
        {
            //Serial.println("andando pra direita");
            robot.turnRight();
        }
        else
        {
            //Serial.println("robo parado");
            robot.stop();
            
        }    
        robot.getReadingBlackTypeSensor(readingBTS);
        robot.getReadingUltrasonic(detectedObjet_U);
        robot.getReadingColorSensor(reading_color_sensor);
        bluetooth.sendPacket(readingBTS, detectedObjet_U);
        //robot.getReadingUltrasonic();
        //time = millis();
        //Serial.println("vou parar");
        //while(millis()-time<=2000)
        //{
        //    robot->stop();
       //}
}
