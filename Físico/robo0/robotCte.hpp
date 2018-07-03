//biblioteca que contém as constantes do robo
#ifndef _ROBOTCEP_HPP_
#define _ROBOTCEP_HPP_

//pinagem dos motores e encoders
#define L_MOTOR 3
#define R_MOTOR 1
#define L_ENCODER 20//2, 3, 18, 19, 20, 21
#define R_ENCODER 2
//pinagem dos sensores. No momento nao eh usado as saidas como vcc e gnd
//pinagem dos sensores de visao
#define L_BLACK_TAPE_SENSOR 38
#define LM_BLACK_TAPE_SENSOR 40
#define M_BLACK_TAPE_SENSOR 42
#define RM_BLACK_TAPE_SENSOR 44
#define R_BLACK_TAPE_SENSOR 46
#define VCC_BLACK_TAPE_SENSOR 0
#define GND_BLACK_TAPE_SENSOR 0
//pinagem dos ultrassons -
#define L_ULTRASONIC_ECHO 24
#define L_ULTRASONIC_TRIG 25
#define L_ULTRASONIC_VCC 0
#define L_ULTRASONIC_GND 0
#define LM_ULTRASONIC_ECHO 48
#define LM_ULTRASONIC_TRIG 49
#define LM_ULTRASONIC_VCC 0
#define LM_ULTRASONIC_GND 0
#define M_ULTRASONIC_ECHO 50
#define M_ULTRASONIC_TRIG 51
#define M_ULTRASONIC_VCC 0
#define M_ULTRASONIC_GND 0
#define RM_ULTRASONIC_ECHO 52
#define RM_ULTRASONIC_TRIG 53
#define RM_ULTRASONIC_VCC 0
#define RM_ULTRASONIC_GND 0
#define R_ULTRASONIC_ECHO 22
#define R_ULTRASONIC_TRIG 23
#define R_ULTRASONIC_VCC 0
#define R_ULTRASONIC_GND 0

//constantes motores
#define RESOLUTION_ENCODER 20 //numero de furos
#define MOTORSPEED 100 //velocidade inicial do motor em relacao nenhuma
#define TIMETURNING 150 //tempo de giro 90º em milisegundos
#define SCALE 0.7
#define UPDATE_DELAY 0
//constantes 
#define BDRATE 115200
#define N_BLACK_TAPE_SENSOR 5
#define N_ULTRASONIC 5
#define PACKETSIZE 25//5 fita, 20 ultrassom
#define ENCODER_UPDATE_TIME 200//tempo em milissigundos para leitura dos encoders
#define SPEED_TO_UPDATE_TIME 10//velocidade em relacao ao update time. Formula: N_VOLTAS*RESOLUTION_ENCODER/(1000/ENCODER_UPDATE_TIME) N_VOLTAS é o numero de voltas por segundo
#define D_SPEED 5//delta para correcao da velocidade
#define OBSTACLE_DIS -1.0
#endif
