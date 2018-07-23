#include <cmath>
#include <iostream>
#include <time.h>
#include <string.h>
using namespace std;
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/offset_ptr.hpp>
using namespace boost::interprocess;

//variaveis para acesso e criacao de memoria
#define NOME_DA_MEMORIA "memoria"
#define NOME_DA_MEMORIA2 "memoria2"
#define TAMANHO_DA_MEMORIA 65536
#define NOME_DO_INT_NA_MEMORIA1 "comando1"
#define NOME_DO_INT_NA_MEMORIA2 "comando2"

//variaveis robo
#define VEL_MOT 6.25 //tecnicamente em rad/s
#define DIS_RETO 0.49 //tamanho do passo em m
#define D_RETO 0.05 //delta para corrigir o caminho
#define DIS_CURVA 0.262 //delta para virar PI, ja que ha o delay de comunicacao e eh float, nunca igual, (~VEL_MOT/(7.5PI)) - aleatorio
#define D_ANG 0.25 //delta para captacao de salto da atan, (~DIS_CURVA-0.1)


extern "C" {
    #include "remoteApi/extApi.h"
    #include "remoteApi/extApiPlatform.h"


}

int main(int argc, char **argv)
{
    //enderecamentos remoteapi
    string serverIP = "127.0.0.1";
	int serverPort = 19999;
    int clientID=simxStart((simxChar*)serverIP.c_str(),serverPort,true,true,2000,5);
    //inteiros para localizacao dos objetos no vrep
    int leftMotorHandle = 0, rightMotorHandle = 0, bubbleRob=0, virei=0;
    //variaveis para movimentacao do robo
    float vLeft = 0, vRight = 0, dx=0, dy=0, X_inicial=0, Y_inicial=0, d_ang=0, ang_inicial=0, ang=0, positionX=0, positionY=0, dx_inicial = 0, dy_inicial = 0, X_inicial1 = 0, Y_inicial1 = 0;
    //vetores para captacao da posicao na cena
    float* angle = new float[3];
    //float* position = new float[3];

  ///Criação de memória
    shared_memory_object::remove(NOME_DA_MEMORIA2);
    managed_shared_memory* dinamica;
    dinamica = new managed_shared_memory(open_or_create, NOME_DA_MEMORIA2, TAMANHO_DA_MEMORIA);
    int* comando2;// comando para enviar dados a memoria
    comando2 = dinamica->construct<int>(NOME_DO_INT_NA_MEMORIA2)();
    *comando2 = 1;
    ///Finalização

    ///Procurando memória
    managed_shared_memory* abrindo_memoria;
    int i=0;
    bool opened = false;
    while(!opened)
    {
        try //tries to open the vision shared memory until it succeeds
        {
           abrindo_memoria = new managed_shared_memory (open_only, NOME_DA_MEMORIA);
            opened = true;
        }
        catch(...)
        {
            std::cout<<"erro ao abrir memoria 1" << i++ << std::endl;
            sleep(1);//extApi_sleepMs(5);
        }
    }
    ///Fechamento

  ///conexao com elementos da cena
	if (clientID!=-1)
	{
		cout << "Servidor conectado!" << std::endl;

        // inicialização dos motores e do robo
        if(simxGetObjectHandle(clientID,"bubbleRob",&bubbleRob, simx_opmode_oneshot_wait)==simx_return_ok)
                cout << "conectado ao bubbleRob" <<endl;

		if(simxGetObjectHandle(clientID,(const simxChar*) "bubbleRob_leftMotor",(simxInt *) &leftMotorHandle, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
			cout << "Handle do motor esquerdo nao encontrado!" << std::endl;
		else
			cout << "Conectado ao motor esquerdo!" << std::endl;

		if(simxGetObjectHandle(clientID,(const simxChar*) "bubbleRob_rightMotor",(simxInt *) &rightMotorHandle, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
			cout << "Handle do motor direito nao encontrado!" << std::endl;
		else
			cout << "Conectado ao motor direito!" << std::endl;
        //capto a primeira posicao e angulacao
        simxGetObjectOrientation(clientID, bubbleRob, -1, angle, simx_opmode_streaming);//alpha, beta e gamma. Usa-se o gamma
        //simxGetObjectPosition(clientID, bubbleRob, -1, position, simx_opmode_streaming);//x, y, z. Nao usa-se o z
        simxGetFloatSignal(clientID, "gpsX", &positionX, simx_opmode_streaming);
        simxGetFloatSignal(clientID, "gpsY", &positionY, simx_opmode_streaming);
        X_inicial1 = positionX;
        Y_inicial1 = positionY;
	}

    //loop de execucao
	while(simxGetConnectionId(clientID)!=-1 )
    {
		pair<int*, managed_shared_memory::size_type> comando1;
		comando1 = abrindo_memoria->find<int>(NOME_DO_INT_NA_MEMORIA1);
        //simxGetObjectPosition(clientID, bubbleRob, -1, position, simx_opmode_buffer);
        simxGetObjectOrientation(clientID, bubbleRob, -1, angle, simx_opmode_buffer);
        simxGetFloatSignal(clientID, "gpsX", &positionX, simx_opmode_buffer);
        simxGetFloatSignal(clientID, "gpsY", &positionY, simx_opmode_buffer);
        X_inicial = positionX;
        Y_inicial = positionY;;
        //X_inicial = position[0];
        //Y_inicial = position[1];
        //cout << "X_inicial eh " <<X_inicial<<" e Y_inicial eh " << Y_inicial<<endl;
        ang_inicial = angle[2];//virar para esquerda, ang aumenta. direita, diminui

        //linha reta
		if (*(comando1.first) == 8)
        {
			while((dx < DIS_RETO && dy <DIS_RETO))
            {
				vLeft = VEL_MOT;
				vRight = VEL_MOT;
				simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
				simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                simxGetFloatSignal(clientID, "gpsX", &positionX, simx_opmode_buffer);
                simxGetFloatSignal(clientID, "gpsY", &positionY, simx_opmode_buffer);
                //simxGetObjectPosition(clientID, bubbleRob, -1, position, simx_opmode_buffer);
                dx = abs(positionX-X_inicial);
                dy = abs(positionY-Y_inicial);
                dx_inicial = positionX-X_inicial1;
                dy_inicial = positionY-Y_inicial1;
                //cout << "dx e "<<dx<<" e dy e "<<dy<<endl;
                cout << "virei = "<<virei<<endl;

                if(dx_inicial>D_RETO&&virei==0)
                {
                    //giro para a direita
                    while(dx_inicial>D_RETO)
                    {
                        vLeft = VEL_MOT;
        				vRight = -VEL_MOT;
        				simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
        				simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                        simxGetFloatSignal(clientID, "gpsX", &positionX, simx_opmode_buffer);
                        dx_inicial = (positionX-X_inicial1);
                    }
                }
                else if(dy_inicial>D_RETO&&(virei==1||virei==-3))
                {
                    //giro para a direita
                    while(dy_inicial>D_RETO)
                    {
                        vLeft = VEL_MOT;
                        vRight = -VEL_MOT;
                        simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                        simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                        simxGetFloatSignal(clientID, "gpsY", &positionY, simx_opmode_buffer);
                        dy_inicial = (positionY-Y_inicial1);
                    }
                }
                else if(dx_inicial<-D_RETO&&virei==0)
                {
                    //giro esquerda
                    while(dx_inicial<-D_RETO)
                    {
                        vLeft = -VEL_MOT;
                        vRight = VEL_MOT;
                        simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                        simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                        simxGetFloatSignal(clientID, "gpsX", &positionX, simx_opmode_buffer);
                        dx_inicial = (positionX-X_inicial1);
                    }
                }
                else if(dy_inicial<-D_RETO&&(virei==1||virei==-3))
                {
                    //giro esquerda
                    while(dy_inicial<-D_RETO)
                    {
                        vLeft = -VEL_MOT;
                        vRight = VEL_MOT;
                        simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                        simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                        simxGetFloatSignal(clientID, "gpsY", &positionY, simx_opmode_buffer);
                        dy_inicial = (positionY-Y_inicial1);
                    }
                }
                else if(dx_inicial>D_RETO&&(abs(virei)==2))
                {
                    //giro esquerda
                    //while(dx_inicial>D_RETO)
                    //{
                        vLeft = -VEL_MOT;
                        vRight = VEL_MOT;
                        simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                        simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                        simxGetFloatSignal(clientID, "gpsX", &positionX, simx_opmode_buffer);
                        dx_inicial = (positionX-X_inicial1);
                    //}
                }
                else if(dy_inicial>D_RETO&&(virei==-1||virei==3))
                {
                    //giro esquerda
                    //while(dy_inicial>D_RETO)
                    //{
                        vLeft = -VEL_MOT;
                        vRight = VEL_MOT;
                        simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                        simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                        simxGetFloatSignal(clientID, "gpsY", &positionY, simx_opmode_buffer);
                        dy_inicial = (positionY-Y_inicial1);
                    //}
                }
                else if((dx_inicial<D_RETO&&(abs(virei)==2)))
                {
                    //giro para a direita
                    //while(dx_inicial<D_RETO)
                    //{
                        vLeft = VEL_MOT;
                        vRight = -VEL_MOT;
                        simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                        simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                        simxGetFloatSignal(clientID, "gpsX", &positionX, simx_opmode_buffer);
                        dx_inicial = (positionX-X_inicial1);
                    //}
                }
                else if(dy_inicial<D_RETO&&(virei==-1||virei==3))
                {
                    //giro para a direita
                //    while(dy_inicial<-D_RETO)
                //    {
                        vLeft = VEL_MOT;
                        vRight = -VEL_MOT;
                        simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                        simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                        simxGetFloatSignal(clientID, "gpsY", &positionY, simx_opmode_buffer);
                        dy_inicial = (positionY-Y_inicial1);
                //    }
                }
            //    dy = abs(dy);
            //    dx = abs(dx);
            }
		}

        //virar direita
		else if(*(comando1.first) == 6)
        {
            virei--;
            if(virei==-4)
                virei = 0;
			while(d_ang < (M_PI/2-DIS_CURVA))
            {
				vLeft = VEL_MOT;
				vRight = -VEL_MOT;
				simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
				simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                simxGetObjectOrientation(clientID, bubbleRob, -1, angle, simx_opmode_buffer);
                ang = (angle[2]);
                d_ang = abs(ang-ang_inicial);
                //se houve salto da atan, ignora colocando o salto na posicao normal. Como abs, mas funciona melhor
                if(d_ang>(M_PI/2-D_ANG)){
                    ang = -ang;
                    d_ang = abs(ang-ang_inicial);
                }
                cout<<"ang "<<ang<<"dang "<<d_ang<<endl;
			}
            simxGetFloatSignal(clientID, "gpsX", &positionX, simx_opmode_buffer);
            simxGetFloatSignal(clientID, "gpsY", &positionY, simx_opmode_buffer);
            X_inicial1 = positionX;
            Y_inicial1 = positionY;
		}

        //virar esquerda
		else if(*(comando1.first) == 4)
        {
            virei++;
            if(virei==4)
            {
                virei = 0;
            }
            while(d_ang <(M_PI/2-DIS_CURVA))
            {
				vLeft = -VEL_MOT;
				vRight = VEL_MOT;
				simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
				simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                simxGetObjectOrientation(clientID, bubbleRob, -1, angle, simx_opmode_buffer);
                ang = (angle[2]);
                d_ang = abs(ang-ang_inicial);
                //se houve salto da atan, ignora colocando o salto na posicao normal. Como abs, mas funciona melhor
                if(d_ang>(M_PI/2-D_ANG)){
                    ang = -ang;
                    d_ang = abs(ang-ang_inicial);
                }
                cout<<"ang "<<ang<<"dang "<<d_ang<<endl;
			}
            simxGetFloatSignal(clientID, "gpsX", &positionX, simx_opmode_buffer);
            simxGetFloatSignal(clientID, "gpsY", &positionY, simx_opmode_buffer);
            X_inicial1 = positionX;
            Y_inicial1 = positionY;
		}

        //zera variaveis e manda o robo parar
        X_inicial = 0;
        Y_inicial = 0;
        dx = 0.0;
        dy = 0.0;
        d_ang = 0;
        ang = 0;
		vLeft = 0;
		vRight = 0;
        *(comando1.first) = 0;
		simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
		simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
		extApi_sleepMs(5);
	}

    shared_memory_object::remove(NOME_DA_MEMORIA2);//destroi memoria
    simxFinish(clientID); // fechando conexao com o servidor
    cout << "Conexao e Servidor fechados!" << std::endl;
	return 0;
 }
