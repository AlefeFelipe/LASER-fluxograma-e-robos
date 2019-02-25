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
    int width;
    int height;
    bool selected, dragging;
    bool out1_selected, out2_selected, in1_selected, in2_selected;

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
    void setDragging(bool s);
    bool getDragging();
    void setSelected(bool s);
    bool getSelected();
    void setOut1Selected(bool s);
    bool getOut1Selected();
    void setIn1Selected(bool s);
    bool getIn1Selected();
    void setOut2Selected(bool s);
    bool getOut2Selected();
    void setIn2Selected(bool s);
    bool getIn2Selected();
    void setWidth(int w);
    int getWidth();
    void setHeight(int h);
    int getHeight();
    virtual void setTypeOfSensor(int t);
    virtual int getTypeOfSensor();
    virtual void setFunction(int f);
    virtual int getFunction();
    //virtual void setNext1(Block *b);
    //virtual Block getNext1();
    //virtual void setNext2(Block *b);
    //virtual Block getNext2();
};
#endif
