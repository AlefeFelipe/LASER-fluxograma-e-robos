//biblioteca feita para comunicacao entre fluxprog front end e back end

#ifndef _COMMUNICATION_HPP_
#define _COMMUNICATION_HPP_
#include "fluxProgBackendCte.h"
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/offset_ptr.hpp>
using namespace boost::interprocess;

class Communication{
    managed_shared_memory *shared_memory;
    int *ultrasonic_sensor_reading;
    int *black_type_sensor_reading;
    //int **color_sensor_reading;
    pair<int*, managed_shared_memory::size_type> command;
    pair<int*, managed_shared_memory::size_type> virtual_robot;
    pair<int*, managed_shared_memory::size_type> feedback;

public:
    Communication();
    ~Communication();
    int getCommand();
    int isVirtual();
    void setFeedback(int feedback);
    void setUltrasonicReading(float *ultrasonic_reading);
    void setBlackTypeReading(int *black_type_reading);
    void setColorReading(int **color_reading);
};

#endif
