#ifndef BLOCK_HPP_
#define BLOCK_HPP_

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <iostream> // usada para debugging
#include <math.h>
#include <fstream>
#include <cstring>
#include <iostream>
#include "fluxProgCte.h"

using namespace std;


class Block {

    int width;
    int height;
    bool selected, dragging, executing;
    bool out1_selected, out2_selected, in1_selected, in2_selected;
    const char* name;
    bool toDelete;

protected:
    int type;
    int x;
    int y;
    int pointIn1_x, pointIn1_y, pointIn2_x, pointIn2_y, pointOut1_x, pointOut1_y, pointOut2_x, pointOut2_y;

public:

    Block();
    virtual ~Block();
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
    virtual void setParameter1(int p);
    virtual int getParameter1();
    virtual void setParameter2(int p);
    virtual int getParameter2();
    virtual void setFunction(int f);
    virtual int getFunction();
    virtual void setNext1(Block *b);
    virtual Block* getNext1();
    virtual void setNext2(Block *b);
    virtual Block* getNext2();
    virtual void setPrevious1(Block *b);
    virtual Block* getPrevious1();
    virtual void setPrevious2(Block *b);
    virtual Block* getPrevious2();
    virtual int getPointIn1X();
    virtual int getPointIn1Y();
    virtual int getPointIn2X();
    virtual int getPointIn2Y();
    virtual int getPointOut1X();
    virtual int getPointOut1Y();
    virtual int getPointOut2X();
    virtual int getPointOut2Y();
    void setName(const char *n);
    char* getName();
    virtual Block* getExecutingNext();
    virtual void reset_loop_variables();
    virtual int getCommand();
    void setExecuting(bool s);
    bool getExecuting();
    void setDelete(bool t);
    bool getDelete();
    virtual void setValue(int v);
    virtual int getUnit();
    virtual int getTen();
};
#endif
