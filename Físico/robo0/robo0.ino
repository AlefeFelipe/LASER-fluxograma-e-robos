#include "Principal.hpp"
#include "robotCte.hpp"

Principal* principal = new Principal();

void setup() {
    Serial.begin(BDRATE);
    Serial1.begin(BDRATE);
    pinMode(L_ENCODER, INPUT);
    pinMode(R_ENCODER, INPUT);
    pinMode(L_ULTRASONIC_TRIG, INPUT);
    pinMode(L_ULTRASONIC_ECHO, INPUT);
    pinMode(LM_ULTRASONIC_TRIG, INPUT);
    pinMode(LM_ULTRASONIC_ECHO, INPUT);
    pinMode(M_ULTRASONIC_TRIG, INPUT);
    pinMode(M_ULTRASONIC_ECHO, INPUT);
    pinMode(RM_ULTRASONIC_TRIG, INPUT);
    pinMode(RM_ULTRASONIC_ECHO, INPUT);
    pinMode(R_ULTRASONIC_TRIG, INPUT);
    pinMode(R_ULTRASONIC_ECHO, INPUT);
    pinMode(L_BLACK_TAPE_SENSOR, INPUT);
    pinMode(LM_BLACK_TAPE_SENSOR, INPUT);
    pinMode(M_BLACK_TAPE_SENSOR, INPUT);
    pinMode(RM_BLACK_TAPE_SENSOR, INPUT);
    pinMode(R_BLACK_TAPE_SENSOR, INPUT);
    

}

void loop(){principal->start();}
