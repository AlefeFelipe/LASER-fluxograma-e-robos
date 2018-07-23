//TesteBoostMemory

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/offset_ptr.hpp>

using namespace boost::interprocess;

using namespace std;
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
#define DIS_RETO 0.5 //tamanho do passo em m ate inicio da leitura dos sensores
#define DIS_CURVA 0.262 //delta para virar PI, ja que ha o delay de comunicacao e eh float, nunca igual, (~VEL_MOT/(7.5PI)) - aleatorio
#define D_ANG 0.1 //delta para captacao de salto da atan, (~DIS_CURVA-0.1)
#define MAX_INTE 0.2 //delta para media ta intensidade. tecnicamente, deve ser zero
#define CONT 0 //contador para comecar virar

//para usar kbhit
//#include <sys/ioctl.h>
//#include <termios.h>


int main()
{
  std::cout<<"Comecou!!!" << endl;
  float dx, dy, X_inicial, Y_inicial;
  cout.flush();
  shared_memory_object::remove(NOME_DA_MEMORIA);

    managed_shared_memory* dinamica;
    dinamica = new managed_shared_memory(open_or_create, NOME_DA_MEMORIA, TAMANHO_DA_MEMORIA);
    int* comando1;
    int* bluetooth;
    comando1 = dinamica->construct<int>(NOME_DO_INT_NA_MEMORIA1)();
    bluetooth = dinamica->construct<int>(BLUETOOTH_ENABLE)();



  std::cout<<"Alocou!!!" << endl;
  cout.flush();
    int abertura;

    cout << "1) Robo físico"<<endl << "0) Robô virtual"<<endl;
    cin >> (*bluetooth);
    cout << "confirme abertura de programa"<<endl;
    cin>>abertura;
    if(abertura)
    {

        pair<int*, managed_shared_memory::size_type> comando2;
        comando2 = dinamica->find<int>(NOME_DO_INT_NA_MEMORIA2);

    //exemplo de leitura dos sensores de visao
        pair<unsigned char*, managed_shared_memory::size_type>reading_VS;
        reading_VS = dinamica->find<unsigned char>(SENSOR_VISAO);
    //POSICAO
        pair<float*, managed_shared_memory::size_type> linPosition;
        linPosition = dinamica->find<float>(POSICAO);
    //ORIENTACAO
        pair<float*, managed_shared_memory::size_type> angPosition;
        angPosition = dinamica->find<float>(ANGULAR);
    //SENSOR_ULTRASSOM
        pair<unsigned char*, managed_shared_memory::size_type>reading_U;
        reading_U = dinamica->find<unsigned char>(SENSOR_ULTRASSOM);
        //objetos detectados
        pair<float*, managed_shared_memory::size_type>detectedObjet_U;
        detectedObjet_U = dinamica->find<float>(POSICAO_DETECTADA);
        cout.flush();
        int controle = 0;

        while(controle <= 2)
        {
            cout << "8)Frente" << endl << "6)Direita " << endl << "4)Esquerda" << endl << "1)Fechar"<<endl;
            cin >> (*comando1);
            if(!(*bluetooth))
            {
                X_inicial = (linPosition.first)[0];
                Y_inicial = (linPosition.first)[1];
                while((*comando1)==8 && (dx < DIS_RETO && dy <DIS_RETO)&&!(reading_U.first)[2])
                {
                    dx = abs((linPosition.first)[0]-X_inicial);
                    dy = abs((linPosition.first)[1]-Y_inicial);
                    if((reading_U.first)[2])
                        cout<<"objeto detectado a "<<(detectedObjet_U.first)[2]<<" metros"<<endl;
                }
                if((*comando1)==1)
                {
                    shared_memory_object::remove(NOME_DA_MEMORIA);
                    delete dinamica;
                    return 0;
                }
                *comando1 = 0;
                dx=0;
                dy=0;
                cout<<"leitura dos sensores de visao"<<endl;
                cout<<int(reading_VS.first[0])<<", "<<int(reading_VS.first[1])<<", "<<int(reading_VS.first[2])<<", "<<int(reading_VS.first[3])<<", "<<int(reading_VS.first[4])<<endl;
            }
        }

        shared_memory_object::remove(NOME_DA_MEMORIA);
        delete dinamica;
        cout.flush();
        std::cout<<"Terminou!!" << endl;
    }

    return 0;
}
