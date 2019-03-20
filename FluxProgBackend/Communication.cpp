#include "Communication.hpp"

Communication::Communication()
{
    bool opened = false;
    int i;
    while(!opened)
    {
        if(shared_memory!=NULL) //deletes the last opened vision shared memory if it exists
        {
            delete shared_memory;
        }
        try //tries to open the vision shared memory until it succeeds
        {
            shared_memory = new managed_shared_memory (open_only, NOME_DA_MEMORIA);
            opened = true;
        }
        catch(...)
        {
            cout<<"erro ao abrir memoria 1" << i++ << endl;
        }
    }
    ultrasonic_sensor_reading = shared_memory->construct<int>(SENSOR_ULTRASSOM)[N_ULTRASONIC]();
    black_type_sensor_reading = shared_memory->construct<int>(SENSOR_VISAO)[N_BLACK_TAPE_SENSOR]();
    //color_sensor_reading = shared_memory->construct<unsigned short int >(POSICAO_DETECTADA)[N_ULTRASONIC]();
    command = shared_memory->find<int>(NOME_DO_INT_NA_MEMORIA1);
	feedback = shared_memory->find<int>(NOME_DO_INT_NA_MEMORIA2);
    virtual_robot = shared_memory->find<int>(BLUETOOTH_ENABLE);
}

Communication::~Communication()
{
    if(shared_memory != NULL)
    {
        delete shared_memory;
    }
    shared_memory_object::remove(NOME_DA_MEMORIA);
}

int Communication::getCommand()
{
    return *command.first;
}

int Communication::isVirtual()
{
    return *virtual_robot.first;
}

void Communication::setFeedback(int _feedback)
{
    *feedback.first = _feedback;
    *command.first = 0;//desnecessario
}

void Communication::setUltrasonicReading(int *ultrasonic_reading)
{
    ultrasonic_sensor_reading = ultrasonic_reading;
}

void Communication::setBlackTypeReading(int *black_type_reading)
{
    black_type_sensor_reading = black_type_reading;
}

/*void Communication::setColorReading(int **color_reading)
{
    color_sensor_reading = color_reading;
}*/
