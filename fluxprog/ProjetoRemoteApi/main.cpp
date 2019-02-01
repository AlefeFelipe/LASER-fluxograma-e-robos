#include <cmath>
#include <iostream>
#include <time.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
using namespace std;
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/offset_ptr.hpp>
#include "rs232.h"
using namespace boost::interprocess;

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

//variaveis bluetooth
#define BDRATE 115200
#define CPORT_NR 16
#define SIZEPACKET 15
#define N_ULTRASONIC 5
#define N_BLACK_TAPE_SENSOR 5


extern "C" {
    #include "remoteApi/extApi.h"
    #include "remoteApi/extApiPlatform.h"


}

int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

/*void //maybeQuit(int *comando2, managed_shared_memory *abrindo_memoria);
{
    char close;
    if(kbhit())
    {
        cin>>close;
        if(close=='x')
        {
            shared_memory_object::remove(NOME_DA_MEMORIA2);
            delete abrindo_memoria;
            *(comando2) = -10;
            exit(0);
        }
    }
}*/
int setup()
{
    if(RS232_OpenComport(CPORT_NR, BDRATE, "8N1"))
    {
        printf("AVISO: NENHUMA serial encontrada!!\n");
        return 0;
    }
    RS232_enableDTR(CPORT_NR);
    RS232_enableRTS(CPORT_NR);
    return 1;

}

void sendCommand(char comando)
{
    stringstream buffer;
    string sendVariable;
    RS232_SendByte(CPORT_NR, comando);
    //RS232_SendBuf(CPORT_NR, (unsigned char*) comando, 1);
}

int receiver(unsigned char* reading, unsigned char* reading_VS, unsigned short int *detectedObjet_U)
{
    int i, there_is_packet = RS232_PollComport(CPORT_NR, reading, SIZEPACKET);
    if (there_is_packet == SIZEPACKET)
    {
        for (i = 0; i < N_BLACK_TAPE_SENSOR; i++)
        {
            reading_VS[i] = reading[i];
        }
        for (i = 0; i < N_ULTRASONIC; i++)
        {
            detectedObjet_U[i] = *(unsigned short int*)&reading[2*i + 5];
        }
    }
    else
        there_is_packet = 0;
    return there_is_packet;
}

void getReadingSensorsSimulator(int clientID, int bubbleRob, float *linPosition, float *angPosition, int Middle_ultrasonic, unsigned char *reading_U,
                                float *detectedObjetLU, float *detectedObjetLMU, float *detectedObjetMU, float *detectedObjetRMU, float *detectedObjetRU,
                                int detectedObjetHandleLU, int detectedObjetHandleLMU, int detectedObjetHandleMU, int detectedObjetHandleRMU, int detectedObjetHandleRU,
                                float *detectedSurfaceLU, float *detectedSurfaceLMU, float *detectedSurfaceMU, float *detectedSurfaceRMU, float *detectedSurfaceRU,
                                int Left_Vision_sensor, int LM_Vision_sensor, int Middle_Vision_sensor, int RM_Vision_sensor, int Right_Vision_sensor,
                                unsigned char *reading_VS, float **DataLVS, float **DataLMVS, float **DataMVS, float **DataRMVS, float **DataRVS,
                                int *auxLVS, int *auxLMVS, int *auxMVS, int *auxRMVS, int *auxRVS, bool first_time=false)
{
    if(!first_time)
    {
        simxGetObjectPosition(clientID, bubbleRob, -1, linPosition, simx_opmode_buffer);
        simxGetObjectOrientation(clientID, bubbleRob, -1, angPosition,simx_opmode_buffer);
        simxReadProximitySensor(clientID, Middle_ultrasonic, &reading_U[0], detectedObjetLU, &detectedObjetHandleLU, detectedSurfaceLU, simx_opmode_buffer);
        simxReadProximitySensor(clientID, Middle_ultrasonic, &reading_U[1], detectedObjetLMU, &detectedObjetHandleLMU, detectedSurfaceLMU, simx_opmode_buffer);
        simxReadProximitySensor(clientID, Middle_ultrasonic, &reading_U[2], detectedObjetMU, &detectedObjetHandleMU, detectedSurfaceMU, simx_opmode_buffer);
        simxReadProximitySensor(clientID, Middle_ultrasonic, &reading_U[3], detectedObjetRMU, &detectedObjetHandleRMU, detectedSurfaceRMU, simx_opmode_buffer);
        simxReadProximitySensor(clientID, Middle_ultrasonic, &reading_U[4], detectedObjetRU, &detectedObjetHandleRU, detectedSurfaceRU, simx_opmode_buffer);
        simxReadVisionSensor(clientID, Left_Vision_sensor, &reading_VS[0], DataLVS, &auxLVS, simx_opmode_buffer);
        simxReadVisionSensor(clientID, LM_Vision_sensor, &reading_VS[1], DataLMVS, &auxLMVS, simx_opmode_buffer);
        simxReadVisionSensor(clientID, Middle_Vision_sensor, &reading_VS[2], DataMVS, &auxMVS, simx_opmode_buffer);
        simxReadVisionSensor(clientID, RM_Vision_sensor, &reading_VS[3], DataRMVS, &auxRMVS, simx_opmode_buffer);
        simxReadVisionSensor(clientID, Right_Vision_sensor, &reading_VS[4], DataRVS, &auxRVS, simx_opmode_buffer);

    }
    else
    {
        simxGetObjectPosition(clientID, bubbleRob, -1, linPosition, simx_opmode_streaming);
        simxGetObjectOrientation(clientID, bubbleRob, -1, angPosition,simx_opmode_streaming);
        simxReadProximitySensor(clientID, Middle_ultrasonic, &reading_U[0], detectedObjetLU, &detectedObjetHandleLU, detectedSurfaceLU, simx_opmode_streaming);
        simxReadProximitySensor(clientID, Middle_ultrasonic, &reading_U[1], detectedObjetLMU, &detectedObjetHandleLMU, detectedSurfaceLMU, simx_opmode_streaming);
        simxReadProximitySensor(clientID, Middle_ultrasonic, &reading_U[2], detectedObjetMU, &detectedObjetHandleMU, detectedSurfaceMU, simx_opmode_streaming);
        simxReadProximitySensor(clientID, Middle_ultrasonic, &reading_U[3], detectedObjetRMU, &detectedObjetHandleRMU, detectedSurfaceRMU, simx_opmode_streaming);
        simxReadProximitySensor(clientID, Middle_ultrasonic, &reading_U[4], detectedObjetRU, &detectedObjetHandleRU, detectedSurfaceRU, simx_opmode_streaming);
        simxReadVisionSensor(clientID, Left_Vision_sensor, &reading_VS[0], DataLVS, &auxLVS, simx_opmode_streaming);
        simxReadVisionSensor(clientID, LM_Vision_sensor, &reading_VS[1], DataLMVS, &auxLMVS, simx_opmode_streaming);
        simxReadVisionSensor(clientID, Middle_Vision_sensor, &reading_VS[2], DataMVS, &auxMVS, simx_opmode_streaming);
        simxReadVisionSensor(clientID, RM_Vision_sensor, &reading_VS[3], DataRMVS, &auxRMVS, simx_opmode_streaming);
        simxReadVisionSensor(clientID, Right_Vision_sensor, &reading_VS[4], DataRVS, &auxRVS, simx_opmode_streaming);
    }
}


