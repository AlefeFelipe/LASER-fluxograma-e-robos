#ifndef _FLUXPROG_HPP_
#define _FLUXPROG_HPP_

#include <iostream> // usada para debugging
#include <cmath>
#include <fstream>
#include <string>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include "Block.hpp"
#include "StartBlock.hpp"
#include "ActionBlock.hpp"
#include "ConditionalBlock.hpp"
#include "EndBlock.hpp"
#include "LoopBlock.hpp"
#include "MergeBlock.hpp"
#include "Communication.hpp"
#include "fluxProgCte.h"
#include "Interface.hpp"
#include "SaveFile.hpp"
#include "LoadFile.hpp"
#include <algorithm>
using namespace std;
#ifdef WIN32
    #include <windows.h>
#endif // WIN32

#define valor_maximo_blocos 100

class FluxProg {

    Block* blocks_list_to_print[valor_maximo_blocos];
    bool executing_fluxogram;
    Block* current_executing_block;
    bool simulator_connected, program_connected;
    Communication* communication;
    Interface* gui;
    bool paused;
    string program_path;
    int ids;
    bool already_saved;
    char* path;
    SaveFile* save;
    LoadFile* load;

    void add_block(Block* b);
    void remove_block(Block* b);
    void refresh_executing_block();
    void execute();
    bool check_if_only_one_startblock_exists();
    bool check_if_at_least_one_endblock_exist();
    bool check_if_all_the_blocks_have_connections();
    bool check_if_all_blocks_have_functions_or_sensors();
    void reset_fluxogram_execution();

    void connect_simulator();
    void connect_robot();
    void connect();
    string getExecutablePath();
    void reset_executing_block();
    int check_abstraction_level();
    void reset_blocks_list();

public:

    FluxProg();
    ~FluxProg();
    void start();
};
#endif
