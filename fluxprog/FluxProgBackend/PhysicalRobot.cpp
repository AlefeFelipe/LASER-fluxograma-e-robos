#include "PhysicalRobot.hpp"

PhysicalRobot::PhysicalRobot()
{
    if(RS232_OpenComport(CPORT_NR, BDRATE, "8N1"))
    {
        printf("AVISO: NENHUMA serial encontrada!!\n");
        command = -1;
    }
    else
    {
        RS232_enableDTR(CPORT_NR);
        RS232_enableRTS(CPORT_NR);
    }
    return 1;
}
