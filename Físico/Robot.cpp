#include "Robot.hpp"
Robot::Robot()
{
    int i;
    motor = new Motor* [2];
    ultrasonic = new MyUltrasonic* [N_ULTRASONIC];
    black_tape_sensor = new BlackTapeSensor* [N_BLACK_TAPE_SENSOR];
    /*for(i = 0; i<2; i++)
    {
        motor[i] = new Motor(i+1);
    }*/
    motor[0] = new Motor(L_MOTOR);
    motor[1] = new Motor(R_MOTOR);
    for(i=0; i<N_ULTRASONIC; i++)
    {
        ultrasonic[i] = new MyUltrasonic(i+1);
    }
    for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
    {
        black_tape_sensor[i] = new BlackTapeSensor(i+1);
    }
}

void Robot::moveFoward()
{
    int i, turnright, turnleft, teste = 15;
    //int** oi = new int*;
    //oi[5] = &teste;
    //Serial.println(*oi[5]);
    long time = millis(), timeold = millis(), time_stop;
    int* readingBTS = new int [N_BLACK_TAPE_SENSOR];
    //while(1)
    //{
        for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
        {
            readingBTS[i] = (black_tape_sensor[i])->getReading();
            Serial.print(readingBTS[i]);
        }
        Serial.println();
    //}

    //while((millis()-time<=5000))
    //{
        motor[0]->move(true, MOTORSPEED);
        motor[1]->move(true, SCALE*MOTORSPEED);
        delay(UPDATE_DELAY);
        for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
        {
            readingBTS[i] = black_tape_sensor[i]->getReading();
        }
       /* if(millis()-timeold>=ENCODER_UPDATE_TIME)
        {
            if(readingRPM[0]>readingRPM[1])
            {
                motor[0]->stop();
                Serial.println("esquerdo correndo");
                //motor[0]->move(true, MOTORSPEED);
                //readingRPM[0] = motor[0]->getRPM(timeold);
                Serial.print("reading 0: ");
                Serial.println(readingRPM[0]);
                //Serial.print("reading 1: ");
                //Serial.println(readingRPM[1]);
                delay(100);
            }
            else if(readingRPM[1]>readingRPM[0])
            {
                motor[1]->stop();
                //motor[1]->move(true, SCALE*MOTORSPEED);
                //readingRPM[1] = motor[1]->getRPM(timeold);
                //Serial.print("reading 1: ");
                //Serial.println(readingRPM[1]);
                Serial.print("reading 0: ");
                Serial.println(readingRPM[0]);
                delay(100);

            }
            timeold = millis();
        }*/
        while(((readingBTS[0]&&readingBTS[2])||(readingBTS[2]&&readingBTS[4])))//verificar se ta no mesmo estado, tecnicamente inutil devido a funcao acima
        {
            motor[0]->move(true, MOTORSPEED);
            motor[1]->move(true, SCALE*MOTORSPEED);
            delay(UPDATE_DELAY);
            for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
            {
                readingBTS[i] = black_tape_sensor[i]->getReading();
            }
        }
        while(!((readingBTS[0]&&readingBTS[2])||(readingBTS[2]&&readingBTS[4])))//anda ate os sensores captarem a linha ou obstaculo
        {
            motor[0]->move(true, MOTORSPEED);
            motor[1]->move(true, SCALE*MOTORSPEED);
            delay(UPDATE_DELAY);
            for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
            {
                readingBTS[i] = black_tape_sensor[i]->getReading();
            }
            
            if(readingBTS[4]&&!readingBTS[2])
            {
                motor[0]->move(true, MOTORSPEED);
                motor[1]->move(true, (SCALE*MOTORSPEED)/4);
                delay(UPDATE_DELAY);
                for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
                {
                    readingBTS[i] = black_tape_sensor[i]->getReading();
                }
                turnright = 1;
                turnleft = 0;
            }
            else if(readingBTS[3]&&!readingBTS[2])
            {
                motor[0]->move(true, MOTORSPEED);
                motor[1]->move(true, (SCALE*MOTORSPEED)/2);
                delay(UPDATE_DELAY);
                for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
                {
                    readingBTS[i] = black_tape_sensor[i]->getReading();
                }
                turnright = 1;
                turnleft = 0;
            }
            else if(readingBTS[1]&&!readingBTS[2])//robo ta desviando para a direita, gira esquerda
            {
                motor[0]->move(true, MOTORSPEED/2);
                motor[1]->move(true, (SCALE*MOTORSPEED));
                delay(UPDATE_DELAY);
                for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
                {
                    readingBTS[i] = black_tape_sensor[i]->getReading();
                }
                turnright = 0;
                turnleft = 1;
            }
            else if(readingBTS[0]&&!readingBTS[2])//robo ta desviando para a direita, gira esquerda
            {
                motor[0]->move(true, MOTORSPEED/4);
                motor[1]->move(true, SCALE*MOTORSPEED);
                delay(UPDATE_DELAY);
                for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
                {
                    readingBTS[i] = black_tape_sensor[i]->getReading();
                }
                turnright = 0;
                turnleft = 1;
            }
            else if(!(readingBTS[0]||readingBTS[1]||readingBTS[2]||readingBTS[3]||readingBTS[4]))
            {
                if(turnright)
                {
                    motor[0]->move(true, MOTORSPEED);
                    motor[1]->move(true, SCALE*MOTORSPEED/4);
                    delay(UPDATE_DELAY);
                    for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
                    {
                        readingBTS[i] = black_tape_sensor[i]->getReading();
                    }
                }
                else if(turnleft)
                {
                    motor[0]->move(true, MOTORSPEED/4);
                    motor[1]->move(true, SCALE*MOTORSPEED);
                    delay(UPDATE_DELAY);
                    for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
                    {
                        readingBTS[i] = black_tape_sensor[i]->getReading();
                    }
                }
            }

            delay(UPDATE_DELAY);
        }
        //while(millis()-time_stop<=1000&& (millis()-time<=5000))
        //{
         //   stop();
        //}
        
        //DEPOIS FAZER A PARTE DE ANDAR PRA FRENTE O RESTANTE
   // }
}

void Robot::turnLeft()
{
    long time = 0;
    while(millis()-time<=TIMETURNING)
    {
        motor[0]->move(false, MOTORSPEED);
        motor[1]->move(true, SCALE*MOTORSPEED);
    }
}

void Robot::turnRight()
{
    long time = 0;
    while(millis()-time<=TIMETURNING)
    {
        motor[0]->move(true, MOTORSPEED);
        motor[1]->move(false, MOTORSPEED);
    }
}

void Robot::stop()
{
    motor[0]->stop();
    motor[1]->stop();
}
