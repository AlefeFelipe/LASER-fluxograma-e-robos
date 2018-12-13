#include "Motor.hpp"

int pulses_l = 0;
int pulses_r = 0;
int pulse_counter_l()
{
    pulses_l++;
}
void pulse_counter_r()
{
    pulses_r++;
}

Motor::Motor(int name) : AF_DCMotor(name)
{
    if(name==L_MOTOR)
    {
        pin_encoder = L_ENCODER;
        attachInterrupt(digitalPinToInterrupt(pin_encoder), pulse_counter_l, FALLING);
    }
    else if(name==R_MOTOR)
    {
        pin_encoder = R_ENCODER;
        attachInterrupt(digitalPinToInterrupt(pin_encoder), pulse_counter_r, FALLING);
    }
    resolution = RESOLUTION_ENCODER;
    //attachInterrupt(digitalPinToInterrupt(pin_encoder), &Motor::pulse_counter, FALLING);
    //colocar aqui o attach também. Necessario nova classe?
}

//fazer: velocidade regulada pelo encoder, direcao pela ponte h
void Motor::move(bool direction, int speed)
{
    setSpeed(speed);
    if(direction == true)
    {
        run(FORWARD);
    }
    else if(direction == false)
    {
        run(BACKWARD);
    }
}

void Motor::stop()
{
    run(RELEASE);
}

int Motor::getRPM(long timeold)
{
    int rpm  = 0;
    //Serial.print("Encoder: ");
    //Serial.println(pin_encoder);
    //Serial.print("pin interrupcao: ");
   // Serial.println(digitalPinToInterrupt(pin_encoder));
    //if (millis() - timeold >= 10)
    //{
      //Desabilita interrupcao durante o calculo
      detachInterrupt(digitalPinToInterrupt(pin_encoder));
      //rpm = ((60 * 1000 / resolution) / (millis() - timeold)) * pulses;
      if(pin_encoder == L_ENCODER)
      {
          rpm = pulses_l;// resolution;
          //Serial.println(pulses_l);
          
          if(millis()-timeold>=long(ENCODER_UPDATE_TIME))
          {
              pulses_l = 0;
          }
          attachInterrupt(digitalPinToInterrupt(pin_encoder), pulse_counter_l, FALLING);
      }
      else
      {
          rpm = pulses_r;// resolution;
          //Serial.println(pulses_r);
          if(millis()-timeold>=long(ENCODER_UPDATE_TIME))
          {
              pulses_r = 0;
          }
          attachInterrupt(digitalPinToInterrupt(pin_encoder), pulse_counter_r, FALLING);
      }
      //Serial.println(millis()-timeold);
    //}
    return rpm;
}
