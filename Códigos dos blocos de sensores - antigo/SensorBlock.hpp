#ifndef SENSORBLOCK_HPP_
#define SENSORBLOCK_HPP_

#include "Block.hpp"

using namespace std;


class SensorBlock : public Block {
    int sensor_type;
    Block next;

public:

    SensorBlock();
    ~SensorBlock();
    void setSensorType(int s);
    int getSensorType();
    void setNext(Block b);
    Block getNext();

};
#endif
