#include "FluxProgBackend.hpp"

FluxProgBackend::FluxProgBackend()
{
    communication = new Communication();
    virtual_robot = NULL;
    physical_robot = NULL;
    feedback = 0;
}

FluxProgBackend::~FluxProgBackend()
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

void FluxProgBackend::connect()
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
        feedback = READY;
        communication->setFeedback(feedback);
    }
}

void FluxProgBackend::start()
{
    connect();
    int command = 0;
    if(virtual_robot)
    {
        virtual_robot->updateSensorsReading();
        communication->setUltrasonicReading(virtual_robot->getUltrasonicReading());
        communication->setBlackTypeReading(virtual_robot->getBlackTypeReading());
    }
    else
    {
        physical_robot->updateSensorsReading();
        communication->setUltrasonicReading(physical_robot->getUltrasonicReading());
        communication->setBlackTypeReading(physical_robot->getBlackTypeReading());
    }
    while(feedback != ERROR && feedback != CLOSE_PROGRAM)
    {
        feedback = 0;
        command = communication->getCommand();
        if(command > 0)
        {
            if(virtual_robot)
            {
                communication->setFeedback(EXECUTING);
                virtual_robot->setCommand(command);
                virtual_robot->updateSensorsReading();
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
                communication->setFeedback(EXECUTING);
                physical_robot->setCommand(command);
                physical_robot->updateSensorsReading();
                feedback = physical_robot->isFinished();
                communication->setFeedback(feedback);
                communication->setUltrasonicReading(physical_robot->getUltrasonicReading());
                communication->setBlackTypeReading(physical_robot->getBlackTypeReading());
            }
        }
        else if(command == CLOSE_PROGRAM)
        {
            feedback = CLOSE_PROGRAM;
            std::cout <<"backend receive shutdown signal"<<std::endl;
            communication->setFeedback(feedback);
        }
    }
}