int main(int argc, char **argv)
{
    //enderecamentos remoteapi
    string serverIP = "127.0.0.1";
	int serverPort = 19999;
    int clientID=simxStart((simxChar*)serverIP.c_str(),serverPort,true,true,2000,5);
    unsigned char* reading_bluetooth;

    //inteiros para localizacao dos objetos no vrep
    //inteiros para partes moveis
    int leftMotorHandle = 0, rightMotorHandle = 0, bubbleRob=0, turnleft=0, turnright=0;

    //inteiros para sensores de visao
    int Left_Vision_sensor = 0, LM_Vision_sensor = 0, Middle_Vision_sensor = 0, RM_Vision_sensor = 0, Right_Vision_sensor = 0;

    //inteiros para sensores ultrassom e os objetos detectados
    int Left_ultrasonic = 0, LM_ultrasonic = 0, Middle_ultrasonic = 0, RM_ultrasonic = 0, Right_ultrasonic = 0;
    int detectedObjetHandleMU = 0, detectedObjetHandleLU = 0, detectedObjetHandleLMU = 0, detectedObjetHandleRMU = 0, detectedObjetHandleRU = 0;

    //variaveis para movimentacao do robo
    float vLeft = 0, vRight = 0, dx=0, dy=0, X_inicial=0, Y_inicial=0, d_ang=0, ang_inicial=0, ang=0;

    //vetores para captacao da posicao na cena
    float* angle = new float[3];
    //float* position = new float[3];

    //variaveis sensores ultrassom
    float* detectedObjetMU = new float[3];//coordenadas do objeto identificado (referencial do sensor). A última posicao possui o valor mais revelante
    float* detectedSurfaceMU = new float[3];//coordenadas da superficie identificada (referencial do sensor)
    //unsigned char reading_U[2] = 0;
    float* detectedObjetLU = new float[3];
    float* detectedSurfaceLU = new float[3];
    float* detectedObjetLMU = new float[3];
    float* detectedSurfaceLMU = new float[3];
    float* detectedObjetRMU = new float[3];
    float* detectedSurfaceRMU = new float[3];
    float* detectedObjetRU = new float[3];
    float* detectedSurfaceRU = new float[3];

    //variaveis sensores de visao
    //unsigned char reading_VS[0] = 0;//leitura do sensor de visao
    float* DataLVS = new float [15];//dados da leitura
    int* auxLVS;//vetor auxiliar para casos especificos
    //unsigned char reading_VS[1] = 0;
    float* DataLMVS = new float [15];
    int* auxLMVS;
    //unsigned char reading_VS[2] = 0;
    float* DataMVS = new float [15];
    int* auxMVS;
    //unsigned char reading_VS[3] = 0;
    float* DataRMVS = new float [15];
    int* auxRMVS;
    //unsigned char reading_VS[4] = 0;
    float* DataRVS = new float [15];
    int* auxRVS;

  ///Criação de memória
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

    unsigned char* reading_VS;//vetores para captacao da leitura dos sensores na memoria
    unsigned char* reading_U;
    unsigned short int*  detectedObjet_U;//vetor para captacao da posicao dos objetos captados
    float* linPosition;//vetores para captacao da localizacao linear e angular do robo
    float* angPosition;
    reading_VS = abrindo_memoria->construct<unsigned char>(SENSOR_VISAO)[N_BLACK_TAPE_SENSOR]();
    reading_U = abrindo_memoria->construct<unsigned char>(SENSOR_ULTRASSOM)[N_ULTRASONIC]();
    detectedObjet_U = abrindo_memoria->construct<unsigned short int >(POSICAO_DETECTADA)[N_ULTRASONIC]();
    linPosition = abrindo_memoria->construct<float>(POSICAO)[3]();
    angPosition = abrindo_memoria->construct<float>(ANGULAR)[3]();
    ///Finalização


    pair<int*, managed_shared_memory::size_type> comando1;
    pair<int*, managed_shared_memory::size_type> bluetooth;
    pair<int*, managed_shared_memory::size_type> comando2;
    comando1 = abrindo_memoria->find<int>(NOME_DO_INT_NA_MEMORIA1);
	comando2 = abrindo_memoria->find<int>(NOME_DO_INT_NA_MEMORIA2);
    bluetooth = abrindo_memoria->find<int>(BLUETOOTH_ENABLE);

    ///Fechamento
    if(*(bluetooth.first))
    {
        if(!setup())
        {
            *comando2.first = -5;
            return 0;
        }
        *comando2.first = -4;
        while(*(bluetooth.first))
        {
            comando1 = abrindo_memoria->find<int>(NOME_DO_INT_NA_MEMORIA1);
            //sendCommand(char(*(comando1.first)+'0'));
            if(*(comando1.first))
            {
                sendCommand(char(*(comando1.first)));
                cout<<"mandei o comando "<<(*(comando1.first))<<endl;
                *(comando1.first) = 0;
            }
            //usleep(10);
            int a = receiver(reading_bluetooth, reading_VS, detectedObjet_U);
            if(a)
            {
                *(comando2.first) = 1;//por enquanto o robo envia coisas quando ele terminou de fazer as paradas
                for (i = 0; i < N_BLACK_TAPE_SENSOR; i++)
                {
                    cout<<"sensor fita " << i << " :" << int(reading_VS[i]) << endl;
                }
                for (i = 0; i < N_ULTRASONIC; i++)
                {
                    cout<<"ultrassom " << i << " :" << detectedObjet_U[i] << endl;
                }
            }
        }
    }


    ///conexao com elementos da cena
	if (clientID!=-1)
	{
        bool tudo_ok = true;
		cout << "Servidor conectado!" << std::endl;

        // inicialização dos motores e do robo
        if(simxGetObjectHandle(clientID,"bubbleRob",&bubbleRob, simx_opmode_oneshot_wait)==simx_return_ok)
                cout << "conectado ao bubbleRob" <<endl;
        if(simxGetObjectHandle(clientID, "Left_ultrasonic", &Left_ultrasonic, simx_opmode_oneshot_wait )==simx_return_ok&&
            simxGetObjectHandle(clientID, "LM_ultrasonic", &LM_ultrasonic, simx_opmode_oneshot_wait )==simx_return_ok&&
            simxGetObjectHandle(clientID, "Middle_ultrasonic", &Middle_ultrasonic, simx_opmode_oneshot_wait )==simx_return_ok&&
            simxGetObjectHandle(clientID, "RM_ultrasonic", &RM_ultrasonic, simx_opmode_oneshot_wait )==simx_return_ok&&
            simxGetObjectHandle(clientID, "Right_ultrasonic", &Right_ultrasonic, simx_opmode_oneshot_wait )==simx_return_ok)
                cout << "conectado aos sensores ultrassom" <<endl;
        if(simxGetObjectHandle(clientID, "Left_Vision_sensor", &Left_Vision_sensor, simx_opmode_oneshot_wait)==simx_return_ok&&
            simxGetObjectHandle(clientID, "LM_Vision_sensor", &LM_Vision_sensor, simx_opmode_oneshot_wait)==simx_return_ok&&
            simxGetObjectHandle(clientID, "Middle_Vision_sensor", &Middle_Vision_sensor, simx_opmode_oneshot_wait)==simx_return_ok&&
            simxGetObjectHandle(clientID, "RM_Vision_sensor", &RM_Vision_sensor, simx_opmode_oneshot_wait)==simx_return_ok&&
            simxGetObjectHandle(clientID, "Right_Vision_sensor", &Right_Vision_sensor, simx_opmode_oneshot_wait)==simx_return_ok
          )
                cout << "conectado aos sensores de visao" <<endl;

		if(simxGetObjectHandle(clientID,(const simxChar*) "bubbleRob_leftMotor",(simxInt *) &leftMotorHandle, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
        {
            cout << "Handle do motor esquerdo nao encontrado!" << std::endl;
            tudo_ok = false;
        }

		else
			cout << "Conectado ao motor esquerdo!" << std::endl;

		if(simxGetObjectHandle(clientID,(const simxChar*) "bubbleRob_rightMotor",(simxInt *) &rightMotorHandle, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
        {
            cout << "Handle do motor direito nao encontrado!" << std::endl;
            tudo_ok = false;
        }

		else
			cout << "Conectado ao motor direito!" << std::endl;
        getReadingSensorsSimulator(clientID, bubbleRob, linPosition, angPosition, Middle_ultrasonic, reading_U,
                                    detectedObjetLU, detectedObjetLMU, detectedObjetMU, detectedObjetRMU, detectedObjetRU,
                                    detectedObjetHandleLU, detectedObjetHandleLMU, detectedObjetHandleMU, detectedObjetHandleRMU, detectedObjetHandleRU,
                                    detectedSurfaceLU, detectedSurfaceLMU, detectedSurfaceMU, detectedSurfaceRMU, detectedSurfaceRU,
                                    Left_Vision_sensor, LM_Vision_sensor, Middle_Vision_sensor, RM_Vision_sensor, Right_Vision_sensor,
                                    reading_VS, &DataLVS, &DataLMVS, &DataMVS, &DataRMVS, &DataRVS,
                                    auxLVS, auxLMVS, auxMVS, auxRMVS, auxRVS, true);

        if (!tudo_ok)
        {
            *(comando2.first) = -5; // se a conexão com o vrep falha o programa manda -5 para indicara falha
        }
        else
        {
    		*(comando2.first) = -4; // se a conexão der certo manda -4
        }
	}
	else
    {
        *(comando2.first) = -5;
    }
    comando1 = abrindo_memoria->find<int>(NOME_DO_INT_NA_MEMORIA1);
    //loop de execucao
	while(simxGetConnectionId(clientID)!=-1 && *(comando1.first) != -10)
    {
        getReadingSensorsSimulator(clientID, bubbleRob, linPosition, angPosition, Middle_ultrasonic, reading_U,
                                    detectedObjetLU, detectedObjetLMU, detectedObjetMU, detectedObjetRMU, detectedObjetRU,
                                    detectedObjetHandleLU, detectedObjetHandleLMU, detectedObjetHandleMU, detectedObjetHandleRMU, detectedObjetHandleRU,
                                    detectedSurfaceLU, detectedSurfaceLMU, detectedSurfaceMU, detectedSurfaceRMU, detectedSurfaceRU,
                                    Left_Vision_sensor, LM_Vision_sensor, Middle_Vision_sensor, RM_Vision_sensor, Right_Vision_sensor,
                                    reading_VS, &DataLVS, &DataLMVS, &DataMVS, &DataRMVS, &DataRVS,
                                    auxLVS, auxLMVS, auxMVS, auxRMVS, auxRVS);
        reading_VS[0] = DataLVS[10]<MAX_INTE;//media da intensidade. No caso preto, sao todos 0
        reading_VS[1] = DataLMVS[10]<MAX_INTE;
        reading_VS[2] = DataMVS[10]<MAX_INTE;
        reading_VS[3] = DataRMVS[10]<MAX_INTE;
        reading_VS[4] = DataRVS[10]<MAX_INTE;
        detectedObjet_U[0] = detectedObjetLU[2];
        detectedObjet_U[1] = detectedObjetLMU[2];
        detectedObjet_U[2] = detectedObjetMU[2];
        detectedObjet_U[3] = detectedObjetRMU[2];
        detectedObjet_U[4] = detectedSurfaceRU[2];

        //linha reta (se nao estiver perto de um obstaculo)
        if (*(comando1.first) == 8 && reading_U[2]==0)
        {
            while(((reading_VS[0]&&reading_VS[2])||(reading_VS[2]&&reading_VS[4])) && reading_U[2]==0)//verificar se ta no mesmo estado, tecnicamente inutil devido a funcao acima
            {
                vLeft = VEL_MOT;
                vRight = VEL_MOT;
                simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                getReadingSensorsSimulator(clientID, bubbleRob, linPosition, angPosition, Middle_ultrasonic, reading_U,
                                            detectedObjetLU, detectedObjetLMU, detectedObjetMU, detectedObjetRMU, detectedObjetRU,
                                            detectedObjetHandleLU, detectedObjetHandleLMU, detectedObjetHandleMU, detectedObjetHandleRMU, detectedObjetHandleRU,
                                            detectedSurfaceLU, detectedSurfaceLMU, detectedSurfaceMU, detectedSurfaceRMU, detectedSurfaceRU,
                                            Left_Vision_sensor, LM_Vision_sensor, Middle_Vision_sensor, RM_Vision_sensor, Right_Vision_sensor,
                                            reading_VS, &DataLVS, &DataLMVS, &DataMVS, &DataRMVS, &DataRVS,
                                            auxLVS, auxLMVS, auxMVS, auxRMVS, auxRVS);
                reading_VS[0] = DataLVS[10]<MAX_INTE;//media da intensidade. No caso preto, sao todos 0
                reading_VS[2] = DataMVS[10]<MAX_INTE;
                reading_VS[4] = DataRVS[10]<MAX_INTE;
                extApi_sleepMs(5);
                if(reading_U[2])
                {
                    cout<<"objeto "<<detectedObjetHandleMU<<" na posicao "<<detectedObjetMU[0]<<", "<<detectedObjetMU[1]<<", "<<detectedObjetMU[2]<<endl;
                    cout<<"superficie em "<<detectedSurfaceMU[0]<<", "<<detectedSurfaceMU[1]<<", "<<detectedSurfaceMU[2]<<endl;
                }
                //maybeQuit(comando2.first, abrindo_memoria);
            }
            while(!((reading_VS[0]&&reading_VS[2])||(reading_VS[2]&&reading_VS[4])) && reading_U[2]==0)//anda ate os sensores captarem a linha ou obstaculo
            {
                vLeft = VEL_MOT;
                vRight = VEL_MOT;
                simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                getReadingSensorsSimulator(clientID, bubbleRob, linPosition, angPosition, Middle_ultrasonic, reading_U,
                                            detectedObjetLU, detectedObjetLMU, detectedObjetMU, detectedObjetRMU, detectedObjetRU,
                                            detectedObjetHandleLU, detectedObjetHandleLMU, detectedObjetHandleMU, detectedObjetHandleRMU, detectedObjetHandleRU,
                                            detectedSurfaceLU, detectedSurfaceLMU, detectedSurfaceMU, detectedSurfaceRMU, detectedSurfaceRU,
                                            Left_Vision_sensor, LM_Vision_sensor, Middle_Vision_sensor, RM_Vision_sensor, Right_Vision_sensor,
                                            reading_VS, &DataLVS, &DataLMVS, &DataMVS, &DataRMVS, &DataRVS,
                                            auxLVS, auxLMVS, auxMVS, auxRMVS, auxRVS);
                reading_VS[0] = DataLVS[10]<MAX_INTE;//media da intensidade. No caso preto, sao todos 0
                reading_VS[1] = DataLMVS[10]<MAX_INTE;
                reading_VS[2] = DataMVS[10]<MAX_INTE;
                reading_VS[3] = DataRMVS[10]<MAX_INTE;
                reading_VS[4] = DataRVS[10]<MAX_INTE;
                //if(!reading_VS[2]&&(reading_VS[3]||reading_VS[4]))//robo esta desviando para a esquerda, gira direita
                if(reading_VS[4])
                {
                    vLeft = VEL_MOT;
                    vRight = VEL_MOT/4;
                    simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                    simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                    getReadingSensorsSimulator(clientID, bubbleRob, linPosition, angPosition, Middle_ultrasonic, reading_U,
                                                detectedObjetLU, detectedObjetLMU, detectedObjetMU, detectedObjetRMU, detectedObjetRU,
                                                detectedObjetHandleLU, detectedObjetHandleLMU, detectedObjetHandleMU, detectedObjetHandleRMU, detectedObjetHandleRU,
                                                detectedSurfaceLU, detectedSurfaceLMU, detectedSurfaceMU, detectedSurfaceRMU, detectedSurfaceRU,
                                                Left_Vision_sensor, LM_Vision_sensor, Middle_Vision_sensor, RM_Vision_sensor, Right_Vision_sensor,
                                                reading_VS, &DataLVS, &DataLMVS, &DataMVS, &DataRMVS, &DataRVS,
                                                auxLVS, auxLMVS, auxMVS, auxRMVS, auxRVS);
                    reading_VS[0] = DataLVS[10]<MAX_INTE;//media da intensidade. No caso preto, sao todos 0
                    reading_VS[2] = DataMVS[10]<MAX_INTE;
                    reading_VS[4] = DataRVS[10]<MAX_INTE;
                    turnright = 1;
                    turnleft = 0;
                }
                else if(reading_VS[3])
                {
                    vLeft = VEL_MOT;
                    vRight = VEL_MOT/2;
                    simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                    simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                    getReadingSensorsSimulator(clientID, bubbleRob, linPosition, angPosition, Middle_ultrasonic, reading_U,
                                                detectedObjetLU, detectedObjetLMU, detectedObjetMU, detectedObjetRMU, detectedObjetRU,
                                                detectedObjetHandleLU, detectedObjetHandleLMU, detectedObjetHandleMU, detectedObjetHandleRMU, detectedObjetHandleRU,
                                                detectedSurfaceLU, detectedSurfaceLMU, detectedSurfaceMU, detectedSurfaceRMU, detectedSurfaceRU,
                                                Left_Vision_sensor, LM_Vision_sensor, Middle_Vision_sensor, RM_Vision_sensor, Right_Vision_sensor,
                                                reading_VS, &DataLVS, &DataLMVS, &DataMVS, &DataRMVS, &DataRVS,
                                                auxLVS, auxLMVS, auxMVS, auxRMVS, auxRVS);
                    reading_VS[0] = DataLVS[10]<MAX_INTE;//media da intensidade. No caso preto, sao todos 0
                    reading_VS[2] = DataMVS[10]<MAX_INTE;
                    reading_VS[4] = DataRVS[10]<MAX_INTE;
                    turnright = 1;
                    turnleft = 0;
                }
                else if(reading_VS[1])//robo ta desviando para a direita, gira esquerda
                {
                    vLeft = VEL_MOT/2;
                    vRight = VEL_MOT;
                    simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                    simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                    getReadingSensorsSimulator(clientID, bubbleRob, linPosition, angPosition, Middle_ultrasonic, reading_U,
                                                detectedObjetLU, detectedObjetLMU, detectedObjetMU, detectedObjetRMU, detectedObjetRU,
                                                detectedObjetHandleLU, detectedObjetHandleLMU, detectedObjetHandleMU, detectedObjetHandleRMU, detectedObjetHandleRU,
                                                detectedSurfaceLU, detectedSurfaceLMU, detectedSurfaceMU, detectedSurfaceRMU, detectedSurfaceRU,
                                                Left_Vision_sensor, LM_Vision_sensor, Middle_Vision_sensor, RM_Vision_sensor, Right_Vision_sensor,
                                                reading_VS, &DataLVS, &DataLMVS, &DataMVS, &DataRMVS, &DataRVS,
                                                auxLVS, auxLMVS, auxMVS, auxRMVS, auxRVS);
                    reading_VS[0] = DataLVS[10]<MAX_INTE;//media da intensidade. No caso preto, sao todos 0
                    reading_VS[2] = DataMVS[10]<MAX_INTE;
                    reading_VS[4] = DataRVS[10]<MAX_INTE;
                    turnright = 0;
                    turnleft = 1;
                }
                else if(reading_VS[0])//robo ta desviando para a direita, gira esquerda
                {
                    vLeft = VEL_MOT/4;
                    vRight = VEL_MOT;
                    simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                    simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                    getReadingSensorsSimulator(clientID, bubbleRob, linPosition, angPosition, Middle_ultrasonic, reading_U,
                                                detectedObjetLU, detectedObjetLMU, detectedObjetMU, detectedObjetRMU, detectedObjetRU,
                                                detectedObjetHandleLU, detectedObjetHandleLMU, detectedObjetHandleMU, detectedObjetHandleRMU, detectedObjetHandleRU,
                                                detectedSurfaceLU, detectedSurfaceLMU, detectedSurfaceMU, detectedSurfaceRMU, detectedSurfaceRU,
                                                Left_Vision_sensor, LM_Vision_sensor, Middle_Vision_sensor, RM_Vision_sensor, Right_Vision_sensor,
                                                reading_VS, &DataLVS, &DataLMVS, &DataMVS, &DataRMVS, &DataRVS,
                                                auxLVS, auxLMVS, auxMVS, auxRMVS, auxRVS);
                    reading_VS[0] = DataLVS[10]<MAX_INTE;//media da intensidade. No caso preto, sao todos 0
                    reading_VS[2] = DataMVS[10]<MAX_INTE;
                    reading_VS[4] = DataRVS[10]<MAX_INTE;
                    turnright = 0;
                    turnleft = 1;
                }
                else if(!(reading_VS[0]||reading_VS[1]||reading_VS[2]||reading_VS[3]||reading_VS[3]))
                {
                    if(turnright)
                    {
                        vLeft = VEL_MOT;
                        vRight = VEL_MOT/4;
                        simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                        simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                        getReadingSensorsSimulator(clientID, bubbleRob, linPosition, angPosition, Middle_ultrasonic, reading_U,
                                                    detectedObjetLU, detectedObjetLMU, detectedObjetMU, detectedObjetRMU, detectedObjetRU,
                                                    detectedObjetHandleLU, detectedObjetHandleLMU, detectedObjetHandleMU, detectedObjetHandleRMU, detectedObjetHandleRU,
                                                    detectedSurfaceLU, detectedSurfaceLMU, detectedSurfaceMU, detectedSurfaceRMU, detectedSurfaceRU,
                                                    Left_Vision_sensor, LM_Vision_sensor, Middle_Vision_sensor, RM_Vision_sensor, Right_Vision_sensor,
                                                    reading_VS, &DataLVS, &DataLMVS, &DataMVS, &DataRMVS, &DataRVS,
                                                    auxLVS, auxLMVS, auxMVS, auxRMVS, auxRVS);
                        reading_VS[0] = DataLVS[10]<MAX_INTE;//media da intensidade. No caso preto, sao todos 0
                        reading_VS[2] = DataMVS[10]<MAX_INTE;
                        reading_VS[4] = DataRVS[10]<MAX_INTE;
                    }
                    else if(turnleft)
                    {
                        vLeft = VEL_MOT/4;
                        vRight = VEL_MOT;
                        simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                        simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                        getReadingSensorsSimulator(clientID, bubbleRob, linPosition, angPosition, Middle_ultrasonic, reading_U,
                                                    detectedObjetLU, detectedObjetLMU, detectedObjetMU, detectedObjetRMU, detectedObjetRU,
                                                    detectedObjetHandleLU, detectedObjetHandleLMU, detectedObjetHandleMU, detectedObjetHandleRMU, detectedObjetHandleRU,
                                                    detectedSurfaceLU, detectedSurfaceLMU, detectedSurfaceMU, detectedSurfaceRMU, detectedSurfaceRU,
                                                    Left_Vision_sensor, LM_Vision_sensor, Middle_Vision_sensor, RM_Vision_sensor, Right_Vision_sensor,
                                                    reading_VS, &DataLVS, &DataLMVS, &DataMVS, &DataRMVS, &DataRVS,
                                                    auxLVS, auxLMVS, auxMVS, auxRMVS, auxRVS);
                        reading_VS[0] = DataLVS[10]<MAX_INTE;//media da intensidade. No caso preto, sao todos 0
                        reading_VS[2] = DataMVS[10]<MAX_INTE;
                        reading_VS[4] = DataRVS[10]<MAX_INTE;
                    }
                }
                if(reading_U[2])
                {
                    cout<<"objeto "<<detectedObjetHandleMU<<" na posicao "<<detectedObjetMU[0]<<", "<<detectedObjetMU[1]<<", "<<detectedObjetMU[2]<<endl;
                    cout<<"superficie em "<<detectedSurfaceMU[0]<<", "<<detectedSurfaceMU[1]<<", "<<detectedSurfaceMU[2]<<endl;
                }
                //maybeQuit(comando2.first, abrindo_memoria);
                extApi_sleepMs(5);
            }
            simxGetObjectPosition(clientID, bubbleRob, -1, linPosition, simx_opmode_buffer);
            X_inicial = linPosition[0];
            Y_inicial = linPosition[1];
            dx=0;
            dy=0;
            while((dx < DIS_RETO && dy <DIS_RETO) && reading_U[2]==0)//chegar na linha
            {
                vLeft = VEL_MOT;
                vRight = VEL_MOT;
                simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                getReadingSensorsSimulator(clientID, bubbleRob, linPosition, angPosition, Middle_ultrasonic, reading_U,
                                            detectedObjetLU, detectedObjetLMU, detectedObjetMU, detectedObjetRMU, detectedObjetRU,
                                            detectedObjetHandleLU, detectedObjetHandleLMU, detectedObjetHandleMU, detectedObjetHandleRMU, detectedObjetHandleRU,
                                            detectedSurfaceLU, detectedSurfaceLMU, detectedSurfaceMU, detectedSurfaceRMU, detectedSurfaceRU,
                                            Left_Vision_sensor, LM_Vision_sensor, Middle_Vision_sensor, RM_Vision_sensor, Right_Vision_sensor,
                                            reading_VS, &DataLVS, &DataLMVS, &DataMVS, &DataRMVS, &DataRVS,
                                            auxLVS, auxLMVS, auxMVS, auxRMVS, auxRVS);
                dx = abs(linPosition[0]-X_inicial);
                dy = abs(linPosition[1]-Y_inicial);
                extApi_sleepMs(5);
                if(reading_U[2])
                {
                    cout<<"objeto "<<detectedObjetHandleMU<<" na posicao "<<detectedObjetMU[0]<<", "<<detectedObjetMU[1]<<", "<<detectedObjetMU[2]<<endl;
                    cout<<"superficie em "<<detectedSurfaceMU[0]<<", "<<detectedSurfaceMU[1]<<", "<<detectedSurfaceMU[2]<<endl;
                }
                //maybeQuit(comando2.first, abrindo_memoria);
            }
            *comando2.first = 1;
            *comando1.first = 0;
            //maybeQuit(comando2.first, abrindo_memoria);
		}

        //virar direita
        else if(*(comando1.first) == 6)
        {
            simxGetObjectOrientation(clientID, bubbleRob, -1, angle, simx_opmode_buffer);
            ang_inicial = (angle[2]);
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
                //maybeQuit(comando2.first, abrindo_memoria);
            }
            getReadingSensorsSimulator(clientID, bubbleRob, linPosition, angPosition, Middle_ultrasonic, reading_U,
                                        detectedObjetLU, detectedObjetLMU, detectedObjetMU, detectedObjetRMU, detectedObjetRU,
                                        detectedObjetHandleLU, detectedObjetHandleLMU, detectedObjetHandleMU, detectedObjetHandleRMU, detectedObjetHandleRU,
                                        detectedSurfaceLU, detectedSurfaceLMU, detectedSurfaceMU, detectedSurfaceRMU, detectedSurfaceRU,
                                        Left_Vision_sensor, LM_Vision_sensor, Middle_Vision_sensor, RM_Vision_sensor, Right_Vision_sensor,
                                        reading_VS, &DataLVS, &DataLMVS, &DataMVS, &DataRMVS, &DataRVS,
                                        auxLVS, auxLMVS, auxMVS, auxRMVS, auxRVS);
            reading_VS[2] = DataMVS[10]<MAX_INTE;
            reading_VS[3] = DataRMVS[10]<MAX_INTE;
            while(reading_VS[2])//verificar se ta no mesmo estado, tecnicamente inutil devido a funcao acima
            {
                vLeft = VEL_MOT/1.5;
                vRight = -VEL_MOT/1.5;
                simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                getReadingSensorsSimulator(clientID, bubbleRob, linPosition, angPosition, Middle_ultrasonic, reading_U,
                                            detectedObjetLU, detectedObjetLMU, detectedObjetMU, detectedObjetRMU, detectedObjetRU,
                                            detectedObjetHandleLU, detectedObjetHandleLMU, detectedObjetHandleMU, detectedObjetHandleRMU, detectedObjetHandleRU,
                                            detectedSurfaceLU, detectedSurfaceLMU, detectedSurfaceMU, detectedSurfaceRMU, detectedSurfaceRU,
                                            Left_Vision_sensor, LM_Vision_sensor, Middle_Vision_sensor, RM_Vision_sensor, Right_Vision_sensor,
                                            reading_VS, &DataLVS, &DataLMVS, &DataMVS, &DataRMVS, &DataRVS,
                                            auxLVS, auxLMVS, auxMVS, auxRMVS, auxRVS);
                reading_VS[2] = DataMVS[10]<MAX_INTE;
                reading_VS[3] = DataRMVS[10]<MAX_INTE;
                extApi_sleepMs(5);
                //maybeQuit(comando2.first, abrindo_memoria);
            }
            while(!(reading_VS[2]))//gira ate os sensores captarem a linha
            {
                vLeft = VEL_MOT/1.5;
                vRight = -VEL_MOT/1.5;
                simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                getReadingSensorsSimulator(clientID, bubbleRob, linPosition, angPosition, Middle_ultrasonic, reading_U,
                                            detectedObjetLU, detectedObjetLMU, detectedObjetMU, detectedObjetRMU, detectedObjetRU,
                                            detectedObjetHandleLU, detectedObjetHandleLMU, detectedObjetHandleMU, detectedObjetHandleRMU, detectedObjetHandleRU,
                                            detectedSurfaceLU, detectedSurfaceLMU, detectedSurfaceMU, detectedSurfaceRMU, detectedSurfaceRU,
                                            Left_Vision_sensor, LM_Vision_sensor, Middle_Vision_sensor, RM_Vision_sensor, Right_Vision_sensor,
                                            reading_VS, &DataLVS, &DataLMVS, &DataMVS, &DataRMVS, &DataRVS,
                                            auxLVS, auxLMVS, auxMVS, auxRMVS, auxRVS);
                reading_VS[2] = DataMVS[10]<MAX_INTE;
                reading_VS[3] = DataRMVS[10]<MAX_INTE;
                extApi_sleepMs(5);
                //maybeQuit(comando2.first, abrindo_memoria);
            }
            *comando2.first = 1;
            *comando1.first = 0;
        }

        //virar esquerda
        else if(*(comando1.first) == 4)
        {
            simxGetObjectOrientation(clientID, bubbleRob, -1, angle, simx_opmode_buffer);
            ang_inicial = (angle[2]);
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
                //maybeQuit(comando2.first, abrindo_memoria);
			}
            getReadingSensorsSimulator(clientID, bubbleRob, linPosition, angPosition, Middle_ultrasonic, reading_U,
                                        detectedObjetLU, detectedObjetLMU, detectedObjetMU, detectedObjetRMU, detectedObjetRU,
                                        detectedObjetHandleLU, detectedObjetHandleLMU, detectedObjetHandleMU, detectedObjetHandleRMU, detectedObjetHandleRU,
                                        detectedSurfaceLU, detectedSurfaceLMU, detectedSurfaceMU, detectedSurfaceRMU, detectedSurfaceRU,
                                        Left_Vision_sensor, LM_Vision_sensor, Middle_Vision_sensor, RM_Vision_sensor, Right_Vision_sensor,
                                        reading_VS, &DataLVS, &DataLMVS, &DataMVS, &DataRMVS, &DataRVS,
                                        auxLVS, auxLMVS, auxMVS, auxRMVS, auxRVS);
            reading_VS[1] = DataLMVS[10]<MAX_INTE;
            reading_VS[2] = DataMVS[10]<MAX_INTE;
            while((reading_VS[2]))//verificar se ta no mesmo estado, tecnicamente inutil devido a funcao acima
            {
                vLeft = -VEL_MOT/1.5;
                vRight = VEL_MOT/1.5;
                simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                getReadingSensorsSimulator(clientID, bubbleRob, linPosition, angPosition, Middle_ultrasonic, reading_U,
                                            detectedObjetLU, detectedObjetLMU, detectedObjetMU, detectedObjetRMU, detectedObjetRU,
                                            detectedObjetHandleLU, detectedObjetHandleLMU, detectedObjetHandleMU, detectedObjetHandleRMU, detectedObjetHandleRU,
                                            detectedSurfaceLU, detectedSurfaceLMU, detectedSurfaceMU, detectedSurfaceRMU, detectedSurfaceRU,
                                            Left_Vision_sensor, LM_Vision_sensor, Middle_Vision_sensor, RM_Vision_sensor, Right_Vision_sensor,
                                            reading_VS, &DataLVS, &DataLMVS, &DataMVS, &DataRMVS, &DataRVS,
                                            auxLVS, auxLMVS, auxMVS, auxRMVS, auxRVS);
                reading_VS[1] = DataLMVS[10]<MAX_INTE;
                reading_VS[2] = DataMVS[10]<MAX_INTE;
                //maybeQuit(comando2.first, abrindo_memoria);
                extApi_sleepMs(5);
            }
            while(!(reading_VS[2]))//gira ate os sensores captarem a linha
            {
                vLeft = -VEL_MOT/1.5;
                vRight = VEL_MOT/1.5;
                simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
                simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
                getReadingSensorsSimulator(clientID, bubbleRob, linPosition, angPosition, Middle_ultrasonic, reading_U,
                                            detectedObjetLU, detectedObjetLMU, detectedObjetMU, detectedObjetRMU, detectedObjetRU,
                                            detectedObjetHandleLU, detectedObjetHandleLMU, detectedObjetHandleMU, detectedObjetHandleRMU, detectedObjetHandleRU,
                                            detectedSurfaceLU, detectedSurfaceLMU, detectedSurfaceMU, detectedSurfaceRMU, detectedSurfaceRU,
                                            Left_Vision_sensor, LM_Vision_sensor, Middle_Vision_sensor, RM_Vision_sensor, Right_Vision_sensor,
                                            reading_VS, &DataLVS, &DataLMVS, &DataMVS, &DataRMVS, &DataRVS,
                                            auxLVS, auxLMVS, auxMVS, auxRMVS, auxRVS);
                reading_VS[1] = DataLMVS[10]<MAX_INTE;
                reading_VS[2] = DataMVS[10]<MAX_INTE;
                //maybeQuit(comando2.first, abrindo_memoria);
                extApi_sleepMs(5);
			}
            *comando2.first = 1;
            *comando1.first = 0;
		}

        //zera variaveis e manda o robo parar
        X_inicial = 0;
        Y_inicial = 0;
        dx = 0;
        dy = 0;
        d_ang = 0;
        ang_inicial = 0;
        ang = 0;
		vLeft = 0;
		vRight = 0;
		simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
		simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
		extApi_sleepMs(5);
        //maybeQuit(comando2.first, abrindo_memoria);
	}

    simxFinish(clientID); // fechando conexao com o servidor
    cout << "Conexao e Servidor fechados!" << std::endl;
    if (*comando2.first != -5)
        *(comando2.first) = -10; //quando o programa fecha, seja por ter acabado sozinho com a simulação parando ou recebendo o comando -10, deve responder com -10 pra sinalizar que fechou. a unica exceção é quando acaba por não ter conseguido conectar, daí manda -5
	return 0;
 }
