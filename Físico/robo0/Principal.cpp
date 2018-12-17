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
        //robot->moveFoward();
        /*while(1)
        {
            //robot->getReadingUltrasonic();
            Serial.println("andando pra frente");
            robot.moveFoward();
            time = millis();
            Serial.println("lendo ultrassom");
            while(millis()-time<=2000)
            {
                robot.stop();
                //
                robot.getReadingUltrasonic();
            }
            Serial.println("andando pra esquerda");
            //robot->turnLeft();
            time = millis();
            
            while(millis()-time<=2000)
            {
                robot.stop();;
            }
            Serial.println("andando pra direita");
            //robot->turnRight();
            time = millis();
            
            while(millis()-time<=2000)
            {
                robot.stop();
            }
        //  float a[] = {1.43, 1.43, 1.34, 1.34, 1.34};
        //  uint8_t b[] = {1,1,1,1,1};
        //  bluetooth->sendPacket(b, a);
        }*/
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
        bluetooth.sendPacket(readingBTS, detectedObjet_U);
        //robot.getReadingUltrasonic();
        //time = millis();
        //Serial.println("vou parar");
        //while(millis()-time<=2000)
        //{
        //    robot->stop();
       //}
}
