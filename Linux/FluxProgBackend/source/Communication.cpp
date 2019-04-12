#include "Communication.hpp"

Communication::Communication()
{
    bool opened = false;
    while(!opened)
    {
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
    abstraction_level = shared_memory->find<int>(MEMORY_ABSTRACTION);
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
    cout << "mandei "<<_feedback<<endl;
}

void Communication::setUltrasonicReading(int *ultrasonic_reading)
{
    for(int i=0; i < N_ULTRASONIC; i++)
    {
        ultrasonic_sensor_reading[i] = ultrasonic_reading[i];
    }
}

void Communication::setBlackTypeReading(int *black_type_reading)
{
    for(int i=0; i < N_BLACK_TAPE_SENSOR; i++)
    {
        black_type_sensor_reading[i] = black_type_reading[i];
    }
}

/*void Communication::setColorReading(int **color_reading)
{
    for(int i=0; i < N_COLOR_SENSOR; i++)
    {
        color_sensor_reading[i] = color_reading[i];
    }
}*/

int Communication::getAbstractionLevel()
{
    return *abstraction_level.first;
}
