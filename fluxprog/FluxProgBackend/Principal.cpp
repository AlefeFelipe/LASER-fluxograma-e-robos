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
    int error = 0;
    if(communication->isVirtual())
    {
        virtual_robot = new VirtualRobot(&error);
    }
    else
    {
        physical_robot = new PhysicalRobot(&error);
    }
    if (error)//nao deu certo se conectar a porta ou a cena
    {
        feedback = ERROR;
        communication->setFeedback(feedback);
        if(virtual_robot)
        {
            delete virtual_robot;
        }
        else
        {
            delete physical_robot;
        }
    }
    else
    {
        feedback = CONNECTED;
        communication->setFeedback(feedback);
    }
}

void Principal::start()
{
    connect();
    int command = 0;
    while(feedback != ERROR && feedback != CLOSE_PROGRAM)
    {
        feedback = 0;
        command = communication->getCommand();
        if(command > 0)
        {
            if(virtual_robot)
            {
                virtual_robot->setCommand(command);
                feedback = virtual_robot->isFinished();
                if(feedback > 0)
                {
                    communication->setFeedback(feedback);
                    communication->setUltrasonicReading(virtual_robot->getUltrasonicReading());
                    communication->setBlackTypeReading(virtual_robot->getBlackTypeReading());
                }
                else if(feedback == -1)
                {
                    feedback = CLOSE_PROGRAM;
                    communication->setFeedback(feedback);
                }
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
        }
    }
}
