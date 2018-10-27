#include "Bluetooth.hpp"

Bluetooth::Bluetooth()
{
    packet = new char [PACKETSIZE];
    command = 0;
}

uint8_t Bluetooth::getCommand()
{
    if(Serial1.available())
    {
        command = Serial1.read();
        Serial.print("comando");
        Serial.println(command);
        return command;
    }
    return 0;
}

void Bluetooth::sendPacket(uint8_t* readingBTS, float* detectedObjet_U)
{
    //Serial.println(detectedObjet_U[4]);
    int i;
    char l_u[4];
    char lm_u[4];
    char m_u[4];
    char rm_u[4];
    char r_u[4];
    
    *((float*)&l_u) = detectedObjet_U[0];
    *((float*)&lm_u) = detectedObjet_U[1];
    *((float*)&m_u) = detectedObjet_U[2];
    *((float*)&rm_u) = detectedObjet_U[3];
    *((float*)&r_u) = detectedObjet_U[4];

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
    //for(i=0; i<PACKETSIZE; i++)
    //{
    //    Serial.print(packet[i], HEX);
    //}
    //Serial.println();
    delay(30);
    Serial1.write(packet, PACKETSIZE);
}
