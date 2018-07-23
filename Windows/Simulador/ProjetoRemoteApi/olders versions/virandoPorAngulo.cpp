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
#define DIS_CURVA 0.262 //delta para virar PI, ja que ha o delay de comunicacao e eh float, nunca igual, (~VEL_MOT/(7.5PI)) - aleatorio
#define D_ANG 0.25 //delta para captacao de salto da atan, (~DIS_CURVA-0.1)
#define MAX_INTE 0.25


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
    //inteiros para partes moveis
    int leftMotorHandle = 0, rightMotorHandle = 0, bubbleRob=0, detectedObjetHandle = 0;
    //inteiros para sensores
    int bubbleRob_sensingNose=0, Left_Vision_sensor = 0, LM_Vision_sensor = 0, Middle_Vision_sensor = 0, RM_Vision_sensor = 0, Right_Vision_sensor = 0;
    //variaveis para movimentacao do robo
    float vLeft = 0, vRight = 0, dx=0, dy=0, X_inicial=0, Y_inicial=0, d_ang=0, ang_inicial=0, ang=0;
    //vetores para captacao da posicao na cena
    float* angle = new float[3];
    float* position = new float[3];
    //variaveis sensor noise
    float* detectedObjet = new float[3];
    float* detectedSurface = new float[3];
    unsigned char leitura = 0;//leitura do sensor noise
    //variaveis sensores de visao
    unsigned char leituraLVS = 0;
    float* DataLVS = new float [15];
    int* auxLVS;
    unsigned char leituraLMVS = 0;
    float* DataLMVS = new float [15];
    int* auxLMVS;
    unsigned char leituraMVS = 0;
    float* DataMVS = new float [15];
    int* auxMVS;
    unsigned char leituraRMVS = 0;
    float* DataRMVS = new float [15];
    int* auxRMVS;
    unsigned char leituraRVS = 0;
    float* DataRVS = new float [15];
    int* auxRVS;
    //ta td certo mesmo, so ver se os valores estao coerentes


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
        if(simxGetObjectHandle(clientID, "bubbleRob_sensingNose", &bubbleRob_sensingNose, simx_opmode_oneshot_wait )==simx_return_ok)
                cout << "conectado ao sensor nose" <<endl;
        if(simxGetObjectHandle(clientID, "Left_Vision_sensor", &Left_Vision_sensor, simx_opmode_oneshot_wait)==simx_return_ok&&
            simxGetObjectHandle(clientID, "LM_Vision_sensor", &LM_Vision_sensor, simx_opmode_oneshot_wait)==simx_return_ok&&
            simxGetObjectHandle(clientID, "Middle_Vision_sensor", &Middle_Vision_sensor, simx_opmode_oneshot_wait)==simx_return_ok&&
            simxGetObjectHandle(clientID, "RM_Vision_sensor", &RM_Vision_sensor, simx_opmode_oneshot_wait)==simx_return_ok&&
            simxGetObjectHandle(clientID, "Right_Vision_sensor", &Right_Vision_sensor, simx_opmode_oneshot_wait)==simx_return_ok
          )
                cout << "conectado aos sensores de visao" <<endl;

		if(simxGetObjectHandle(clientID,(const simxChar*) "bubbleRob_leftMotor",(simxInt *) &leftMotorHandle, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
			cout << "Handle do motor esquerdo nao encontrado!" << std::endl;
		else
			cout << "Conectado ao motor esquerdo!" << std::endl;

		if(simxGetObjectHandle(clientID,(const simxChar*) "bubbleRob_rightMotor",(simxInt *) &rightMotorHandle, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
			cout << "Handle do motor direito nao encontrado!" << std::endl;
		else
			cout << "Conectado ao motor direito!" << std::endl;
        simxGetObjectOrientation(clientID, bubbleRob, -1, angle, simx_opmode_streaming);//alpha, beta e gamma. Usa-se o gamma
        simxGetObjectPosition(clientID, bubbleRob, -1, position, simx_opmode_streaming);//x, y, z. Nao usa-se o z
        simxReadProximitySensor(clientID, bubbleRob_sensingNose, &leitura, detectedObjet, &detectedObjetHandle, detectedSurface, simx_opmode_streaming);
        simxReadVisionSensor(clientID, Left_Vision_sensor, &leituraLVS, &DataLVS, &auxLVS, simx_opmode_streaming);
        simxReadVisionSensor(clientID, LM_Vision_sensor, &leituraLMVS, &DataLMVS, &auxLMVS, simx_opmode_streaming);
        simxReadVisionSensor(clientID, Middle_Vision_sensor, &leituraMVS, &DataMVS, &auxMVS, simx_opmode_streaming);
        simxReadVisionSensor(clientID, RM_Vision_sensor, &leituraRMVS, &DataRMVS, &auxRMVS, simx_opmode_streaming);
        simxReadVisionSensor(clientID, Right_Vision_sensor, &leituraRVS, &DataRVS, &auxRVS, simx_opmode_streaming);
	}

    //loop de execucao
	while(simxGetConnectionId(clientID)!=-1 )
    {
		pair<int*, managed_shared_memory::size_type> comando1;
		comando1 = abrindo_memoria->find<int>(NOME_DO_INT_NA_MEMORIA1);
        simxGetObjectPosition(clientID, bubbleRob, -1, position, simx_opmode_buffer);
        simxGetObjectOrientation(clientID, bubbleRob, -1, angle, simx_opmode_buffer);
        X_inicial = position[0];
        Y_inicial = position[1];
        ang_inicial = angle[2];//virar para esquerda, ang aumenta. direita, diminui
        simxReadProximitySensor(clientID, bubbleRob_sensingNose, &leitura, detectedObjet, &detectedObjetHandle, detectedSurface, simx_opmode_buffer);
        simxReadVisionSensor(clientID, Left_Vision_sensor, &leituraLVS, &DataLVS, &auxLVS, simx_opmode_buffer);
        simxReadVisionSensor(clientID, LM_Vision_sensor, &leituraLMVS, &DataLMVS, &auxLMVS, simx_opmode_buffer);
        simxReadVisionSensor(clientID, Middle_Vision_sensor, &leituraMVS, &DataMVS, &auxMVS, simx_opmode_buffer);
        simxReadVisionSensor(clientID, RM_Vision_sensor, &leituraRMVS, &DataRMVS, &auxRMVS, simx_opmode_buffer);
        simxReadVisionSensor(clientID, Right_Vision_sensor, &leituraRVS, &DataRVS, &auxRVS, simx_opmode_buffer);
        leituraLVS = DataLVS[10]<MAX_INTE;//media da intensidade. No caso preto, sao todos 0
        leituraLMVS = DataLMVS[10]<MAX_INTE;
        leituraMVS = DataMVS[10]<MAX_INTE;
        leituraRMVS = DataRMVS[10]<MAX_INTE;
        leituraRVS = DataRVS[10]<MAX_INTE;
        cout<<"sensores resultaram em " << static_cast<int>(leituraLVS) <<" "<<static_cast<int>(leituraLMVS)<<" "<<static_cast<int>(leituraMVS)<<" "<<static_cast<int>(leituraRMVS)<<" "<<static_cast<int>(leituraRVS)<<endl;
        //linha reta (se nao estiver perto de um obstaculo)
		if (*(comando1.first) == 8 && leitura==0)
        {
			while((dx < DIS_RETO && dy <DIS_RETO) && leitura==0)
            {
				vLeft = VEL_MOT;
				vRight = VEL_MOT;
				simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
				simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                simxGetObjectPosition(clientID, bubbleRob, -1, position, simx_opmode_buffer);
                simxReadProximitySensor(clientID, bubbleRob_sensingNose, &leitura, detectedObjet, &detectedObjetHandle, detectedSurface, simx_opmode_buffer);
                dx = abs(position[0]-X_inicial);
                dy = abs(position[1]-Y_inicial);
            }
		}

        //virar direita
		else if(*(comando1.first) == 6)
        {
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
                if(d_ang>(M_PI/2-D_ANG))
                {
                    ang = -ang;
                    d_ang = abs(ang-ang_inicial);
                }
			}
		}

        //virar esquerda
		else if(*(comando1.first) == 4)
        {
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
                if(d_ang>(M_PI/2-D_ANG))
                {
                    ang = -ang;
                    d_ang = abs(ang-ang_inicial);
                }
			}
		}

        //zera variaveis e manda o robo parar
        X_inicial = 0;
        Y_inicial = 0;
        dx = 0;
        dy = 0;
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
