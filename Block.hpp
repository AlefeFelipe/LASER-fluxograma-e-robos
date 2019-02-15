#ifndef BLOCK_HPP_
#define BLOCK_HPP_

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <iostream> // usada para debugging
#include <math.h>
#include <fstream>
#include <cstring>
#include <iostream>

using namespace std;


class Block {

    int x;
    int y;
    bool selected;

protected:
    int type;

public:

    Block();
    ~Block();
    void setX(int nx);
    int getX();
    void setY(int ny);
    int getY();
    int getType();
    void setSelected(bool s);
    bool getSelected();
    void setTypeOfSensor(int t);
    int getTypeOfSensor();
};
#endif
