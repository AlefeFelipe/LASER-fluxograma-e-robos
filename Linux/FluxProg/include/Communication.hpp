//biblioteca feita para comunicacao entre fluxprog front end e back end

#ifndef _COMMUNICATION_HPP_
#define _COMMUNICATION_HPP_
#include "fluxProgCte.h"
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/offset_ptr.hpp>
#include <iostream>
using namespace boost::interprocess;
using namespace std;

class Communication{
    managed_shared_memory *shared_memory;
    pair<int*, managed_shared_memory::size_type> ultrasonic_sensor_reading;
    pair<int*, managed_shared_memory::size_type> black_type_sensor_reading;
    //pair<int*, managed_shared_memory::size_type> color_sensor_reading;
    int *command;
    int *abstraction_level;
    int *virtual_robot;
    int *feedback;

public:
    Communication();
    ~Communication();
    void setCommand(int c);
    void setAbstractionLevel(int a);
    void setIfVirtual(int i);
    int getFeedback();
    int* getUltrasonicReading();
    int* getBlackTypeReading();
    void upadateReadings();
    void inicialize();
    //int* getColorReading();
};

#endif
