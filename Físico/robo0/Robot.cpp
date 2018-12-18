#include <math.h>
#include "Robot.hpp"
int myABS(int x)
{
    return (x>0? x : -x);
}

Robot::Robot()
{
    int i;
    motor[0] = new Motor(L_MOTOR);
    motor[1] = new Motor(R_MOTOR);

    for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
    {
        black_tape_sensor[i] = new BlackTapeSensor(i+1);
    }
    lm_speed = MOTORSPEED;
    rm_speed = MOTORSPEED*SCALE;
    
}

void Robot::moveFoward()
{
    int i, turnright, turnleft;
    long time = millis(), timeold = millis(), time_stop;
    int readingBTS [N_BLACK_TAPE_SENSOR];
    unsigned int readingU[N_ULTRASONIC];
    //primeira leitura dos sensores
    for(i=0; i<N_ULTRASONIC; i++)
    {
        readingU[i] = (ultrasonic[i].read() > OBSTACLE_DIS ? 0 : 1);  
    }
    for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
    {
        readingBTS[i] = black_tape_sensor[i]->getReading();
    }
    
    while(((readingBTS[0]&&readingBTS[2])||(readingBTS[2]&&readingBTS[4])) && !readingU[2])//verificar se ta no mesmo estado, tecnicamente inutil devido a funcao acima
    {
        motor[0]->move(true, lm_speed);
        motor[1]->move(true, rm_speed);
        moveStraight(&timeold, -1);
        for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
        {
            readingBTS[i] = black_tape_sensor[i]->getReading();
        }
        for(i=0; i<N_ULTRASONIC; i++)
        {
            readingU[i] = (ultrasonic[i].read() > OBSTACLE_DIS ? 0 : 1);
        }        
    }
    
    while(!((readingBTS[0]&&readingBTS[2])||(readingBTS[2]&&readingBTS[4])) && !readingU[2])//anda ate os sensores captarem a linha ou obstaculo
    {
        for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
        {
            readingBTS[i] = black_tape_sensor[i]->getReading();
        }
        for(i=0; i<N_ULTRASONIC; i++)
        {
            readingU[i] = (ultrasonic[i].read() > OBSTACLE_DIS ? 0 : 1);
        }
        
        if(readingBTS[4]&&!(readingBTS[2]||readingBTS[1]||readingBTS[0]))
        {
            motor[0]->move(true, lm_speed);
            motor[1]->move(false, rm_speed);
            moveStraight(&timeold, -1);
            delay(UPDATE_DELAY);
            for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
            {
                readingBTS[i] = black_tape_sensor[i]->getReading();
            }
            turnright = 1;
            turnleft = 0;
        }
        else if(readingBTS[3]&&!(readingBTS[2]||readingBTS[1]||readingBTS[0]))
        {
            motor[0]->move(true, lm_speed);
            motor[1]->stop();
            moveStraight(&timeold, 1);
            delay(UPDATE_DELAY);
            for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
            {
                readingBTS[i] = black_tape_sensor[i]->getReading();
            }
            turnright = 1;
            turnleft = 0;
        }
        else if(readingBTS[1]&&!(readingBTS[2]||readingBTS[3]||readingBTS[4]))//robo ta desviando para a direita, gira esquerda
        {
            motor[0]->stop();
            motor[1]->move(true, rm_speed);
            moveStraight(&timeold, 0);
            delay(UPDATE_DELAY);
            for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
            {
                readingBTS[i] = black_tape_sensor[i]->getReading();
            }
            turnright = 0;
            turnleft = 1;
        }
        else if(readingBTS[0]&&!(readingBTS[2]||readingBTS[3]||readingBTS[4]))//robo ta desviando para a direita, gira esquerda
        {
            motor[0]->move(false, lm_speed);
            motor[1]->move(true, rm_speed);
            moveStraight(&timeold, -1);
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
                motor[1]->stop();
                moveStraight(&timeold, 1);
                delay(UPDATE_DELAY);
                for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
                {
                    readingBTS[i] = black_tape_sensor[i]->getReading();
                }
            }
            else if(turnleft)
            {
                motor[0]->stop();
                motor[1]->move(true, rm_speed);
                moveStraight(&timeold, 0);
                delay(UPDATE_DELAY);
                for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
                {
                    readingBTS[i] = black_tape_sensor[i]->getReading();
                }
            }
        }
        else
        {
            motor[0]->move(true, lm_speed);
            motor[1]->move(true, rm_speed);
            moveStraight(&timeold, -1);
        }      
    }
    time = millis();
    while(millis()-time <= TIMETURNING && !readingU[2])
    {
        motor[0]->move(true, lm_speed);
        motor[1]->move(true, rm_speed);
        moveStraight(&timeold, -1);
        for(i=0; i<N_ULTRASONIC; i++)
        {
            readingU[i] = (ultrasonic[i].read() > OBSTACLE_DIS ? 0 : 1);    
        }
    }
}

