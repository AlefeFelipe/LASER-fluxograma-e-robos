//biblioteca que contém as constantes do robo
#ifndef _ROBOTCEP_HPP_
#define _ROBOTCEP_HPP_

//pinagem dos motores e encoders
#define L_MOTOR 1
#define R_MOTOR 2
#define L_ENCODER 21//2, 3, 18, 19, 20, 21
#define R_ENCODER 20
//pinagem dos sensores. No momento nao eh usado as saidas como vcc e gnd
//pinagem dos sensores de visao
#define L_BLACK_TAPE_SENSOR 44
#define LM_BLACK_TAPE_SENSOR 46
#define M_BLACK_TAPE_SENSOR 48
#define RM_BLACK_TAPE_SENSOR 50
#define R_BLACK_TAPE_SENSOR 52
#define OBSTACLE_SENSOR 22
//pinagem dos ultrassons -
#define L_ULTRASONIC_ECHO 53
#define L_ULTRASONIC_TRIG 51
#define LM_ULTRASONIC_ECHO 33
#define LM_ULTRASONIC_TRIG 31
#define M_ULTRASONIC_ECHO 37
#define M_ULTRASONIC_TRIG 35
#define RM_ULTRASONIC_ECHO 41
#define RM_ULTRASONIC_TRIG 39
#define R_ULTRASONIC_ECHO 49
#define R_ULTRASONIC_TRIG 47
//pinagem sensores de cor
#define L_COLOR_SENSOR_SDA 23 
#define L_COLOR_SENSOR_SCL 22
#define R_COLOR_SENSOR_SDA 25
#define R_COLOR_SENSOR_SCL 24


//constantes motores
#define RESOLUTION_ENCODER 20 //numero de furos
#define MOTORSPEED 100 //velocidade inicial do motor em relacao nenhuma
#define TIMETURNING 100 //tempo de giro 90º em milisegundos
#define SCALE 0.6 //por enquanto, depois olhar um para o outro
#define UPDATE_DELAY 0
//constantes 
#define BDRATE 115200
#define N_BLACK_TAPE_SENSOR 5
#define N_ULTRASONIC 5
#define N_COLOR_SENSOR 2
#define COLOR_SENSOR_GAIN 2//grau de ganho, aqui vai de 1 a 4, no sensor de 1 a 60
#define PACKETSIZE 15//5 fita, 10 ultrassom
#define COMMUNICATION_DELAY 30//tempo de delay do bluetooth em ms
#define ENCODER_UPDATE_TIME 200//tempo em milissigundos para leitura dos encoders
#define SPEED_TO_UPDATE_TIME 2//velocidade em relacao ao update time. Formula: N_VOLTAS*RESOLUTION_ENCODER/(1000/ENCODER_UPDATE_TIME) N_VOLTAS é o numero de voltas por segundo
#define D_SPEED 5//delta para correcao da velocidade
#define OBSTACLE_DIS 10
#define ULTRASONIC_TIMEOUT 2940 //maximo tempo de espera pela leitura do ultrassom, formula:     int timeout = (MAX_DETECTION_DISTANCE*1000000)/17000;
#define PING_INTERVAL 33//tempo entre a leitura de um ultrassom e outro para evitar conflito
#endif

//tentar o seguinte: olhar o encoder 2x por segundo. ver qual é a velocidade que se quer, para a velocidade inicial estar próxima do valor
//fazer a velocidade cair 1.5, 2 e 4. Depois fazer isso acontecer rapidamente. Criar um cara que será a diferença entre os caras
//depois seguir linha é pra funcionar tranquilamente, mas se n funcionar acrescentar +2 graus de cada lado
