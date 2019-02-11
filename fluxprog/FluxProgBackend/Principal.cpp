#include "Principal.hpp"

Principal::Principal()
{
    communication = new Communication();
    virtual_robot = NULL;
    physical_robot = NULL;
    feedback = 0;
}

Principal::~Principal()
{
    delete communication;
    if(virtual_robot != NULL)
    {
        delete virtual_robot;
    }
    if(physical_robot != NULL)
    {
        delete physical_robot;
    }
}

void Principal::connect()
{
    //abertura das paradas
    if(communication->isVirtual())
    {
        virtual_robot = new VirtualRobot();
        if (virtual_robot == NULL)//nao deu certo abrir a cena
        {
            feedback = ERROR;
            communication->setFeedback(feedback);
            ~Principal();
        }
        else
        {
            feedback = CONNECTED;
            communication->setFeedback(feedback);
        }
    }
    else
    {
        physical_robot = new VirtualRobot();
        if (physical_robot == NULL)//nao deu certo se conectar a porta
        {
            feedback = ERROR;
            communication->setFeedback(feedback);
            ~Principal();
        }
        else
        {
            feedback = CONNECTED;
            communication->setFeedback(feedback);
        }
    }
}

void Principal::start()
{
    connect();
    int command = 0;
    while(true)
    {
        feedback = 0;
        command = communication->getCommand();
        if(command > 0)
        {
            if(virtual_robot)
            {
                virtual_robot->setCommand(command);
                feedback = virtual_robot->isFinished();
                communication->setFeedback(feedback);
                communication->setUltrasonicReading(virtual_robot->getUltrasonicReading());
                communication->setBlackTypeReading(virtual_robot->getBlackTypeReading());
            }
            else
            {
                physical_robot->setCommand(command);
                feedback = physical_robot->isFinished();
                communication->setFeedback(feedback);
                communication->setUltrasonicReading(physical_robot->getUltrasonicReading());
                communication->setBlackTypeReading(physical_robot->getBlackTypeReading());
            }
        }
        else if(command == CLOSE_PROGRAM)
        {
            feedback = CLOSE_PROGRAM;
            communication->setFeedback(feedback);
            break;
        }
    }
    ~Principal();
}
