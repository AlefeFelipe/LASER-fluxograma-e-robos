#include "Principal.hpp"

Principal::Principal()
{
    bluetooth = new Bluetooth();
    robot = new Robot();
    command = 0;
}

void Principal::start()
{
    while(1)
    {
        long time = millis();
        command = bluetooth->getCommand();
        if(command == MOVE_FORWARD)
        {
            //Serial.println("andando pra frente");
            robot->moveFoward();
        }
        else if(command == TURN_LEFT)
        {
            //Serial.println("andando pra esquerda");
            robot->turnLeft();

        }
        else if(command == TURN_RIGHT)
        {
            //Serial.println("andando pra direita");
            robot->turnRight();
        }
        else
        {
            //Serial.println("robo parado");
            robot->stop();
        }
        //delay(100);
        robot->getReadingBlackTypeSensor(readingBTS);
        robot->getReadingUltrasonic(detectedObjet_U);
        //robot->getReadingColorSensor(reading_color_sensor);
        bluetooth->sendPacket(readingBTS, detectedObjet_U);
        //robot.getReadingUltrasonic();
//        if(millis()-time >=1000)
  //      {
            //Serial.println("oi");
    //        time = millis();
      //  }
        //while(millis()-time<=2000)
        //{
        //    robot->stop();
       //}
    }
}
