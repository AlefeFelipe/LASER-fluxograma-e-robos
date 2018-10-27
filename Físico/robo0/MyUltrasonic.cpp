#include "MyUltrasonic.hpp"
NewPing ultrasonic[N_ULTRASONIC] = 
    { 
      //Inicializa os sensores nos pinos especificados
      //(pino_trigger, pino_echo, distancia_maxima)
      //Sensor 1
      NewPing(L_ULTRASONIC_TRIG, L_ULTRASONIC_ECHO, MAX_DETECTION_DISTANCE),
      //Sensor 2
      NewPing(LM_ULTRASONIC_TRIG, LM_ULTRASONIC_ECHO, MAX_DETECTION_DISTANCE),
      NewPing(M_ULTRASONIC_TRIG, M_ULTRASONIC_ECHO, MAX_DETECTION_DISTANCE),
      NewPing(RM_ULTRASONIC_TRIG, RM_ULTRASONIC_ECHO, MAX_DETECTION_DISTANCE),
      NewPing(R_ULTRASONIC_TRIG, R_ULTRASONIC_ECHO, MAX_DETECTION_DISTANCE),
    };
unsigned long pingTimer[N_ULTRASONIC];
float cm[N_ULTRASONIC];
uint8_t current_ultrasonic = 0;
void initUltrasonics()
{
    pingTimer[0] = millis() + 75;
    for (uint8_t i = 1; i < N_ULTRASONIC; i++)
    {
        pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
    }
}

float* isDistanceAvailable() 
{
    for (uint8_t i = 0; i < N_ULTRASONIC; i++)
    { // Loop entre todos os sensores
        if (millis() >= pingTimer[i])
        {   
            //Serial.print("corrente: "); 
            //Serial.print(current_ultrasonic) ;    
            pingTimer[i] += PING_INTERVAL * N_ULTRASONIC;  
            //if (i == 0 && current_ultrasonic == N_ULTRASONIC - 1)
            //{
            //    oneSensorCycle();
            //}
            ultrasonic[current_ultrasonic].timer_stop();
            current_ultrasonic = i;
            //Serial.print("Agora corrente: ");
            //Serial.print(current_ultrasonic) ;
            cm[current_ultrasonic] = 0;
            ultrasonic[current_ultrasonic].ping_timer(getDistance);
          }
    }
    /*Serial.print("Lendo ");
    for (uint8_t i = 0; i < N_ULTRASONIC; i++)
    {
        Serial.print(cm[i]);
        Serial.print(" ");
    }
    Serial.println();*/
    float* distance = cm;
    return distance;
}

void getDistance() 
{ 
  //Se receber um sinal (eco), calcula a distancia
  if (ultrasonic[current_ultrasonic].check_timer())
    cm[current_ultrasonic] = ultrasonic[current_ultrasonic].ping_result / US_ROUNDTRIP_CM;
}

void oneSensorCycle() 
{ 
  // Ciclo de leitura do sensor
  /*for (uint8_t i = 0; i < N_ULTRASONIC; i++) 
  {
    //Imprime os valores lidos pelos sensores, no serial monitor
    Serial.print("Sensor : ");
    Serial.print(i); 
    Serial.print(" = ");
    Serial.print(cm[i]);
    Serial.print(" cm - ");
  }*/
  Serial.println("Agora ta pronto");
}
