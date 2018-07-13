#include "Bluetooth.hpp"

Bluetooth::Bluetooth()
{
    packet = new char [PACKETSIZE];
}

uint8_t Bluetooth::getCommand()
{
    if(Serial1.available())
    {
        command = Serial1.read();
        //Serial.print("legal");
        return command;
    }
    return 0;
}

void Bluetooth::sendPacket(uint8_t* readingBTS, float* readingU)
{
    int i;
    char l_u[4];
    char lm_u[4];
    char m_u[4];
    char rm_u[4];
    char r_u[4];
    
    *((float*)&l_u) = readingU[0];
    *((float*)&lm_u) = readingU[1];
    *((float*)&m_u) = readingU[2];
    *((float*)&rm_u) = readingU[3];
    *((float*)&r_u) = readingU[4];

    for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
    {
        packet[i] = readingBTS[i];
    }
    for(i=0; i<4; i++)
    {
        packet[i+5] = l_u[i];
        packet[i+9] = lm_u[i];
        packet[i+13] = m_u[i];
        packet[i+17] = rm_u[i];
        packet[i+21] = r_u[i];
    }
    Serial1.write(packet, PACKETSIZE);
}
