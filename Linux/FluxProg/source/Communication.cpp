#include "Communication.hpp"

Communication::Communication()
{
    shared_memory = NULL;
    inicialize();
}

Communication::~Communication()
{
    if(shared_memory != NULL)
    {
        delete shared_memory;
    }
    shared_memory_object::remove(MEMORY_BLOCK);
}

void Communication::setCommand(int c)
{
    *command = c;
}

void Communication::setAbstractionLevel(int a)
{
    *abstraction_level = a;
}

void Communication::setIfVirtual(int i)
{
    *virtual_robot = i;
}

int Communication::getFeedback()
{
    return *feedback;
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
        ultrasonic_sensor_reading = shared_memory->find<int>(MEMORY_ULTRASONIC_SENSOR);
        black_type_sensor_reading = shared_memory->find<int>(MEMORY_VISION_SENSOR);
    }
    catch(...)
    {
        cout<<"programa nao abriu"<<endl;
    }
}

void Communication :: inicialize() {
    bool opened = false;
    while(!opened) {

        if(shared_memory!=NULL) //deletes the last opened vision shared memory if it exists
        {
            shared_memory_object::remove(MEMORY_BLOCK);
            delete shared_memory;
        }
        try //tenta criar a memória
        {
            shared_memory = new managed_shared_memory (create_only, MEMORY_BLOCK, MEMORY_BLOCK_SIZE);
            opened = true;
        }
        catch(...)
        {
            shared_memory_object::remove(MEMORY_BLOCK);
            cout<<"erro ao criar memoria" << endl;
        }
    }
    //color_sensor_reading = shared_memory->find'<unsigned short int >(POSICAO_DETECTADA).first;
    command = shared_memory->construct<int>(MEMORY_COMMAND)();
    abstraction_level = shared_memory->construct<int>(MEMORY_ABSTRACTION)();
    virtual_robot = shared_memory->construct<int>(MEMORY_ROBOT_TYPE)();
    feedback = shared_memory->construct<int>(MEMORY_FEEDBACK)();
    *feedback = 0;
    *command = 0;
    *abstraction_level = 0;
    *virtual_robot = 2;
}
