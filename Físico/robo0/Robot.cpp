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
    lm_speed = MOTORSPEED;
    rm_speed = MOTORSPEED;
}

void Robot::moveFoward()
{
    int i, turnright, turnleft;
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


//    while(1)//(millis()-time<=5000))
//    {
        
        motor[0]->move(true, lm_speed);
        motor[1]->move(true, rm_speed);
        moveStraight(&timeold);
        
     
        delay(UPDATE_DELAY);
        for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
        {
            readingBTS[i] = black_tape_sensor[i]->getReading();
        }
        

    //}
        while(((readingBTS[0]&&readingBTS[2])||(readingBTS[2]&&readingBTS[4])))//verificar se ta no mesmo estado, tecnicamente inutil devido a funcao acima
        {
            motor[0]->move(true, lm_speed);
            motor[1]->move(true, rm_speed);
            moveStraight(&timeold);
            delay(UPDATE_DELAY);
            for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
            {
                readingBTS[i] = black_tape_sensor[i]->getReading();
            }
        }
        while(!((readingBTS[0]&&readingBTS[2])||(readingBTS[2]&&readingBTS[4])))//anda ate os sensores captarem a linha ou obstaculo
        {
            motor[0]->move(true, lm_speed);
            motor[1]->move(true, rm_speed);
            moveStraight(&timeold);
            for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
            {
                readingBTS[i] = black_tape_sensor[i]->getReading();
            }
            
            if(readingBTS[4]&&!readingBTS[2])
            {
                motor[0]->move(true, lm_speed);
                //motor[1]->move(false, rm_speed);
                //motor[1]->move(true, rm_speed/4);
                motor[1]->stop();
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
                motor[0]->move(true, lm_speed);
                //motor[1]->move(false, rm_speed);
                //motor[1]->move(true, rm_speed/2);
                motor[1]->stop();
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
                //motor[0]->move(false, lm_speed);
                //motor[0]->move(true, lm_speed/2);
                motor[0]->stop();
                motor[1]->move(true, rm_speed);
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
                //motor[0]->move(false, lm_speed);
                //motor[0]->move(true, lm_speed/4);
                motor[0]->stop();
                motor[1]->move(true, rm_speed);
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
                    motor[0]->move(true, lm_speed);
                    //motor[1]->move(true, rm_speed/8);
                    motor[1]->stop();
                    delay(UPDATE_DELAY);
                    for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
                    {
                        readingBTS[i] = black_tape_sensor[i]->getReading();
                    }
                }
                else if(turnleft)
                {
                    //motor[0]->move(true, lm_speed/8);
                    motor[0]->stop();
                    motor[1]->move(true, rm_speed);
                    delay(UPDATE_DELAY);
                    for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
                    {
                        readingBTS[i] = black_tape_sensor[i]->getReading();
                    }
                }
            }

            delay(UPDATE_DELAY);
        }
        time = millis();
        while(millis()-time <= TIMETURNING)
        {
            motor[0]->move(true, lm_speed);
            motor[1]->move(true, rm_speed);
            moveStraight(&timeold);
        }
}

void Robot::turnLeft()
{
    long time = millis();
    int i;
    int* readingBTS = new int [N_BLACK_TAPE_SENSOR];
    while(millis()-time<=TIMETURNING*2)
    {
        motor[0]->move(false, MOTORSPEED/1.2);
        motor[1]->move(true, (MOTORSPEED*SCALE)/1.2);
    }
    for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
    {
        readingBTS[i] = black_tape_sensor[i]->getReading();
    }
    while(readingBTS[3])
    {
        motor[0]->move(false, MOTORSPEED/1.2);
        motor[1]->move(true, (MOTORSPEED*SCALE)/1.2);
        readingBTS[3] = black_tape_sensor[3]->getReading();
    }
    while(!readingBTS[3])
    {
        motor[0]->move(false, MOTORSPEED/1.2);
        motor[1]->move(true, (MOTORSPEED*SCALE)/1.2);
        readingBTS[3] = black_tape_sensor[3]->getReading();
    }
}

void Robot::turnRight()
{
    long time = millis();
    
    int i;
    int* readingBTS = new int [N_BLACK_TAPE_SENSOR];
    while(millis()-time<=TIMETURNING*2)
    {
        motor[0]->move(true, MOTORSPEED/1.2);
        motor[1]->move(false, (MOTORSPEED*SCALE)/1.2);
    }
    for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
    {
        readingBTS[i] = black_tape_sensor[i]->getReading();
    }
    while(readingBTS[1])
    {
        motor[0]->move(true, MOTORSPEED/1.2);
        motor[1]->move(false, (MOTORSPEED*SCALE)/1.2);
        readingBTS[1] = black_tape_sensor[1]->getReading();
    }
    while(!readingBTS[1])
    {
        motor[0]->move(true, MOTORSPEED/1.2);
        motor[1]->move(false, (MOTORSPEED*SCALE)/1.2);
        readingBTS[1] = black_tape_sensor[1]->getReading();
    }
}

void Robot::stop()
{
    motor[0]->stop();
    motor[1]->stop();
}

void Robot::moveStraight(long *timeold)
{
    int* readingRPM = new int [2];
    //long timeold = millis();
    readingRPM[0] = motor[0]->getRPM(*timeold);
    readingRPM[1] = motor[1]->getRPM(*timeold);
    if(millis()-*timeold>=ENCODER_UPDATE_TIME)
    {
        if(readingRPM[0]>SPEED_TO_UPDATE_TIME)
        {
            lm_speed -= D_SPEED;
            if(lm_speed < 0)
            {
                lm_speed = 0;
            }
        }
        else
        {
            lm_speed += D_SPEED;
        }
        if(readingRPM[1]>SPEED_TO_UPDATE_TIME)
        {
            rm_speed -= D_SPEED;
            if(rm_speed < 0)
            {
                rm_speed = 0;
            }
        }
        else
        {
            rm_speed += D_SPEED;
        }
        readingRPM[0] = motor[0]->getRPM(*timeold);
        readingRPM[1] = motor[1]->getRPM(*timeold);
        *timeold = millis();
    }
}


