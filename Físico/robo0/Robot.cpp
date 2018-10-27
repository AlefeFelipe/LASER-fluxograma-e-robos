#include <math.h>
#include "Robot.hpp"
int myABS(int x)
{
    return (x>0? x : -x);
}

Robot::Robot()
{
    int i;
    //motor = new Motor* [2];
    //ultrasonic = new MyUltrasonic* [N_ULTRASONIC];
    //black_tape_sensor = new BlackTapeSensor* [N_BLACK_TAPE_SENSOR];
    /*for(i = 0; i<2; i++)
    {
        motor[i] = new Motor(i+1);
    }*/
    motor[0] = new Motor(L_MOTOR);
    motor[1] = new Motor(R_MOTOR);
    //for(i=0; i<N_ULTRASONIC; i++)
    //{
     //   ultrasonic[i] = new MyUltrasonic(i+1);
     //   Serial.println("criando");
    //}
    for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
    {
        black_tape_sensor[i] = new BlackTapeSensor(i+1);
    }
    lm_speed = MOTORSPEED;
    rm_speed = MOTORSPEED*SCALE;
    initUltrasonics();
    
}

void Robot::moveFoward()
{
    int i, turnright, turnleft;
    long time = millis(), timeold = millis(), time_stop;
    int readingBTS [N_BLACK_TAPE_SENSOR];
    float* readingU;;// = new float [N_ULTRASONIC];
    //readingU[2] = 0;
    //while(millis()-time<5000)
    //{
    //    motor[0]->move(true, lm_speed);
    //    motor[1]->move(true, rm_speed);
    //    moveStraight(&timeold);
    //}
    
    //while(1)
    //{
        //for(i=0; i<N_ULTRASONIC; i++)
        //{
            //Serial.print("ultrasonic ");
            //Serial.print(i);
            //Serial.print(": ");
          //  readingU[i] = (ultrasonic[i]->isDistanceAvailable() > OBSTACLE_DIS ? 0 : 1);
            //Serial.println(ultrasonic[i]->isDistanceAvailable());
            
        //}
        //Serial.println();
    //}


//    while(1)//(millis()-time<=5000))
//    {
        
        //motor[0]->move(true, lm_speed);
        //motor[1]->move(true, rm_speed);
        //moveStraight(&timeold);
        
     
        //while(1)
        {
            for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
            {
                readingBTS[i] = black_tape_sensor[i]->getReading();
                //Serial.print(readingBTS[i]);
            //    Serial.print(" ");
            }
            //Serial.println();
        }
        readingU = isDistanceAvailable();
        //Serial.println(1);
        /*for(i=0; i<N_ULTRASONIC; i++)
        {
            if(readingU[i]>0)
            {
                readingU[i] = (readingU[i] > OBSTACLE_DIS ? 0 : 1);
                Serial.print(readingU[i]);
                Serial.print(" ");    
            }
            
        }*/
        //Serial.println();
        
        //readingU[2] = (ultrasonic[2]->isDistanceAvailable() > OBSTACLE_DIS ? 0 : 1);
        //Serial.println("hello");
        //readingU[2] = 0;

    //}
        while(((readingBTS[0]&&readingBTS[2])||(readingBTS[2]&&readingBTS[4])) && !readingU[2])//verificar se ta no mesmo estado, tecnicamente inutil devido a funcao acima
        {
            //Serial.print(" v0: ");
            //Serial.print(lm_speed);
            //Serial.print(" v1: ");
            //Serial.println(rm_speed);
            motor[0]->move(true, lm_speed);
            motor[1]->move(true, rm_speed);
            moveStraight(&timeold, -1);
            //Serial.println("loop");

            for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
            {
                readingBTS[i] = black_tape_sensor[i]->getReading();
            }
            readingU = isDistanceAvailable();
            /*for(i=0; i<N_ULTRASONIC; i++)
            {
              
                if(readingU[i]>0)
                {
                    readingU[i] = (readingU[i] > OBSTACLE_DIS ? 0 : 1); 
                    Serial.print(readingU[i]);
                    Serial.print(" ");    
                }
            }*/
            //Serial.println();
            //Serial.println(2);
            //for(i=0; i<N_ULTRASONIC; i++)
            //{
            //    //readingU[i] = (ultrasonic[i]->isDistanceAvailable() > OBSTACLE_DIS ? 0 : 1);
                //Serial.print(//readingU[i]);    
            //}
            //readingU[2] = (ultrasonic[2]->isDistanceAvailable() > OBSTACLE_DIS ? 0 : 1);
           // Serial.println("to lendo todo mundo!!");
            
        }
        while(!((readingBTS[0]&&readingBTS[2])||(readingBTS[2]&&readingBTS[4])) && !readingU[2])//anda ate os sensores captarem a linha ou obstaculo
        {
            for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
            {
                readingBTS[i] = black_tape_sensor[i]->getReading();
            }
            //for(i=0; i<N_ULTRASONIC; i++)
            //{
                //readingU[i] = (ultrasonic[i]->isDistanceAvailable() > OBSTACLE_DIS ? 0 : 1);    
            //}
            //readingU[2] = (ultrasonic[2]->isDistanceAvailable() > OBSTACLE_DIS ? 0 : 1);
            //Serial.println("to achando o caminho!!");
            readingU = isDistanceAvailable();
            //Serial.println(3);
            /*for(i=0; i<N_ULTRASONIC; i++)
            {
                 if(readingU[i]>0)
                {
                    readingU[i] = (readingU[i] > OBSTACLE_DIS ? 0 : 1);    
                    Serial.print(readingU[i]);
                    Serial.print(" ");    
                }
            }
            //Serial.println();
            readingU[2] = 0;*/
            
            if(readingBTS[4]&&!(readingBTS[2]||readingBTS[1]||readingBTS[0]))
            {
                //Serial.println("vou parar o motor 1");
                motor[0]->move(true, lm_speed);
                motor[1]->move(false, rm_speed);
                //motor[1]->move(true, rm_speed/2);
                //motor[1]->stop();
                //reduceSpeed(&timeold, 1, 2);
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
                //motor[1]->move(false, rm_speed);
                //motor[1]->move(true, rm_speed/1.5);
                motor[1]->stop();
                moveStraight(&timeold, 1);
                //reduceSpeed(&timeold, 1, 1);
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
                //motor[0]->move(false, lm_speed);
                //motor[0]->move(true, lm_speed/1.5);
                motor[0]->stop();
                motor[1]->move(true, rm_speed);
                moveStraight(&timeold, 0);
                //reduceSpeed(&timeold, 0, 1);
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
                //motor[0]->move(true, lm_speed/2);
                //motor[0]->stop();
                motor[1]->move(true, rm_speed);
                moveStraight(&timeold, -1);
                //reduceSpeed(&timeold, 0, 2);
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
                    //reduceSpeed(&timeold, 1, 4);
                    moveStraight(&timeold, 1);
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
                    //reduceSpeed(&timeold, 0, 4);
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
            //for(i=0; i<N_ULTRASONIC; i++)
            //{
            //    //readingU[i] = (ultrasonic[i]->isDistanceAvailable() > OBSTACLE_DIS ? 0 : 1);    
            //}
            //readingU[2] = (ultrasonic[2]->isDistanceAvailable() > OBSTACLE_DIS ? 0 : 1);
            //Serial.println("to chegando");
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
        //Serial.print("rpm[0]: ");
        //Serial.print(readingRPM[0]);
       // Serial.print(" rpm[1]: ");
      //  Serial.print(readingRPM[1]);
        //Serial.print(" leitura velocidade: ");
        //Serial.print(SPEED_TO_UPDATE_TIME);
        if(stopped_motor != 0)
        {
            if(readingRPM[0]>SPEED_TO_UPDATE_TIME)
            {
                lm_speed -= D_SPEED;
    
                error = SPEED_TO_UPDATE_TIME-readingRPM[0];
                //lm_speed -= myABS(error);
                //Serial.print("cara menor: ");
                if(lm_speed < 0)
                {
                    lm_speed = 0;
                }
            }
            else
            {
                error = SPEED_TO_UPDATE_TIME-readingRPM[0];
                //Serial.print("leitura encoder esquerdo: ");
                //Serial.println(readingRPM[0]);
                //Serial.print("leitura velocidade: ");
                //Serial.println(SPEED_TO_UPDATE_TIME);
                //Serial.print("velocidade - encoder: ");
                //Serial.println(SPEED_TO_UPDATE_TIME - readingRPM[0]);
                //Serial.print("myABS da subtração: ");
                //Serial.println(myABS(SPEED_TO_UPDATE_TIME - readingRPM[0]));
                
                
                //Serial.print(int(SPEED_TO_UPDATE_TIME-readingRPM[0]));
                lm_speed += D_SPEED;
                //lm_speed += myABS(error);
                //Serial.print("cara maior");
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
                //rm_speed -= myABS(error);
                //Serial.print("cara menor: ");
                if(rm_speed < 0)
                {
                    rm_speed = 0;
                }
            }
            else
            {
                error = int(SPEED_TO_UPDATE_TIME)-readingRPM[1];
                rm_speed += D_SPEED;
                //rm_speed += myABS(error);    
                //Serial.print("cara maior: ");            
                if(rm_speed > 255)
                {
                    rm_speed = 255;
                }
            }
        }
        readingRPM[0] = motor[0]->getRPM(*timeold);
        readingRPM[1] = motor[1]->getRPM(*timeold);
        //Serial.print(" v0: ");
        //Serial.print(lm_speed);
        //Serial.print(" v1: ");
        //Serial.println(rm_speed);
        //Serial.print(millis()-*timeold);
        *timeold = millis();
        //Serial1.write("v0: ");
        //char oi[4] = {lm_speed, readingRPM[0], rm_speed, readingRPM[1]};
        //Serial1.write(oi, 4);
        //Serial1.write("\n");
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
            
            /*if(readingRPM[1]>(SPEED_TO_UPDATE_TIME-reason))
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
            }*/
        }
        else
        {
            /*if(readingRPM[0]>(SPEED_TO_UPDATE_TIME-reason))
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
            }*/
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

uint8_t* Robot::getReadingBlackTypeSensor()
{
    int i;
    uint8_t* readingBTS = new uint8_t [N_BLACK_TAPE_SENSOR];
    for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
    {
        readingBTS[i] = black_tape_sensor[i]->getReading();
    }
    return readingBTS;
}   

float* Robot::getReadingUltrasonic()
{
    int i;
    float* detectedObjet_U;
    detectedObjet_U= isDistanceAvailable();
    //Serial.print("Recebendo ");
    //for(i=0; i<N_ULTRASONIC; i++)
    //{
        //detectedObjet_U[i] = ultrasonic[i]->isDistanceAvailable();
        //Serial.print("objeto ");
        //Serial.print(i+1);
      //  Serial.print(detectedObjet_U[i]);
      //  Serial.print(" ");
    //}
    //Serial.println();
    return detectedObjet_U;
    //return 0;
}
