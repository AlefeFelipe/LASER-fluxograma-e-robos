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
    if(command)
    {
        sendCommand();
        command = 1;//tirar depois
    }
}

void PhysicalRobot::sendCommand()
{
    while(1)
        RS232_SendByte(CPORT_NR, char(command));
}

void PhysicalRobot::updateSensorsReading()
{
    std::cout << "recebendo"<<std::endl;
    receiveData();
}

void PhysicalRobot::receiveData()
{
    unsigned char* reading;
    int i, there_is_packet = RS232_PollComport(CPORT_NR, reading, SIZEPACKET);
    if (there_is_packet == SIZEPACKET)
    {
        for (i = 0; i < N_BLACK_TAPE_SENSOR; i++)
        {
            black_type_sensor_reading[i] = reading[i];
        }
        for (i = 0; i < N_ULTRASONIC; i++)
        {
            ultrasonic_sensor_reading[i] = *(unsigned short int*)&reading[2*i + 5];
        }
        command = 1;
    }
    else
        command = 0;
}
