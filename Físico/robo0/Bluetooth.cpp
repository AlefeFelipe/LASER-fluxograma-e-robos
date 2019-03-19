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

void Bluetooth::sendPacket(uint8_t *readingBTS, unsigned int *detectedObjet_U)
{
    unsigned int detectedObjet2_U[5];
    //Serial.println(detectedObjet_U[4]);
    int i;
    char l_u[2];
    char lm_u[2];
    char m_u[2];
    char rm_u[2];
    char r_u[2];
    
    *((unsigned int*)&l_u) = detectedObjet_U[0];
    *((unsigned int*)&lm_u) = detectedObjet_U[1];
    *((unsigned int*)&m_u) = detectedObjet_U[2];
    *((unsigned int*)&rm_u) = detectedObjet_U[3];
    *((unsigned int*)&r_u) = detectedObjet_U[4];

    for(i=0; i<N_BLACK_TAPE_SENSOR; i++)
    {
        packet[i] = readingBTS[i];
    }
    for(i=0; i<2; i++)
    {
        packet[i+5] = l_u[i];
        packet[i+7] = lm_u[i];
        packet[i+9] = m_u[i];
        packet[i+11] = rm_u[i];
        packet[i+13] = r_u[i];
    }
    //for(i=0; i<PACKETSIZE; i++)
    //{
    //   Serial.print(packet[i], HEX);
    //   Serial.print(" ");
    //}
    //Serial.println();
    delay(COMMUNICATION_DELAY);
    Serial1.write(packet, PACKETSIZE);
    //Serial1.flush();
}
