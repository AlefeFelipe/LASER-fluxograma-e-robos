#ifndef _FLUX_PROG_BACKEND_CTE_H_
#define _FLUX_PROG_BACKEND_CTE_H_

//variaveis para acesso e criacao de memoria
#define NOME_DA_MEMORIA "memoria"
#define NOME_DA_MEMORIA2 "memoria2"
#define TAMANHO_DA_MEMORIA 65536
#define NOME_DO_INT_NA_MEMORIA1 "comando1"
#define NOME_DO_INT_NA_MEMORIA2 "comando2"
#define SENSOR_VISAO "reading_VS"
#define SENSOR_ULTRASSOM "readin_U"
#define POSICAO_DETECTADA "detectedObjet_U"
#define POSICAO "linPosition"
#define ANGULAR "angPosition"
#define BLUETOOTH_ENABLE "bluetooth"

//variaveis robo
#define VEL_MOT 4.25 //tecnicamente em rad/s
#define DIS_RETO 0.05 //tamanho do passo em m ate inicio da leitura dos sensores
#define DIS_CURVA 0.262 //delta para virar PI, ja que ha o delay de comunicacao e eh float, nunca igual, (~VEL_MOT/(7.5PI)) - aleatorio
#define D_ANG 0.1 //delta para captacao de salto da atan, (~DIS_CURVA-0.1)
#define MAX_INTE 0.2 //delta para media ta intensidade. tecnicamente, deve ser zero
#define CONT 0 //contador para comecar virar
#define N_MOTORS 2
#define ANGLE_COMPONENTS 3

//variaveis bluetooth
#define BDRATE 115200
#define CPORT_NR 16
#define SIZEPACKET 15
#define N_ULTRASONIC 3
#define N_BLACK_TAPE_SENSOR 5
#define N_COLOR_SENSOR 2
#define ULTRASONIC_SENSOR_COMPONENTS 3
#define VISION_SENSOR_COMPONENTS 15

//variaveis de communicacao entre back e front end
#define CLOSE_PROGRAM -10
#define ERROR -5
#define READY 1
#define EXECUTING 0
#define MOVE_FORWARD 8
#define TURN_LEFT 4
#define TURN_RIGHT 6
#endif
