#include "Communication.hpp"

Communication::Communication()
{
    bool opened = false;
    int i;
    if(shared_memory!=NULL) //deletes the last opened vision shared memory if it exists
    {
        delete shared_memory;
    }
    try //tenta criar a memória
    {
        shared_memory = new managed_shared_memory (open_or_create, NOME_DA_MEMORIA, TAMANHO_DA_MEMORIA);
    }
    catch(...)
    {
        cout<<"erro ao criar memoria" << endl;
    }

    ultrasonic_sensor_reading = shared_memory->find<int>(SENSOR_ULTRASSOM)[N_ULTRASONIC]();
    black_type_sensor_reading = shared_memory->find<int>(SENSOR_VISAO)[N_BLACK_TAPE_SENSOR]();
    //color_sensor_reading = shared_memory->find'<unsigned short int >(POSICAO_DETECTADA)[N_ULTRASONIC]();
    command = shared_memory->construct<int>(NOME_DO_INT_NA_MEMORIA1);
	feedback = shared_memory->construct<int>(NOME_DO_INT_NA_MEMORIA2);
    virtual_robot = shared_memory->construct<int>(BLUETOOTH_ENABLE);
    *feedback = 0;
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
    *command.first = c;
}

void Communication::setIfVirtual(int i)
{
    *virtual_robot.first = i;
}

int Communication::getFeedback()
{
    return *feedback.first;
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
