#include "Communication.hpp"

Communication::Communication()
{
    bool opened = false;
    while(!opened) {

        if(shared_memory!=NULL) //deletes the last opened vision shared memory if it exists
        {
            cout<<"qqr cosia"<<endl;
            delete shared_memory;
            shared_memory_object::remove(NOME_DA_MEMORIA);
        }
        try //tenta criar a memória
        {
            shared_memory = new managed_shared_memory (create_only, NOME_DA_MEMORIA, TAMANHO_DA_MEMORIA);
            opened = true;
        }
        catch(...)
        {
            shared_memory_object::remove(NOME_DA_MEMORIA);
            cout<<"erro ao criar memoria" << endl;
        }
    }
    //color_sensor_reading = shared_memory->find'<unsigned short int >(POSICAO_DETECTADA).first;
    command = shared_memory->construct<int>(NOME_DO_INT_NA_MEMORIA1)();
    virtual_robot = shared_memory->construct<int>(BLUETOOTH_ENABLE)();
    *command = 0;
    *virtual_robot = 2;
}

Communication::~Communication()
{
    if(shared_memory != NULL)
    {
        delete shared_memory;
    }
    shared_memory_object::remove(NOME_DA_MEMORIA);
}

void Communication::setCommand(int c)
{
    *command = c;
}

void Communication::setIfVirtual(int i)
{
    *virtual_robot = i;
}

int Communication::getFeedback()
{
    int oi = 0;
    try
    {
        oi = *feedback.first;
    }
    catch(...)
    {
        cout<<"nao fuinc"<<endl;
    }
    return oi;
}

int* Communication::getUltrasonicReading()
{
    return ultrasonic_sensor_reading.first;
}

int* Communication::getBlackTypeReading()
{
    return black_type_sensor_reading.first;
}

/*int* Communication::setColorReading()
{
    return color_sensor_reading.first;
}*/

void  Communication::upadateReadings()
{
    try
    {
        ultrasonic_sensor_reading = shared_memory->find<int>(SENSOR_ULTRASSOM);
        black_type_sensor_reading = shared_memory->find<int>(SENSOR_VISAO);
        feedback = shared_memory->find<int>(NOME_DO_INT_NA_MEMORIA2);
    }
    catch(...)
    {
        cout<<"programa nao abriu"<<endl;
    }
}