void Robot::turnLeft()
{
    long time = millis(), timeold = millis();
    int i;
    int readingBTS[N_BLACK_TAPE_SENSOR];
    while(millis()-time<=TIMETURNING*2)
    {
        motor[0]->move(false, lm_speed);
        motor[1]->move(true, rm_speed);
        moveStraight(&timeold, -1);
    }
    for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
    {
        readingBTS[i] = black_tape_sensor[i]->getReading();
    }
    while(readingBTS[3])
    {
        motor[0]->move(false, lm_speed);
        motor[1]->move(true, rm_speed);
        moveStraight(&timeold, -1);
        readingBTS[3] = black_tape_sensor[3]->getReading();
    }
    readingBTS[2] = black_tape_sensor[2]->getReading();
    while(!readingBTS[2])
    {
        motor[0]->move(false, lm_speed);
        motor[1]->move(true, rm_speed);
        moveStraight(&timeold, -1);
        readingBTS[2] = black_tape_sensor[2]->getReading();
    }
}

void Robot::turnRight()
{
    long time = millis(), timeold = millis();
    
    int i;
    int readingBTS[N_BLACK_TAPE_SENSOR];
    while(millis()-time<=TIMETURNING*2)
    {
        motor[0]->move(true, lm_speed);
        motor[1]->move(false, rm_speed);
        moveStraight(&timeold, -1);
    }
    for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
    {
        readingBTS[i] = black_tape_sensor[i]->getReading();
    }
    while(readingBTS[3])
    {
        motor[0]->move(true, lm_speed);
        motor[1]->move(false, rm_speed);
        moveStraight(&timeold, -1);
        readingBTS[3] = black_tape_sensor[3]->getReading();
    }
    readingBTS[4] = black_tape_sensor[4]->getReading();
    while(!readingBTS[4])
    {
        motor[0]->move(true, lm_speed);
        motor[1]->move(false, rm_speed);
        moveStraight(&timeold, -1);
        readingBTS[4] = black_tape_sensor[4]->getReading();
    }
}

void Robot::stop()
{
    motor[0]->stop();
    motor[1]->stop();
}

void Robot::moveStraight(long *timeold, int stopped_motor)
{
    int error = 0;
    int readingRPM[2];
    readingRPM[0] = motor[0]->getRPM(*timeold);
    readingRPM[1] = motor[1]->getRPM(*timeold);
    if(millis()-*timeold>=ENCODER_UPDATE_TIME)
    {
        if(stopped_motor != 0)
        {
            if(readingRPM[0]>SPEED_TO_UPDATE_TIME)
            {
                lm_speed -= D_SPEED;
    
                error = SPEED_TO_UPDATE_TIME-readingRPM[0];
                if(lm_speed < 0)
                {
                    lm_speed = 0;
                }
            }
            else
            {
                error = SPEED_TO_UPDATE_TIME-readingRPM[0];
                lm_speed += D_SPEED;
                if(lm_speed > 255)
                {
                    lm_speed = 255;
                }
            }
        }
        if(stopped_motor != 1)
        {
            if(readingRPM[1]>SPEED_TO_UPDATE_TIME)
            {
                error = int(SPEED_TO_UPDATE_TIME)-readingRPM[1];
                rm_speed -= D_SPEED;
                if(rm_speed < 0)
                {
                    rm_speed = 0;
                }
            }
            else
            {
                error = int(SPEED_TO_UPDATE_TIME)-readingRPM[1];
                rm_speed += D_SPEED;           
                if(rm_speed > 255)
                {
                    rm_speed = 255;
                }
            }
        }
        readingRPM[0] = motor[0]->getRPM(*timeold);
        readingRPM[1] = motor[1]->getRPM(*timeold);
        *timeold = millis();
    }
}

void Robot::reduceSpeed(long *timeold, int motor_num, float reason)
{
    int readingRPM[2];
    readingRPM[0] = motor[0]->getRPM(*timeold);
    readingRPM[1] = motor[1]->getRPM(*timeold);
    motor[motor_num]->stop();
    if(millis()-*timeold>=ENCODER_UPDATE_TIME)
    {
        if (motor_num == 1)
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
                if(lm_speed > 255)
                {
                    lm_speed = 255;
                }
            }
        }
        else
        {
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
                if(rm_speed > 255)
                {
                    rm_speed = 255;
                }
            }
        }
        readingRPM[0] = motor[0]->getRPM(*timeold);
        readingRPM[1] = motor[1]->getRPM(*timeold);
        *timeold = millis();
    }
}

void Robot::getReadingBlackTypeSensor(uint8_t *readingBTS)
{
    int i;
    for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
    {
        readingBTS[i] = black_tape_sensor[i]->getReading();
    }
}   

void Robot::getReadingUltrasonic(unsigned int *detectedObjet_U)
{
    int i;
    for(i=0; i<N_ULTRASONIC; i++)
    {
        detectedObjet_U[i] = ultrasonic[i].read();
    }
}

void Robot::getReadingColorSensor(uint16_t reading[N_COLOR_SENSOR][4])
{
    int i, j;
    for(i=0; i<N_COLOR_SENSOR; i++)
    {
        color_sensor[i].getRawData(&colors[i][0], &colors[i][1], &colors[i][2], &colors[i][3]);    
    }
    reading = colors;
    /*for(i=0; i<N_COLOR_SENSOR; i++)
    {
        Serial.print("sensor ");
        Serial.print(i);
        for(j=0; j<4; j++)
        {
            Serial.print(" posicao");
            Serial.print(j);
            Serial.print(" ");
            Serial.print(reading[i][j]);
        }
        Serial.println();
    }*/    
}
