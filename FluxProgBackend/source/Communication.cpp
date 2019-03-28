#include "Communication.hpp"

Communication::Communication()
{
    bool opened = false;
    while(!opened)
    {
        if(shared_memory!=NULL) //deletes the last opened vision shared memory if it exists
        {
            delete shared_memory;
        }
        try //tries to open the vision shared memory until it succeeds
        {
            shared_memory = new managed_shared_memory (open_only, MEMORY_BLOCK);
            opened = true;
        }
        catch(...)
        {
            cout<<"erro ao abrir memoria 1" << endl;
        }
    }
    ultrasonic_sensor_reading = shared_memory->construct<int>(MEMORY_ULTRASONIC_SENSOR)[N_ULTRASONIC]();
    black_type_sensor_reading = shared_memory->construct<int>(MEMORY_VISION_SENSOR)[N_BLACK_TAPE_SENSOR]();
    //color_sensor_reading = shared_memory->construct<unsigned short int >(POSICAO_DETECTADA)[N_ULTRASONIC]();
    command = shared_memory->find<int>(MEMORY_COMMAND);
    virtual_robot = shared_memory->find<int>(MEMORY_ROBOT_TYPE);
    feedback = shared_memory->find<int>(MEMORY_FEEDBACK);
    setFeedback(CONNECT);
}

Communication::~Communication()
{
    if(shared_memory != NULL)
    {
        delete shared_memory;
    }
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
    *command.first = 0;
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
