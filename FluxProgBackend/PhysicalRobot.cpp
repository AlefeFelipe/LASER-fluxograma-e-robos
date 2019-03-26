#include "PhysicalRobot.hpp"

PhysicalRobot::PhysicalRobot(int *error)
{
    if(RS232_OpenComport(CPORT_NR, BDRATE, "8N1"))
    {
        printf("AVISO: NENHUMA serial encontrada!!\n");
        *error = 1;
    }
    else
    {
        RS232_enableDTR(CPORT_NR);
        RS232_enableRTS(CPORT_NR);
    }
}

PhysicalRobot::~PhysicalRobot()
{
    RS232_CloseComport(CPORT_NR);
}

void PhysicalRobot::setCommand(int _command)
{
    command = _command;
    if(command > 0)
    {
        sendCommand();
    }
}

void PhysicalRobot::sendCommand()
{
    RS232_flushRXTX(CPORT_NR);
    RS232_SendByte(CPORT_NR, (unsigned char)command);
}

void PhysicalRobot::updateSensorsReading()
{
    receiveData();
}

void PhysicalRobot::receiveData()
{
    //std::cout << "recebendo"<<std::endl;
    unsigned char* reading;
    RS232_flushRXTX(CPORT_NR);
    clock_t t = clock();
    int i, there_is_packet = 0;
    while(there_is_packet != SIZEPACKET && ((float)(clock() - t))/CLOCKS_PER_SEC < 20.0)
    {
        there_is_packet = RS232_PollComport(CPORT_NR, reading, SIZEPACKET);
    }
    if (there_is_packet == SIZEPACKET)
    {
        for (i = 0; i < N_BLACK_TAPE_SENSOR; i++)
        {
            black_type_sensor_reading[i] = reading[i];
            //std::cout <<black_type_sensor_reading[i]<<" ";
        }
        for (i = 0; i < N_ULTRASONIC; i++)
        {
            ultrasonic_sensor_reading[i] = *(unsigned short int*)&reading[2*i + 5];
            //std::cout <<ultrasonic_sensor_reading[i]<<" ";
        }
        std::cout<<std::endl;
        command = 1;
    }
    else
        command = 0;
}