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
#define VEL_MOT 4.25 //tecnicamente em rad/s
#define DIS_RETO 0.065 //tamanho do passo em m ate inicio da leitura dos sensores
#define DIS_CURVA 0.262 //delta para virar PI, ja que ha o delay de comunicacao e eh float, nunca igual, (~VEL_MOT/(7.5PI)) - aleatorio
#define D_ANG 0.1 //delta para captacao de salto da atan, (~DIS_CURVA-0.1)
#define MAX_INTE 0.2 //delta para media ta intensidade. tecnicamente, deve ser zero
#define CONT 0 //contador para comecar virar


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
    int leftMotorHandle = 0, rightMotorHandle = 0, bubbleRob=0, detectedObjetHandle = 0, turnleft=0, turnright=0;
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
    unsigned char readingUSS = 0;//leitura do sensor noise
    //variaveis sensores de visao
    unsigned char readingLVS = 0;
    float* DataLVS = new float [15];
    int* auxLVS;
    unsigned char readingLMVS = 0;
    float* DataLMVS = new float [15];
    int* auxLMVS;
    unsigned char readingMVS = 0;
    float* DataMVS = new float [15];
    int* auxMVS;
    unsigned char readingRMVS = 0;
    float* DataRMVS = new float [15];
    int* auxRMVS;
    unsigned char readingRVS = 0;
    float* DataRVS = new float [15];
    int* auxRVS;
    //ta td certo mesmo, so ver se os valores estao coerentes


    ///Finalização

    ///Procurando memória


	//mudei para o meu programa criar a unica memoria usada e as variáveis dela

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
	pair<int*, managed_shared_memory::size_type> comando2; 
	comando2 = abrindo_memoria->find<int>(NOME_DO_INT_NA_MEMORIA2);
	pair<int*, managed_shared_memory::size_type> comando1;
	comando1 = abrindo_memoria->find<int>(NOME_DO_INT_NA_MEMORIA1);
	
    ///Fechamento

  ///conexao com elementos da cena
	if (clientID!=-1)
	{
	bool tudo_ok = true;
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

		if(simxGetObjectHandle(clientID,(const simxChar*) "bubbleRob_leftMotor",(simxInt *) &leftMotorHandle, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok) { 
			cout << "Handle do motor esquerdo nao encontrado!" << std::endl;
			tudo_ok = false;}
		else
			cout << "Conectado ao motor esquerdo!" << std::endl;

		if(simxGetObjectHandle(clientID,(const simxChar*) "bubbleRob_rightMotor",(simxInt *) &rightMotorHandle, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok){
			cout << "Handle do motor direito nao encontrado!" << std::endl;
			tudo_ok = false;}		
		else
			cout << "Conectado ao motor direito!" << std::endl;
        simxGetObjectOrientation(clientID, bubbleRob, -1, angle, simx_opmode_streaming);//alpha, beta e gamma. Usa-se o gamma
        simxGetObjectPosition(clientID, bubbleRob, -1, position, simx_opmode_streaming);//x, y, z. Nao usa-se o z
        simxReadProximitySensor(clientID, bubbleRob_sensingNose, &readingUSS, detectedObjet, &detectedObjetHandle, detectedSurface, simx_opmode_streaming);
        simxReadVisionSensor(clientID, Left_Vision_sensor, &readingLVS, &DataLVS, &auxLVS, simx_opmode_streaming);
        simxReadVisionSensor(clientID, LM_Vision_sensor, &readingLMVS, &DataLMVS, &auxLMVS, simx_opmode_streaming);
        simxReadVisionSensor(clientID, Middle_Vision_sensor, &readingMVS, &DataMVS, &auxMVS, simx_opmode_streaming);
        simxReadVisionSensor(clientID, RM_Vision_sensor, &readingRMVS, &DataRMVS, &auxRMVS, simx_opmode_streaming);
        simxReadVisionSensor(clientID, Right_Vision_sensor, &readingRVS, &DataRVS, &auxRVS, simx_opmode_streaming);
	if (!tudo_ok) *(comando2.first) = -5; // se a conexão com o vrep falha o programa manda -5 para indicara falha
	else { 
		*(comando2.first) = -4; // se a conexão der certo manda -4
	}
	}
	else *(comando2.first) = -5;
    //loop de execucao
	while(simxGetConnectionId(clientID)!=-1 && *(comando1.first) != -10) // se o programa recebe -10 ele deve terminar
    {
        simxGetObjectPosition(clientID, bubbleRob, -1, position, simx_opmode_buffer);
        simxGetObjectOrientation(clientID, bubbleRob, -1, angle, simx_opmode_buffer);
        X_inicial = position[0];
        Y_inicial = position[1];
        ang_inicial = angle[2];//virar para esquerda, ang aumenta. direita, diminui
        simxReadProximitySensor(clientID, bubbleRob_sensingNose, &readingUSS, detectedObjet, &detectedObjetHandle, detectedSurface, simx_opmode_buffer);
        simxReadVisionSensor(clientID, Left_Vision_sensor, &readingLVS, &DataLVS, &auxLVS, simx_opmode_buffer);
        simxReadVisionSensor(clientID, LM_Vision_sensor, &readingLMVS, &DataLMVS, &auxLMVS, simx_opmode_buffer);
        simxReadVisionSensor(clientID, Middle_Vision_sensor, &readingMVS, &DataMVS, &auxMVS, simx_opmode_buffer);
        simxReadVisionSensor(clientID, RM_Vision_sensor, &readingRMVS, &DataRMVS, &auxRMVS, simx_opmode_buffer);
        simxReadVisionSensor(clientID, Right_Vision_sensor, &readingRVS, &DataRVS, &auxRVS, simx_opmode_buffer);
        readingLVS = DataLVS[10]<MAX_INTE;//media da intensidade. No caso preto, sao todos 0
        readingLMVS = DataLMVS[10]<MAX_INTE;
        readingMVS = DataMVS[10]<MAX_INTE;
        readingRMVS = DataRMVS[10]<MAX_INTE;
        readingRVS = DataRVS[10]<MAX_INTE;

        //linha reta (se nao estiver perto de um obstaculo)
		if (*(comando1.first) == 8 && readingUSS==0)
        {
            /*while((dx < DIS_RETO && dy <DIS_RETO) && readingUSS==0)//escapar do estado de verificacao dos sensores
            {
				vLeft = VEL_MOT;
				vRight = VEL_MOT;
				simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
				simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                simxGetObjectPosition(clientID, bubbleRob, -1, position, simx_opmode_buffer);
                simxReadProximitySensor(clientID, bubbleRob_sensingNose, &readingUSS, detectedObjet, &detectedObjetHandle, detectedSurface, simx_opmode_buffer);
                dx = abs(position[0]-X_inicial);
                dy = abs(position[1]-Y_inicial);
            }*/
            while(((readingLVS&&readingMVS)||(readingMVS&&readingRVS)) && readingUSS==0)//verificar se ta no mesmo estado, tecnicamente inutil devido a funcao acima
            {
                vLeft = VEL_MOT;
                vRight = VEL_MOT;
                simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                simxReadProximitySensor(clientID, bubbleRob_sensingNose, &readingUSS, detectedObjet, &detectedObjetHandle, detectedSurface, simx_opmode_buffer);
                simxReadVisionSensor(clientID, Left_Vision_sensor, &readingLVS, &DataLVS, &auxLVS, simx_opmode_buffer);
                simxReadVisionSensor(clientID, Middle_Vision_sensor, &readingMVS, &DataMVS, &auxMVS, simx_opmode_buffer);
                simxReadVisionSensor(clientID, Right_Vision_sensor, &readingRVS, &DataRVS, &auxRVS, simx_opmode_buffer);
                readingLVS = DataLVS[10]<MAX_INTE;//media da intensidade. No caso preto, sao todos 0
                readingMVS = DataMVS[10]<MAX_INTE;
                readingRVS = DataRVS[10]<MAX_INTE;
                extApi_sleepMs(5);
            }
            while(!((readingLVS&&readingMVS)||(readingMVS&&readingRVS)) && readingUSS==0)//anda ate os sensores captarem a linha ou obstaculo
            {
                vLeft = VEL_MOT;
                vRight = VEL_MOT;
                simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                simxReadProximitySensor(clientID, bubbleRob_sensingNose, &readingUSS, detectedObjet, &detectedObjetHandle, detectedSurface, simx_opmode_buffer);
                simxReadVisionSensor(clientID, Left_Vision_sensor, &readingLVS, &DataLVS, &auxLVS, simx_opmode_buffer);
                simxReadVisionSensor(clientID, LM_Vision_sensor, &readingLMVS, &DataLMVS, &auxLMVS, simx_opmode_buffer);
                simxReadVisionSensor(clientID, Middle_Vision_sensor, &readingMVS, &DataMVS, &auxMVS, simx_opmode_buffer);
                simxReadVisionSensor(clientID, RM_Vision_sensor, &readingRMVS, &DataRMVS, &auxRMVS, simx_opmode_buffer);
                simxReadVisionSensor(clientID, Right_Vision_sensor, &readingRVS, &DataRVS, &auxRVS, simx_opmode_buffer);
                readingLVS = DataLVS[10]<MAX_INTE;//media da intensidade. No caso preto, sao todos 0
                readingLMVS = DataLMVS[10]<MAX_INTE;
                readingMVS = DataMVS[10]<MAX_INTE;
                readingRMVS = DataRMVS[10]<MAX_INTE;
                readingRVS = DataRVS[10]<MAX_INTE;
                //if(!readingMVS&&(readingRMVS||readingRVS))//robo esta desviando para a esquerda, gira direita
                if(readingRVS)
                {
                    vLeft = VEL_MOT;
                    vRight = VEL_MOT/4;
                    simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                    simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                    simxReadProximitySensor(clientID, bubbleRob_sensingNose, &readingUSS, detectedObjet, &detectedObjetHandle, detectedSurface, simx_opmode_buffer);
                    simxReadVisionSensor(clientID, Left_Vision_sensor, &readingLVS, &DataLVS, &auxLVS, simx_opmode_buffer);
                    simxReadVisionSensor(clientID, Middle_Vision_sensor, &readingMVS, &DataMVS, &auxMVS, simx_opmode_buffer);
                    simxReadVisionSensor(clientID, Right_Vision_sensor, &readingRVS, &DataRVS, &auxRVS, simx_opmode_buffer);
                    readingLVS = DataLVS[10]<MAX_INTE;//media da intensidade. No caso preto, sao todos 0
                    readingMVS = DataMVS[10]<MAX_INTE;
                    readingRVS = DataRVS[10]<MAX_INTE;
                    turnright = 1;
                    turnleft = 0;
                }
                else if(readingRMVS)
                {
                    vLeft = VEL_MOT;
                    vRight = VEL_MOT/2;
                    simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                    simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                    simxReadProximitySensor(clientID, bubbleRob_sensingNose, &readingUSS, detectedObjet, &detectedObjetHandle, detectedSurface, simx_opmode_buffer);
                    simxReadVisionSensor(clientID, Left_Vision_sensor, &readingLVS, &DataLVS, &auxLVS, simx_opmode_buffer);
                    simxReadVisionSensor(clientID, Middle_Vision_sensor, &readingMVS, &DataMVS, &auxMVS, simx_opmode_buffer);
                    simxReadVisionSensor(clientID, Right_Vision_sensor, &readingRVS, &DataRVS, &auxRVS, simx_opmode_buffer);
                    readingLVS = DataLVS[10]<MAX_INTE;//media da intensidade. No caso preto, sao todos 0
                    readingMVS = DataMVS[10]<MAX_INTE;
                    readingRVS = DataRVS[10]<MAX_INTE;
                    turnright = 1;
                    turnleft = 0;
                }
                else if(readingLMVS)//robo ta desviando para a direita, gira esquerda
                {
                    vLeft = VEL_MOT/2;
                    vRight = VEL_MOT;
                    simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                    simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                    simxReadProximitySensor(clientID, bubbleRob_sensingNose, &readingUSS, detectedObjet, &detectedObjetHandle, detectedSurface, simx_opmode_buffer);
                    simxReadVisionSensor(clientID, Left_Vision_sensor, &readingLVS, &DataLVS, &auxLVS, simx_opmode_buffer);
                    simxReadVisionSensor(clientID, Middle_Vision_sensor, &readingMVS, &DataMVS, &auxMVS, simx_opmode_buffer);
                    simxReadVisionSensor(clientID, Right_Vision_sensor, &readingRVS, &DataRVS, &auxRVS, simx_opmode_buffer);
                    readingLVS = DataLVS[10]<MAX_INTE;//media da intensidade. No caso preto, sao todos 0
                    readingMVS = DataMVS[10]<MAX_INTE;
                    readingRVS = DataRVS[10]<MAX_INTE;
                    turnright = 0;
                    turnleft = 1;
                }
                else if(readingLVS)//robo ta desviando para a direita, gira esquerda
                {
                    vLeft = VEL_MOT/4;
                    vRight = VEL_MOT;
                    simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                    simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                    simxReadProximitySensor(clientID, bubbleRob_sensingNose, &readingUSS, detectedObjet, &detectedObjetHandle, detectedSurface, simx_opmode_buffer);
                    simxReadVisionSensor(clientID, Left_Vision_sensor, &readingLVS, &DataLVS, &auxLVS, simx_opmode_buffer);
                    simxReadVisionSensor(clientID, Middle_Vision_sensor, &readingMVS, &DataMVS, &auxMVS, simx_opmode_buffer);
                    simxReadVisionSensor(clientID, Right_Vision_sensor, &readingRVS, &DataRVS, &auxRVS, simx_opmode_buffer);
                    readingLVS = DataLVS[10]<MAX_INTE;//media da intensidade. No caso preto, sao todos 0
                    readingMVS = DataMVS[10]<MAX_INTE;
                    readingRVS = DataRVS[10]<MAX_INTE;
                    turnright = 0;
                    turnleft = 1;
                }
                else if(!(readingLVS||readingLMVS||readingMVS||readingRMVS||readingRMVS))
                {
                    if(turnright)
                    {
                        vLeft = VEL_MOT;
                        vRight = VEL_MOT/4;
                        simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                        simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                        simxReadProximitySensor(clientID, bubbleRob_sensingNose, &readingUSS, detectedObjet, &detectedObjetHandle, detectedSurface, simx_opmode_buffer);
                        simxReadVisionSensor(clientID, Left_Vision_sensor, &readingLVS, &DataLVS, &auxLVS, simx_opmode_buffer);
                        simxReadVisionSensor(clientID, Middle_Vision_sensor, &readingMVS, &DataMVS, &auxMVS, simx_opmode_buffer);
                        simxReadVisionSensor(clientID, Right_Vision_sensor, &readingRVS, &DataRVS, &auxRVS, simx_opmode_buffer);
                        readingLVS = DataLVS[10]<MAX_INTE;//media da intensidade. No caso preto, sao todos 0
                        readingMVS = DataMVS[10]<MAX_INTE;
                        readingRVS = DataRVS[10]<MAX_INTE;
                    }
                    else if(turnleft)
                    {
                        vLeft = VEL_MOT/4;
                        vRight = VEL_MOT;
                        simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                        simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                        simxReadProximitySensor(clientID, bubbleRob_sensingNose, &readingUSS, detectedObjet, &detectedObjetHandle, detectedSurface, simx_opmode_buffer);
                        simxReadVisionSensor(clientID, Left_Vision_sensor, &readingLVS, &DataLVS, &auxLVS, simx_opmode_buffer);
                        simxReadVisionSensor(clientID, Middle_Vision_sensor, &readingMVS, &DataMVS, &auxMVS, simx_opmode_buffer);
                        simxReadVisionSensor(clientID, Right_Vision_sensor, &readingRVS, &DataRVS, &auxRVS, simx_opmode_buffer);
                        readingLVS = DataLVS[10]<MAX_INTE;//media da intensidade. No caso preto, sao todos 0
                        readingMVS = DataMVS[10]<MAX_INTE;
                        readingRVS = DataRVS[10]<MAX_INTE;
                    }
                }
                extApi_sleepMs(5);
            }
            simxGetObjectPosition(clientID, bubbleRob, -1, position, simx_opmode_buffer);
            X_inicial = position[0];
            Y_inicial = position[1];
            dx=0;
            dy=0;
            while((dx < DIS_RETO && dy <DIS_RETO) && readingUSS==0)//chegar na linha
            {
                vLeft = VEL_MOT;
                vRight = VEL_MOT;
                simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                simxGetObjectPosition(clientID, bubbleRob, -1, position, simx_opmode_buffer);
                simxReadProximitySensor(clientID, bubbleRob_sensingNose, &readingUSS, detectedObjet, &detectedObjetHandle, detectedSurface, simx_opmode_buffer);
                dx = abs(position[0]-X_inicial);
                dy = abs(position[1]-Y_inicial);
                extApi_sleepMs(5);
            }
            *(comando2.first) = 1;
*(comando1.first) = 0;		
}

        //virar direita
		else if(*(comando1.first) == 6)
        {
            while(d_ang < (M_PI/4-DIS_CURVA))//escapar do estado de verificacao dos sensores
            {
				vLeft = VEL_MOT/1.5;
				vRight = -VEL_MOT/1.5;
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
                extApi_sleepMs(5);
			}
            simxReadVisionSensor(clientID, Middle_Vision_sensor, &readingMVS, &DataMVS, &auxMVS, simx_opmode_buffer);
            simxReadVisionSensor(clientID, RM_Vision_sensor, &readingRMVS, &DataRMVS, &auxRMVS, simx_opmode_buffer);
            readingMVS = DataMVS[10]<MAX_INTE;
            readingRMVS = DataRMVS[10]<MAX_INTE;
            while(readingMVS)//verificar se ta no mesmo estado, tecnicamente inutil devido a funcao acima
            {
				vLeft = VEL_MOT/1.5;
				vRight = -VEL_MOT/1.5;
				simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
				simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                simxReadVisionSensor(clientID, Middle_Vision_sensor, &readingMVS, &DataMVS, &auxMVS, simx_opmode_buffer);
                simxReadVisionSensor(clientID, RM_Vision_sensor, &readingRMVS, &DataRMVS, &auxRMVS, simx_opmode_buffer);
                readingMVS = DataMVS[10]<MAX_INTE;
                readingRMVS = DataRMVS[10]<MAX_INTE;
                extApi_sleepMs(5);
			}
			while(!(readingMVS))//gira ate os sensores captarem a linha
            {
                vLeft = VEL_MOT/1.5;
				vRight = -VEL_MOT/1.5;
				simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
				simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                simxReadVisionSensor(clientID, Middle_Vision_sensor, &readingMVS, &DataMVS, &auxMVS, simx_opmode_buffer);
                simxReadVisionSensor(clientID, RM_Vision_sensor, &readingRMVS, &DataRMVS, &auxRMVS, simx_opmode_buffer);
                readingMVS = DataMVS[10]<MAX_INTE;
                readingRMVS = DataRMVS[10]<MAX_INTE;
                extApi_sleepMs(5);
			}
            *(comando2.first) = 1;
*(comando1.first) = 0;		
}

        //virar esquerda
		else if(*(comando1.first) == 4)
        {
            while(d_ang < (M_PI/4-DIS_CURVA))//escapar do estado de verificacao dos sensores
            {
				vLeft = -VEL_MOT/1.5;
				vRight = VEL_MOT/1.5;
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
                extApi_sleepMs(5);
			}
            simxReadVisionSensor(clientID, LM_Vision_sensor, &readingLMVS, &DataLMVS, &auxLMVS, simx_opmode_buffer);
            simxReadVisionSensor(clientID, Middle_Vision_sensor, &readingMVS, &DataMVS, &auxMVS, simx_opmode_buffer);
            readingLMVS = DataLMVS[10]<MAX_INTE;
            readingMVS = DataMVS[10]<MAX_INTE;
            while((readingMVS))//verificar se ta no mesmo estado, tecnicamente inutil devido a funcao acima
            {
                vLeft = -VEL_MOT/1.5;
                vRight = VEL_MOT/1.5;
                simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                simxReadVisionSensor(clientID, LM_Vision_sensor, &readingLMVS, &DataLMVS, &auxLMVS, simx_opmode_buffer);
                simxReadVisionSensor(clientID, Middle_Vision_sensor, &readingMVS, &DataMVS, &auxMVS, simx_opmode_buffer);
                readingLMVS = DataLMVS[10]<MAX_INTE;
                readingMVS = DataMVS[10]<MAX_INTE;
                extApi_sleepMs(5);
            }
            while(!(readingMVS))//gira ate os sensores captarem a linha
            {
                vLeft = -VEL_MOT/1.5;
                vRight = VEL_MOT/1.5;
                simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                simxReadVisionSensor(clientID, LM_Vision_sensor, &readingLMVS, &DataLMVS, &auxLMVS, simx_opmode_buffer);
                simxReadVisionSensor(clientID, Middle_Vision_sensor, &readingMVS, &DataMVS, &auxMVS, simx_opmode_buffer);
                readingLMVS = DataLMVS[10]<MAX_INTE;
                readingMVS = DataMVS[10]<MAX_INTE;
                extApi_sleepMs(5);
			}
            *(comando2.first) = 1;
	*(comando1.first) = 0;
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
		simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
		simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
		extApi_sleepMs(5);

}
	
    simxFinish(clientID); // fechando conexao com o servidor
    cout << "Conexao e Servidor fechados!" << std::endl;
	if (*comando2.first != -5) *(comando2.first) = -10; //quando o programa fecha, seja por ter acabado sozinho com a simulação parando ou recebendo o comando -10, deve responder com -10 pra sinalizar que fechou. a unica exceção é quando acaba por não ter conseguido conectar, daí manda -5
	return 0;
 }
